#include "pch.h"
#include "SocketUtils.h"

/*----------------
	SocketUtils
-----------------*/

// LPFN_XXXX : ���� Ȯ�� �Լ��� �����͸� �޾ƿ� �Լ� �����͵�
LPFN_CONNECTEX		SocketUtils::ConnectEx = nullptr;
LPFN_DISCONNECTEX	SocketUtils::DisconnectEx = nullptr;
LPFN_ACCEPTEX		SocketUtils::AcceptEx = nullptr;

void SocketUtils::Init()
{
	WSADATA wsaData;
	ASSERT_CRASH(::WSAStartup(MAKEWORD(2, 2), OUT &wsaData) == 0);
	
	/* ��Ÿ�ӿ� �ּ� ������ API */
	SOCKET dummySocket = CreateSocket();
	// ���� connect�Լ��� Ȯ�� �Լ� connectex�Լ��� �����͸� �޾� ��
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)));
	// ���� disconnect�Լ��� Ȯ�� �Լ� disconnectex�Լ��� �����͸� �޾� ��
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)));
	// ���� accept�Լ��� Ȯ�� �Լ� acceptex�Լ��� �����͸� �޾� ��
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)));
	Close(dummySocket);
}

void SocketUtils::Clear()
{
	::WSACleanup();
}

bool SocketUtils::BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn)
{
	DWORD bytes = 0;
	/* WSAIoctl ( ������ ��带 �����ϴ� �Լ� )
	 [in] s : ����
	 [in] dwIoControlCode : The control code of operation to perform.
	 [in] lpvInBuffer : A pointer to the input buffer.
	 [in] cbInBuffer : The size, in bytes, of the input buffer.
	 [out] lpvOutBuffer : A pointer to the output buffer.
	 [in] cbOutBuffer : The size, in bytes, of the output buffer.
	 [out] lpcbBytesReturned : A pointer to actual number of bytes of output.
	 [in] lpOverlapped : A pointer to a WSAOVERLAPPED structure (ignored for non-overlapped sockets).
	 [in] lpCompletionRoutine : Type - _In_opt_ LPWSAOVERLAPPED_COMPLETION_ROUTINE

	 ������ ��Ÿ ���ĺ��ʹ� ���� Ȯ�� �Լ��� �޾�
	*/
	// SIO_GET_EXTENSION_FUNCTION_POINTER�� ���� Ȯ�� �Լ� ����� ���� ������ ��û �ڵ�
	return SOCKET_ERROR != ::WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL);
}

SOCKET SocketUtils::CreateSocket()
{
	/* WSA_FLAG_OVERLAPPED 
	  - ��ø ������ �����. 
	    ��ø ������ WSASend, WSASendTo, WSARecv, WSARecvFrom, WSAIoctl���� �Լ� ��� ����.
		 �� ���� ������� ������ �� ��ø �������� ���������.*/
	return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

bool SocketUtils::SetLinger(SOCKET socket, uint16 onoff, uint16 linger)
{
	LINGER option;
	option.l_onoff = onoff;
	option.l_linger = linger;
	return SetSockOpt(socket, SOL_SOCKET, SO_LINGER, option);
}

bool SocketUtils::SetReuseAddress(SOCKET socket, bool flag)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_REUSEADDR, flag);
}

bool SocketUtils::SetRecvBufferSize(SOCKET socket, int32 size)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_RCVBUF, size);
}

bool SocketUtils::SetSendBufferSize(SOCKET socket, int32 size)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_SNDBUF, size);
}

bool SocketUtils::SetTcpNoDelay(SOCKET socket, bool flag)
{
	return SetSockOpt(socket, SOL_SOCKET, TCP_NODELAY, flag);
}

// ListenSocket�� Ư���� ClientSocket�� �״�� ����
bool SocketUtils::SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listenSocket);
}

bool SocketUtils::Bind(SOCKET socket, NetAddress netAddr)
{
	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<const SOCKADDR*>(&netAddr.GetSockAddr()), sizeof(SOCKADDR_IN));
}

bool SocketUtils::BindAnyAddress(SOCKET socket, uint16 port)
{
	SOCKADDR_IN myAddress;
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr = ::htonl(INADDR_ANY);
	myAddress.sin_port = ::htons(port);

	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<const SOCKADDR*>(&myAddress), sizeof(myAddress));
}

bool SocketUtils::Listen(SOCKET socket, int32 backlog)
{
	return SOCKET_ERROR != ::listen(socket, backlog);
}

void SocketUtils::Close(SOCKET& socket)
{
	if (socket != INVALID_SOCKET)
		::closesocket(socket);
	socket = INVALID_SOCKET;
}