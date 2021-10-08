#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << cause << " ErrorCode : " << errCode << endl;
}
void KEEP_ALIVE(SOCKET& sock) {

	/*
		소켓에 SO_KEEPALIVE옵션을 주게되면 주기적으로 상대와의 연결이 끊어졌는지를 확인하게된다.
	*/
	bool enable = true;
	setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (const char*)&enable, sizeof(enable));
}

void SET_LINGER(SOCKET& sock) {
	/*
		소켓에 SO_LINGER 옵션을 주게되면 closesocket 함수 호출시 소켓이 즉시 닫히지 않고 전달한 linger객체에 지정된 시간만큼 지연된 후에 닫힌다.*/
	linger ling;
	ling.l_onoff = 1;
	ling.l_linger = 5;

	setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));
}

void GetSetBuffeSize(SOCKET& sock) {
	//SO_SNDBUF 송신 버퍼 크기
	//SO_RCVBUF 수신 버퍼 크기
	int sendBufferSize;
	int optionLen = sizeof(sendBufferSize);
	if (getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&sendBufferSize, &optionLen) == SOCKET_ERROR) {
		// 에러 처리
	}
	std::cout << "송신 버퍼 크기 : " << sendBufferSize << std::endl;

	sendBufferSize *= 2;// 송신 버퍼 2배로 늘리기

	if (setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (const char*)sendBufferSize, optionLen) == SOCKET_ERROR)
	{
		// 에러 처리
	}

	int recvBufferSize;
	optionLen = sizeof(recvBufferSize);
	if (getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&recvBufferSize, &optionLen) == SOCKET_ERROR) {
		// 에러 처리
	}
	std::cout << "수신 버퍼 크기 : " << recvBufferSize << std::endl;
	recvBufferSize *= 2;// 수신 버퍼 2배로 늘리기

	if (setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const char*)recvBufferSize, optionLen) == SOCKET_ERROR)
	{
		// 에러 처리
	}

}

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	// 블로킹(Blocking) 소켓일 때 각 함수가 성공적으로 반환하는 경우
	// accept -> 접속한 클라가 있을 때
	// connect -> 서버 접속 성공했을 때
	// send, sendto -> 요청한 데이터를 송신 버퍼에 복사했을 때
	// recv, recvfrom -> 수신 버퍼에 도착한 데이터가 있고, 이를 유저레벨 버퍼에 복사했을 때

	// 논블로킹(Non-Blocking)

	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
		return 0;

	// Non-Blocking Socket으로 변경하기
	u_long on = 1;
	if (::ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(7777);

	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		return 0;

	// listen은 블로킹 소켓과 논블로킹 소켓간의 별다른 차이가 없음
	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
		return 0;

	cout << "Accept" << endl;

	SOCKADDR_IN clientAddr;
	int32 addrLen = sizeof(clientAddr);

	// Accept를 Non-Blocking 소켓으로 수행할 경우
	while (true)
	{
		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		/* 논블로킹 소켓이므로 accept에서 클라의 접속 요청이 있을 때 까지 대기하지 않기 때문에 accept 결과가 INVALID_SOCKET 일 수도 있다.이 경우 추가로 에러코드를 확인하여야한다.
		 만약 추가로 확인된 에러코드 WSAEWOULDBLOCK이라면 접속 요청한 클라가 없는 상태로 accept함수가 반환된 경우를 의미한다.
		 따라서 accept를 재수행하면 된다.*/
		if (clientSocket == INVALID_SOCKET)
		{
			// 논블로킹 소켓 사용 시 
			
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			// Error
			break;
		}

		cout << "Client Connected!" << endl;
		  
		// Recv
		while (true)
		{
			char recvBuffer[1000];
			int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			
			if (recvLen == SOCKET_ERROR)
			{/* 논블록이기 때문에 recv함수가 데이터를 수신할 때까지 대기하는 것이 아니므로 SOCKET_ERROR가 발생하더라도 진짜 에러가 아닐수 있다.
			따라서 한번 더 자세한 에러코드를 확인해주어야 한다.
			만약 WSAEWOULDBLOCK이라면 수신된 데이터가 없는 상태로 recv함수가 반환된 경우이므로 다시 recv를 호출하는 과정을 진행하면된다.*/
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
					continue;// 논블로킹 방식때문에 발생한 에러라면 재시도한다

				break;// 진짜 에러 발생
			}
			else if (recvLen == 0)
			{
				// 연결 끊김
				break;
			}

			cout << "Recv Data Len = " << recvLen << endl;

			// Send
			while (true)
			{
				if (::send(clientSocket, recvBuffer, recvLen, 0) == SOCKET_ERROR)
				{// 논블로킹 소켓이므로 send시 송신 버퍼에 여유공간이 없는 경우 여유가 생길때 까지 기다리지 않고 결과를 즉시 반환하므로 SOCKET_ERROR를 반환할 수도 있다. 이 경우 추가로 에러코드를 확인하여야한다.
					if (::WSAGetLastError() == WSAEWOULDBLOCK)
						continue;// 논블로킹 방식때문에 발생한 에러라면 재시도한다
					
					break;// 진짜 에러 발생
				}

				cout << "Send Data ! Len = " << recvLen << endl;
				break;
			}
		}
		shutdown(clientSocket, SD_BOTH);
		closesocket(clientSocket);
	}


	// 윈속 종료
	::WSACleanup();
}