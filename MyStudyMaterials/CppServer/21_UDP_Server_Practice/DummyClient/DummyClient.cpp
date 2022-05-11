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
	printf("========================     Start Client     =========================\n\n");
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;
	//UDP용 클라 소켓 생성
	SOCKET clientSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);

	/* 
		※ Unconnected UDP
	
		- 원래 UDP는 연결이 없는 Unconnected UDP이다.
		sendto와 recvfrom메서드를 사용해여 송/수신이 필요 할 때 특정 주소를 직접 입력하는 송/수신하는 방식이다.

		※ Connected UDP
	
		- 하지만 UDP도 TCP방식 처럼 소켓에 미리 주소를 등록하여 사용하는 방식도 있는데, 그것이 바로 Connected UDP이다.
		 Connected UDP는 TCP처럼 connect메서드를 사용하여 소켓에 상대방의 주소를 바인딩하게되는데, 이 connect 메서드를 호출하는 행위가 TCP의 특성인 연결을 형성하는 것을 의미하지는 않는다는 점에 주의해야한다.
		 connect 메서드를 호출하는 행위는 단지 소켓에 주소를 미리 부여하여 사용할 수 있도록하는 행위일 뿐이다.
		 그리고 소켓에 주소를 부여하였으므로 sendto와 recvfrom메서드를 사용하지 않고, TCP와 같은 send와 recv메서드를 사용하게된다.

		 이때 send/recv메서드와 sendto/recvfrom메서드의 차이점은 connect메서드에서 입력한 상대방 주소를 직접 입력하느냐 마느냐의 차이다.
	*/
	::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

	// ---------------------------

	while (true)
	{
		char sendBuffer[100] = "Hello World!";

		// 나의 IP 주소 + 포트 번호 설정

		// Unconnected UDP 코드
		/*int32 resultCode = ::sendto(clientSocket, sendBuffer, sizeof(sendBuffer), 0,
			(SOCKADDR*)&serverAddr, sizeof(serverAddr));*/

		// Connected UDP
		int32 resultCode = ::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);

		if (resultCode == SOCKET_ERROR)
		{
			HandleError("SendTo");
			return 0;
		}

		cout << "Send Data! Len = " << sizeof(sendBuffer) << endl;

		SOCKADDR_IN recvAddr;
		::memset(&recvAddr, 0, sizeof(recvAddr));
		int32 addrLen = sizeof(recvAddr);

		char recvBuffer[1000];

		// Unconnected UDP
		//int32 recvLen = ::recvfrom(clientSocket, recvBuffer, sizeof(recvBuffer), 0,
		//	(SOCKADDR*)&recvAddr, &addrLen);
		
		int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);

		if (recvLen <= 0)
		{
			HandleError("RecvFrom");
			return 0;
		}

		cout << "Recv Data! Data = " << recvBuffer << endl;
		cout << "Recv Data! Len = " << recvLen << endl;

		this_thread::sleep_for(1s);
	}

	// ---------------------------

	// 소켓 리소스 반환
	::closesocket(clientSocket);

	// 윈속 종료
	::WSACleanup();
}