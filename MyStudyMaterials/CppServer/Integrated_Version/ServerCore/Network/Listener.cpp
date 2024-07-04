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

	// Listening ������ IOCP�� ���
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

	// �ִ� ������ ��ŭ AcceptEvent���� & RegisterAccept ȣ��
	const int32 acceptCount = ownerService->GetMaxSessionCount();
	for (int32 i = 0; i < acceptCount; i++)
	{
		// IOCP ����� ���� OVERLAPPED ����ü�� ��ӹ��� AcceptEvent�� MaxSessionCount��ŭ �̸� ����صα�
		AcceptEvent* acceptEvent = XNew<AcceptEvent>();
		// AcceptEvent�� OwnerSession�� Listner�� ����صΰ� ���߿� IocpService�� GQCS ȣ��ο��� ó��
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

// ��� ����
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
	/* AcceptEx�Լ��� ���� ������ ������ OS�� IOCP�� ���� ���ֱ� ������ 
	   GQCS�Լ��� ���� ��ó���� �� �� �ִ�.
			
	   AcceptEx �Լ� ���� ���
	   : ListenSocket, AcceptSocket, outputBuffer�ּ�, receiveDataLength, LocalAddressLength, BytesReceived, Overlapped
	  */
	if (false == 
		SocketUtils::AcceptEx(listenSocket, session->GetSocket(),
		session->recvBuffer.WritePos(), 0, sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16, OUT & bytesReceived,
		static_cast<LPOVERLAPPED>(_acceptEvent)))
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)// IO_PENDING�� �ƴѰ�� ������ 
		{
			// �ϴ� �ٽ� Accept �ɾ��ش�
			RegisterAccept(_acceptEvent);
		}
	}
}

// ��� ����(why?)
void Listener::ProcessAccept(AcceptEvent* _acceptEvent)
{
	std::shared_ptr<Session> session = 
		std::static_pointer_cast<Session>(_acceptEvent->session);
	// ListenSocket�� Ư���� ClientSocket�� �״�� �����ϱ�
	if (false == SocketUtils::SyncSocketContext(session->GetSocket(), listenSocket))
	{
		RegisterAccept(_acceptEvent);
		return;
	}

	SOCKADDR_IN sockAddress;
	int32 sizeOfSockAddr = sizeof(sockAddress);
	// getpeername�Լ��� accepted socket�� local address�� ��ȸ
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