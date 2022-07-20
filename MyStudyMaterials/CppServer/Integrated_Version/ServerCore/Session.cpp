#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"
#include "Service.h"

/*--------------
	Session
---------------*/

Session::Session() : recvBuffer(BUFFER_SIZE)
{
	socket = SocketUtils::CreateSocket();
}

Session::~Session()
{
	SocketUtils::Close(socket);
}

void Session::Send(SendBufferRef sendBuffer)
{
	if (IsConnected() == false)
		return;

	bool registerSend = false;

	// 현재 RegisterSend가 걸리지 않은 상태라면, 걸어준다
	{
		WRITE_LOCK;

		sendQueue.push(sendBuffer);

		if (isSendRegistered.exchange(true) == false)
			registerSend = true;
	}
	
	if (registerSend)
		RegisterSend();
}

bool Session::Connect()
{
	return RegisterConnect();
}

void Session::Disconnect(const WCHAR* cause)
{
	if (isConnected.exchange(false) == false)
		return;

	// TEMP
	wcout << "Disconnect : " << cause << endl;

	RegisterDisconnect();
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	switch (iocpEvent->eventType)
	{
	case EventType::Connect:
		ProcessConnect();
		break;
	case EventType::Disconnect:
		ProcessDisconnect();
		break;
	case EventType::Recv:
		ProcessRecv(numOfBytes);
		break;
	case EventType::Send:
		ProcessSend(numOfBytes);
		break;
	default:
		break;
	}
}

bool Session::RegisterConnect()
{
	if (IsConnected())
		return false;

	if (GetService()->GetServiceType() != ServiceType::Client)
		return false;

	if (SocketUtils::SetReuseAddress(socket, true) == false)
		return false;

	if (SocketUtils::BindAnyAddress(socket, 0/*남는거*/) == false)
		return false;

	connectEvent.Init();
	connectEvent.owner = shared_from_this(); // ADD_REF

	DWORD numOfBytes = 0;
	SOCKADDR_IN sockAddr = GetService()->GetNetAddress().GetSockAddr();
	
	if (false == SocketUtils::ConnectEx(socket, 
		reinterpret_cast<SOCKADDR*>(&sockAddr), 
		sizeof(sockAddr), nullptr, 0, &numOfBytes, &connectEvent))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			connectEvent.owner = nullptr; // RELEASE_REF
			return false;
		}
	}

	return true;
}

bool Session::RegisterDisconnect()
{
	disconnectEvent.Init();
	disconnectEvent.owner = shared_from_this(); // ADD_REF

	if (false == SocketUtils::DisconnectEx(socket, &disconnectEvent,
		TF_REUSE_SOCKET, 0))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			disconnectEvent.owner = nullptr; // RELEASE_REF
			return false;
		}
	}

	return true;
}

void Session::RegisterRecv()
{
	if (IsConnected() == false)
		return;

	recvEvent.Init();
	recvEvent.owner = shared_from_this(); // ADD_REF

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(recvBuffer.WritePos());
	wsaBuf.len = recvBuffer.FreeSize();

	DWORD bytesReceived = 0;
	DWORD flags = 0;
	if (SOCKET_ERROR == ::WSARecv(socket, &wsaBuf, 1, OUT &bytesReceived, 
		OUT &flags, &recvEvent, nullptr))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			recvEvent.owner = nullptr; // RELEASE_REF
		}
	}
}

