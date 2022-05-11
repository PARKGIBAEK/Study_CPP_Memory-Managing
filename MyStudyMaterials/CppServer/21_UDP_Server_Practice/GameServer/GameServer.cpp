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

int main()
{
	std::cout << "===========================                 Start Server                 ===========================\n\n";
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET serverSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
	//UDP는 listeningSocket이 필요 없음
	if (serverSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(7777);

	if (::bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		HandleError("Bind");
		return 0;
	}

	while (true)
	{
		SOCKADDR_IN clientAddr;
		::memset(&clientAddr, 0, sizeof(clientAddr));
		int32 addrLen = sizeof(clientAddr);
		this_thread::sleep_for(1s);

		/*
		* UDP통신에서는 TCP통신에서 처럼 accept하여 클라용 소켓을 생성해주는 과정이 없다.

		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen); // TCP에서 accept로 connection established
		if (clientSocket == INVALID_SOCKET)
		{
			int32 errCode = ::WSAGetLastError();
			cout << "Accept ErrorCode : " << errCode << endl;
			return 0;
		}

		// 손님 입장!
		char ipAddress[16];
		::inet_ntop(AF_INET, &clientAddr.sin_addr, ipAddress, sizeof(ipAddress));
		cout << "Client Connected! IP = " << ipAddress << endl;
		*/

		/* UDP는 accept를 통하여 연결을 맺지 않으므로 클라 소켓을 생성할 필요가 없고,
			recvfrom메서드 내부에서 TCP프로토콜의 송신측의 주소를 읽어 clientAddr에 저장하기 때문에
			송신 시 sendto메서드를 통해 clientAddr 주소로 데이터를 바로 전달하면된다*/
		char recvBuffer[1000];
		int32 recvLen = ::recvfrom(serverSocket, recvBuffer, sizeof(recvBuffer), 0,
			(SOCKADDR*)&clientAddr, &addrLen);

		if (recvLen <= 0)
		{
			HandleError("RecvFrom");
			return 0;
		}

		cout << "Recv from client : " << recvBuffer << endl;
		cout << "Recv Data! Len = " << recvLen << endl;
		char strBuf[16];
		cout << "Client Address : " << inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, strBuf, sizeof(strBuf)) << std::endl;

		int32 errorCode = ::sendto(serverSocket, recvBuffer, recvLen, 0,
			(SOCKADDR*)&clientAddr, sizeof(clientAddr));

		if (errorCode == SOCKET_ERROR)
		{
			HandleError("SendTo");
			return 0;
		}

		cout << "Send Data! Len = " << recvLen << endl;
	}

	// 윈속 종료
	::WSACleanup();
}