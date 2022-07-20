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

	// ���� RegisterSend�� �ɸ��� ���� ���¶��, �ɾ��ش�
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

	if (SocketUtils::BindAnyAddress(socket, 0/*���°�*/) == false)
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

	// ���� �����͸� sendEvent�� ���
	{
		WRITE_LOCK;

		int32 writeSize = 0;
		while (sendQueue.empty() == false)
		{
			SendBufferRef sendBuffer = sendQueue.front();

			writeSize += sendBuffer->WriteSize();
			// TODO : ���� üũ

			sendQueue.pop();
			sendEvent.sendBuffers.push_back(sendBuffer);
		}
	}

	/* Gathering write(��� ������)*/
	Vector<WSABUF> wsaBufs;
	wsaBufs.reserve(/*���� ��Ŷ ��*/sendEvent.sendBuffers.size());
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

	// ���� ���
	GetService()->AddSession(GetSessionRef());

	// ������ �ڵ忡�� ������
	OnConnected();

	// ���� ���
	RegisterRecv();
}

void Session::ProcessDisconnect()
{
	disconnectEvent.owner = nullptr; // RELEASE_REF

	OnDisconnected(); // ������ �ڵ忡�� ������
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
	int32 processLen = OnRecv(recvBuffer.ReadPos(), dataSize); // ������ �ڵ忡�� ������
	if (processLen < 0 || dataSize < processLen ||
		recvBuffer.OnRead(processLen) == false)
	{
		Disconnect(L"OnRead Overflow");
		return;
	}

	
	// Ŀ�� ����
	recvBuffer.Clean();

	// ���� ���
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

	// ������ �ڵ忡�� ������
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
		// �ּ��� ����� �Ľ��� �� �־�� �Ѵ�
		if (dataSize < sizeof(PacketHeader))
			break;

		PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[processedLen]));
		
		// �Ľ��� �� ���� ��ŭ�� ���� ��Ŷ�� �������� ���� ��� ����(����� ��ϵ� ��Ŷ ũ�� ��ŭ �Ľ��� �� �־�� ��)
		if (dataSize < header.size)
			break;

		// ��Ŷ ���� & PacketHandler�� �����Ͽ� ��Ŷ ó��
		OnRecvPacket(&buffer[processedLen], header.size);

		processedLen += header.size;
	}

	return processedLen;
}