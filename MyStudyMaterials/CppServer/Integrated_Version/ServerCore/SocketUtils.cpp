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

// LPFN_XXXX : 윈속 확장 함수의 포인터를 받아올 함수 포인터들
LPFN_CONNECTEX		SocketUtils::ConnectEx = nullptr;
LPFN_DISCONNECTEX	SocketUtils::DisconnectEx = nullptr;
LPFN_ACCEPTEX		SocketUtils::AcceptEx = nullptr;

void SocketUtils::Init()
{
	WSADATA wsaData;
	ASSERT_CRASH(::WSAStartup(MAKEWORD(2, 2), OUT &wsaData) == 0);
	
	/* 런타임에 주소 얻어오는 API */
	SOCKET dummySocket = CreateSocket();

	// 윈속 connect함수의 확장 함수 ConnectEx함수의 포인터를 받아 옴
	ASSERT_CRASH(
		BindWindowsFunction(
			dummySocket, WSAID_CONNECTEX, 
			reinterpret_cast<LPVOID*>(&ConnectEx))
	);

	// 윈속 disconnect함수의 확장 함수 DisconnectEx함수의 포인터를 받아 옴
	ASSERT_CRASH(
		BindWindowsFunction(
			dummySocket, WSAID_DISCONNECTEX, 
			reinterpret_cast<LPVOID*>(&DisconnectEx))
	);

	// 윈속 accept함수의 확장 함수 AcceptEx함수의 포인터를 받아 옴
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
	/* WSAIoctl ( 소켓의 모드를 조정하는 함수 )
		
		WSAIoctl( ListenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER,
		  &GuidAcceptEx, sizeof (GuidAcceptEx),
		  &lpfnAcceptEx, sizeof (lpfnAcceptEx),
		  &dwBytes, NULL, NULL);
	
	 [in] s : 소켓
	 [in] dwIoControlCode : The control code of operation to perform.
	 [in] lpvInBuffer : A pointer to the input buffer.
	 [in] cbInBuffer : The size, in bytes, of the input buffer.
	 [out] lpvOutBuffer : A pointer to the output buffer.
	 [in] cbOutBuffer : The size, in bytes, of the output buffer.
	 [out] lpcbBytesReturned : A pointer to actual number of bytes of output.
	 [in] lpOverlapped : A pointer to a WSAOVERLAPPED structure (ignored for non-overlapped sockets).
	 [in] lpCompletionRoutine : Type - _In_opt_ LPWSAOVERLAPPED_COMPLETION_ROUTINE

	 윈도우 비스타 이후부터는 윈속 확장 함수를 받아 사용 가능
	*/

	// SIO_GET_EXTENSION_FUNCTION_POINTER는 윈속 확장 함수 사용을 위한 포인터 요청 코드
	return SOCKET_ERROR != ::WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL);
}

SOCKET SocketUtils::CreateSocket()
{
	/* WSA_FLAG_OVERLAPPED 
	  - Overlapped I/O 소켓이 생성되며,
	    해당 소켓은 WSASend, WSASendTo, WSARecv, WSARecvFrom, WSAIoctl등의 함수 사용 가능.
		 이 값을 명시하지 않으면 Non-Overlapped 소켓으로 만들어진다.*/
	return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

bool SocketUtils::SetLinger(SOCKET socket, uint16 onoff, uint16 lingerTime)
{// Linger 옵션은 소켓을 close했을 때 전송되지 않은 데이터를 어떻게 처리할 것인가에 대한 옵션
	LINGER option;
	option.l_onoff = onoff; // 옵션 활성/비활성
	option.l_linger = lingerTime; // 옵션 활성 시간
	return SetSockOpt(socket, SOL_SOCKET, SO_LINGER, option);
}

bool SocketUtils::SetReuseAddress(SOCKET socket, bool flag)
{/* 커널이 소켓의 포트를 점유하고 있는 상황에서 서버 프로그램이 종료된 후에도 
	커널은 일정시간 동안 소켓을 계속 점유한다.
	하지만 SO_REUSEADDR옵션을 적용하면 커널이 점유 중인 소켓을 재사용 할 수 있다*/
	return SetSockOpt(socket, SOL_SOCKET, SO_REUSEADDR, flag);
}

bool SocketUtils::SetRecvBufferSize(SOCKET socket, int32 size)
{// SO_RECVBUF는 커널의 수신 버퍼 크기를 조절하는 옵션
	return SetSockOpt(socket, SOL_SOCKET, SO_RCVBUF, size);
}

bool SocketUtils::SetSendBufferSize(SOCKET socket, int32 size)
{// SO_SNDBUF는 커널의 송신 버퍼 크기를 조절하는 옵션
	return SetSockOpt(socket, SOL_SOCKET, SO_SNDBUF, size);
}

bool SocketUtils::SetTcpNoDelay(SOCKET socket, bool flag)
{// NAGLE 알고리즘 On/Off
	return SetSockOpt(socket, SOL_SOCKET, TCP_NODELAY, flag);
}


bool SocketUtils::SyncSocketContext(SOCKET socket, SOCKET listenSocket)
{ // listenSocket의 속성을 socket(클라 소켓)에 그대로 적용하기(이걸 안하면 local/remote address 받아오는 과정 복잡해 짐)
	return SetSockOpt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listenSocket);
}

bool SocketUtils::Bind(SOCKET socket, NetAddress netAddr)
{// 소켓 바인딩
	return SOCKET_ERROR != ::bind(socket,
		reinterpret_cast<const SOCKADDR*>(&netAddr.GetSockAddr()),
		sizeof(SOCKADDR_IN));
}

bool SocketUtils::BindAnyAddress(SOCKET socket, uint16 port)
{// 소켓 주소를 INADDR_ANY로 바인딩
	SOCKADDR_IN myAddress;
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr = ::htonl(INADDR_ANY);
	myAddress.sin_port = ::htons(port);

	return SOCKET_ERROR != ::bind(
		socket, reinterpret_cast<const SOCKADDR*>(&myAddress),
		sizeof(myAddress));
}

bool SocketUtils::Listen(SOCKET socket, int32 backlog)
{// Listen 시작
	return SOCKET_ERROR != ::listen(socket, backlog);
}

void SocketUtils::Close(SOCKET& socket)
{// 소켓 자원 반환
	if (socket != INVALID_SOCKET)
		::closesocket(socket);
	socket = INVALID_SOCKET;
}