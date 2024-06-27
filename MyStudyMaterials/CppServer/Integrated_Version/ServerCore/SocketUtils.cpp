#include "SocketUtils.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>


#include "CoreMacro.h"

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

	// ���� connect�Լ��� Ȯ�� �Լ� ConnectEx�Լ��� �����͸� �޾� ��
	ASSERT_CRASH(
		BindWindowsFunction(
			dummySocket, WSAID_CONNECTEX, 
			reinterpret_cast<LPVOID*>(&ConnectEx))
	);

	// ���� disconnect�Լ��� Ȯ�� �Լ� DisconnectEx�Լ��� �����͸� �޾� ��
	ASSERT_CRASH(
		BindWindowsFunction(
			dummySocket, WSAID_DISCONNECTEX, 
			reinterpret_cast<LPVOID*>(&DisconnectEx))
	);

	// ���� accept�Լ��� Ȯ�� �Լ� AcceptEx�Լ��� �����͸� �޾� ��
	ASSERT_CRASH(
		BindWindowsFunction(
			dummySocket, WSAID_ACCEPTEX,
			reinterpret_cast<LPVOID*>(&AcceptEx))
	);

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
		
		WSAIoctl( ListenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER,
		  &GuidAcceptEx, sizeof (GuidAcceptEx),
		  &lpfnAcceptEx, sizeof (lpfnAcceptEx),
		  &dwBytes, NULL, NULL);
	
	 [in] s : ����
	 [in] dwIoControlCode : The control code of operation to perform.
	 [in] lpvInBuffer : A pointer to the input buffer.
	 [in] cbInBuffer : The size, in bytes, of the input buffer.
	 [out] lpvOutBuffer : A pointer to the output buffer.
	 [in] cbOutBuffer : The size, in bytes, of the output buffer.
	 [out] lpcbBytesReturned : A pointer to actual number of bytes of output.
	 [in] lpOverlapped : A pointer to a WSAOVERLAPPED structure (ignored for non-overlapped sockets).
	 [in] lpCompletionRoutine : Type - _In_opt_ LPWSAOVERLAPPED_COMPLETION_ROUTINE

	 ������ ��Ÿ ���ĺ��ʹ� ���� Ȯ�� �Լ��� �޾� ��� ����
	*/

	// SIO_GET_EXTENSION_FUNCTION_POINTER�� ���� Ȯ�� �Լ� ����� ���� ������ ��û �ڵ�
	return SOCKET_ERROR != ::WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL);
}

SOCKET SocketUtils::CreateSocket()
{
	/* WSA_FLAG_OVERLAPPED 
	  - Overlapped I/O ������ �����Ǹ�,
	    �ش� ������ WSASend, WSASendTo, WSARecv, WSARecvFrom, WSAIoctl���� �Լ� ��� ����.
		 �� ���� ������� ������ Non-Overlapped �������� ���������.*/
	return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

bool SocketUtils::SetLinger(SOCKET socket, uint16 onoff, uint16 lingerTime)
{// Linger �ɼ��� ������ close���� �� ���۵��� ���� �����͸� ��� ó���� ���ΰ��� ���� �ɼ�
	LINGER option;
	option.l_onoff = onoff; // �ɼ� Ȱ��/��Ȱ��
	option.l_linger = lingerTime; // �ɼ� Ȱ�� �ð�
	return SetSockOpt(socket, SOL_SOCKET, SO_LINGER, option);
}

bool SocketUtils::SetReuseAddress(SOCKET socket, bool flag)
{/* Ŀ���� ������ ��Ʈ�� �����ϰ� �ִ� ��Ȳ���� ���� ���α׷��� ����� �Ŀ��� 
	Ŀ���� �����ð� ���� ������ ��� �����Ѵ�.
	������ SO_REUSEADDR�ɼ��� �����ϸ� Ŀ���� ���� ���� ������ ���� �� �� �ִ�*/
	return SetSockOpt(socket, SOL_SOCKET, SO_REUSEADDR, flag);
}

bool SocketUtils::SetRecvBufferSize(SOCKET socket, int32 size)
{// SO_RECVBUF�� Ŀ���� ���� ���� ũ�⸦ �����ϴ� �ɼ�
	return SetSockOpt(socket, SOL_SOCKET, SO_RCVBUF, size);
}

bool SocketUtils::SetSendBufferSize(SOCKET socket, int32 size)
{// SO_SNDBUF�� Ŀ���� �۽� ���� ũ�⸦ �����ϴ� �ɼ�
	return SetSockOpt(socket, SOL_SOCKET, SO_SNDBUF, size);
}

bool SocketUtils::SetTcpNoDelay(SOCKET socket, bool flag)
{// NAGLE �˰��� On/Off
	return SetSockOpt(socket, SOL_SOCKET, TCP_NODELAY, flag);
}


bool SocketUtils::SyncSocketContext(SOCKET socket, SOCKET listenSocket)
{ // listenSocket�� �Ӽ��� socket(Ŭ�� ����)�� �״�� �����ϱ�(�̰� ���ϸ� local/remote address �޾ƿ��� ���� ������ ��)
	return SetSockOpt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listenSocket);
}

bool SocketUtils::Bind(SOCKET socket, NetAddress netAddr)
{// ���� ���ε�
	return SOCKET_ERROR != ::bind(socket,
		reinterpret_cast<const SOCKADDR*>(&netAddr.GetSockAddr()),
		sizeof(SOCKADDR_IN));
}

bool SocketUtils::BindAnyAddress(SOCKET socket, uint16 port)
{// ���� �ּҸ� INADDR_ANY�� ���ε�
	SOCKADDR_IN myAddress;
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr = ::htonl(INADDR_ANY);
	myAddress.sin_port = ::htons(port);

	return SOCKET_ERROR != ::bind(
		socket, reinterpret_cast<const SOCKADDR*>(&myAddress),
		sizeof(myAddress));
}

bool SocketUtils::Listen(SOCKET socket, int32 backlog)
{// Listen ����
	return SOCKET_ERROR != ::listen(socket, backlog);
}

void SocketUtils::Close(SOCKET& socket)
{// ���� �ڿ� ��ȯ
	if (socket != INVALID_SOCKET)
		::closesocket(socket);
	socket = INVALID_SOCKET;
}