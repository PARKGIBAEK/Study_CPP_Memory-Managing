#include "pch.h"
#include <iostream>

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << cause << " ErrorCode : " << errCode << endl;
}

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
		return 0;

	// Non-Blocking Socket으로 변경하기
	u_long on = 1;
	if (::ioctlsocket(clientSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);
	
	// Connect
	while (true)
	{
		if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		{// 논블로킹 소켓이므로 connect시 서버가 열릴 때까지 대기하지 않기 때문에 connect결과를 즉시 반환하므로 SOCKET_ERROR를 반환할 수도 있다. 이 경우 추가로 에러코드를 확인하여야한다.
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;// 논블로킹 방식때문에 발생한 에러라면 재시도한다
			// 이미 연결된 상태라면 break
			if (::WSAGetLastError() == WSAEISCONN)
				break;
			
			break;// 진짜 에러 발생
		}
	}

	cout << "Connected to Server!" << endl;

	char sendBuffer[100] = "Hello World";

	// Send
	while (true)
	{
		if (::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0) == SOCKET_ERROR)
		{// 논블로킹 소켓이므로 send시 송신 버퍼에 여유공간이 없는 경우 여유가 생길때 까지 기다리지 않고 결과를 즉시 반환하므로 SOCKET_ERROR를 반환할 수도 있다. 이 경우 추가로 에러코드를 확인하여야한다.
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;// 논블로킹 방식때문에 발생한 에러라면 재시도한다
			
			break;// 진짜 에러 발생
		}

		cout << "Send Data ! Len = " << sizeof(sendBuffer) << endl;

		// Recv
		while (true)
		{
			char recvBuffer[1000];
			int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			if (recvLen == SOCKET_ERROR)
			{// 논블로킹 소켓이므로 recv시 데이터가 수신될 때 까지 기다리지 않고 결과를 즉시 반환하므로 SOCKET_ERROR를 반환할 수도 있다. 이 경우 추가로 에러코드를 확인하여야한다.
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
			break;
		}

		this_thread::sleep_for(1s);
	}

	// 소켓 리소스 반환
	::closesocket(clientSocket);

	// 윈속 종료
	::WSACleanup();
}