void Session::RegisterSend()
{
	if (IsConnected() == false)
		return;

	sendEvent.Init();
	sendEvent.owner = shared_from_this(); // ADD_REF

	// 보낼 데이터를 sendEvent에 등록
	{
		WRITE_LOCK;

		int32 writeSize = 0;
		while (sendQueue.empty() == false)
		{
			SendBufferRef sendBuffer = sendQueue.front();

			writeSize += sendBuffer->WriteSize();
			// TODO : 예외 체크

			sendQueue.pop();
			sendEvent.sendBuffers.push_back(sendBuffer);
		}
	}

	/* Gathering write(모아 보내기)*/
	Vector<WSABUF> wsaBufs;
	wsaBufs.reserve(/*보낼 패킷 수*/sendEvent.sendBuffers.size());
	for (SendBufferRef sendBuffer : sendEvent.sendBuffers)
	{
		WSABUF wsaBuf;
		wsaBuf.buf = reinterpret_cast<char*>(sendBuffer->Buffer());
		wsaBuf.len = static_cast<LONG>(sendBuffer->WriteSize());
		wsaBufs.push_back(wsaBuf);
	}
	DWORD bytesTransferred = 0;
	if (SOCKET_ERROR ==
		::WSASend(socket, wsaBufs.data(), static_cast<DWORD>(wsaBufs.size()),
			OUT &bytesTransferred, 0, &sendEvent, nullptr)
		)
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			sendEvent.owner = nullptr; // RELEASE_REF
			sendEvent.sendBuffers.clear(); // RELEASE_REF
			isSendRegistered.store(false);
		}
	}
}

void Session::ProcessConnect()
{
	connectEvent.owner = nullptr; // RELEASE_REF

	isConnected.store(true);

	// 세션 등록
	GetService()->AddSession(GetSessionRef());

	// 컨텐츠 코드에서 재정의
	OnConnected();

	// 수신 등록
	RegisterRecv();
}

void Session::ProcessDisconnect()
{
	disconnectEvent.owner = nullptr; // RELEASE_REF

	OnDisconnected(); // 컨텐츠 코드에서 재정의
	GetService()->ReleaseSession(GetSessionRef());
}

void Session::ProcessRecv(int32 numOfBytes)
{
	recvEvent.owner = nullptr; // RELEASE_REF

	if (numOfBytes == 0)
	{
		Disconnect(L"Recv 0");
		return;
	}

	if (recvBuffer.OnWrite(numOfBytes) == false)
	{
		Disconnect(L"OnWrite Overflow");
		return;
	}

	int32 dataSize = recvBuffer.DataSize();
	int32 processLen = OnRecv(recvBuffer.ReadPos(), dataSize); // 컨텐츠 코드에서 재정의
	if (processLen < 0 || dataSize < processLen ||
		recvBuffer.OnRead(processLen) == false)
	{
		Disconnect(L"OnRead Overflow");
		return;
	}

	
	// 커서 정리
	recvBuffer.Clean();

	// 수신 등록
	RegisterRecv();
}

void Session::ProcessSend(int32 numOfBytes)
{
	sendEvent.owner = nullptr; // RELEASE_REF
	sendEvent.sendBuffers.clear(); // RELEASE_REF

	if (numOfBytes == 0)
	{
		Disconnect(L"Send 0");
		return;
	}

	// 컨텐츠 코드에서 재정의
	OnSend(numOfBytes);

	WRITE_LOCK;
	if (sendQueue.empty())
		isSendRegistered.store(false);
	else
		RegisterSend();
}

void Session::HandleError(int32 errorCode)
{
	switch (errorCode)
	{
	case WSAECONNRESET:
	case WSAECONNABORTED:
		Disconnect(L"HandleError");
		break;
	default:
		// TODO : Log
		cout << "Handle Error : " << errorCode << endl;
		break;
	}
}

/*-----------------
	PacketSession
------------------*/

PacketSession::PacketSession()
{
}

PacketSession::~PacketSession()
{
}

// [size(2)][id(2)][data....][size(2)][id(2)][data....]
int32 PacketSession::OnRecv(BYTE* buffer, int32 len)
{
	int32 processedLen = 0;

	while (true)
	{
		int32 dataSize = len - processedLen;
		// 최소한 헤더는 파싱할 수 있어야 한다
		if (dataSize < sizeof(PacketHeader))
			break;

		PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[processedLen]));
		
		// 파싱할 수 있을 만큼의 길의 패킷을 수신하지 않은 경우 종료(헤더에 기록된 패킷 크기 만큼 파싱할 수 있어야 함)
		if (dataSize < header.size)
			break;

		// 패킷 추출 & PacketHandler로 전달하여 패킷 처리
		OnRecvPacket(&buffer[processedLen], header.size);

		processedLen += header.size;
	}

	return processedLen;
}