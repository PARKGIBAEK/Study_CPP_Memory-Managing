#include "Listener.h"
#include "../Network/IocpEvent.h"
#include "../Network/ServerService.h"
#include "../Memory/MemoryManager.h"
#include "../Network/SocketUtils.h"
#include "../Network/Session.h"
// #include "../Network/Service.h"
#include "../Network/IocpService.h"

namespace ServerCore
{
Listener::Listener():listenSocket(INVALID_SOCKET)
{
}

Listener::~Listener()
{
	SocketUtils::Close(listenSocket);

	for (AcceptEvent* acceptEvent : acceptEvents)
	{
		// TODO

		XDelete(acceptEvent);
	}
}

bool Listener::StartAccept(std::shared_ptr<ServerService>_ownerService)
{
	ownerService = _ownerService;
	if (_ownerService == nullptr)
		return false;

	listenSocket = SocketUtils::CreateSocket();
	if (listenSocket == INVALID_SOCKET)
		return false;

	// Listening 소켓을 IOCP에 등록
	if (ownerService->GetIocpCore()->RegisterSocketToIocp(shared_from_this()) == false)
		return false;

	if (SocketUtils::SetReuseAddress(listenSocket, true) == false)
		return false;

	if (SocketUtils::SetLinger(listenSocket, 0, 0) == false)
		return false;

	if (SocketUtils::Bind(listenSocket, ownerService->GetNetAddress()) == false)
		return false;

	if (SocketUtils::Listen(listenSocket) == false)
		return false;

	// 최대 동접수 만큼 AcceptEvent생성 & RegisterAccept 호출
	const int32 acceptCount = ownerService->GetMaxSessionCount();
	for (int32 i = 0; i < acceptCount; i++)
	{
		// IOCP 사용을 위한 OVERLAPPED 구조체를 상속받은 AcceptEvent를 MaxSessionCount만큼 미리 등록해두기
		AcceptEvent* acceptEvent = XNew<AcceptEvent>();
		// AcceptEvent의 OwnerSession에 Listner를 등록해두고 나중에 IocpService의 GQCS 호출부에서 처리
		acceptEvent->ownerDispatcher = shared_from_this();
		acceptEvents.emplace_back(acceptEvent);
		RegisterAccept(acceptEvent);
	}

	return true;
}

void Listener::CloseSocket()
{
	SocketUtils::Close(listenSocket);
}

HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(listenSocket);
}

// 사용 안함
void Listener::DispatchEvent(IocpEvent* _iocpEvent, int32 _numOfBytes)
{
	ASSERT_CRASH(_iocpEvent->eventType == EventType::Accept)
	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(_iocpEvent);
	ProcessAccept(acceptEvent);
}

void Listener::RegisterAccept(AcceptEvent* _acceptEvent)
{
	std::shared_ptr<Session> session = ownerService->CreateSession(); // Register IOCP

	_acceptEvent->Init();
	_acceptEvent->session = session;

	DWORD bytesReceived = 0;
	/* AcceptEx함수를 통해 접속을 받으면 OS가 IOCP로 통지 해주기 때문에 
	   GQCS함수를 통해 후처리를 할 수 있다.
			
	   AcceptEx 함수 인자 요약
	   : ListenSocket, AcceptSocket, outputBuffer주소, receiveDataLength, LocalAddressLength, BytesReceived, Overlapped
	  */
	if (false == 
		SocketUtils::AcceptEx(listenSocket, session->GetSocket(),
		session->recvBuffer.WritePos(), 0, sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16, OUT & bytesReceived,
		static_cast<LPOVERLAPPED>(_acceptEvent)))
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)// IO_PENDING이 아닌경우 문제가 
		{
			// 일단 다시 Accept 걸어준다
			RegisterAccept(_acceptEvent);
		}
	}
}

// 사용 안함(why?)
void Listener::ProcessAccept(AcceptEvent* _acceptEvent)
{
	std::shared_ptr<Session> session = 
		std::static_pointer_cast<Session>(_acceptEvent->session);
	// ListenSocket의 특성을 ClientSocket에 그대로 적용하기
	if (false == SocketUtils::SyncSocketContext(session->GetSocket(), listenSocket))
	{
		RegisterAccept(_acceptEvent);
		return;
	}

	SOCKADDR_IN sockAddress;
	int32 sizeOfSockAddr = sizeof(sockAddress);
	// getpeername함수는 accepted socket의 local address를 조회
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), 
		OUT reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeOfSockAddr))
	{
		RegisterAccept(_acceptEvent);
		return;
	}

	session->SetNetAddress(NetAddress(sockAddress));
	session->ProcessConnect();
	RegisterAccept(_acceptEvent);
}
}