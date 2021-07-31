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

const int32 BUFSIZE = 1000;

struct Session
{
	SOCKET socket = INVALID_SOCKET;
	char recvBuffer[BUFSIZE] = {};
	int32 recvBytes = 0;
	WSAOVERLAPPED overlapped = {};
};

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
		return 0;
	//비동기 소켓 생성
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

	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
		return 0;

	cout << "Accept" << endl;

	/*Overlapped IO(비동기 + 논블로킹) 소개
	- Overlapped 함수를 건다 (WSARecv, WSASend)
	- Overlapped 함수가 성공했는지 확인 후
	-> 성공했으면 결과 얻어서 처리
	-> 실패했으면 사유를 확인

	간단한 동작 순서

	1) 비동기 입출력 소켓
	2) WSABUF 배열의 시작 주소 + 개수 // Scatter-Gather
	3) 보내고/받은 바이트 수
	4) 상세 옵션인데 0
	5) WSAOVERLAPPED 구조체 주소값
	6) 입출력이 완료되면 OS가 호출할 콜백 함수
	WSASend
	WSARecv
	*/

	/* ※ Overlapped 모델(이벤트 기반) 흐름
	1) 비동기 입출력 지원하는 소켓 생성 + 통지 받기 위한 이벤트 객체 생성
	2) 비동기 입출력 함수 호출 (1에서 만든 이벤트 객체를 같이 넘겨줌)
	3) 비동기 작업이 바로 완료되지 않으면, WSA_IO_PENDING 오류 코드
	운영체제는 이벤트 객체를 signaled 상태로 만들어서 완료 상태 알려줌
	4) WSAWaitForMultipleEvents 함수 호출해서 이벤트 객체의 signal 판별
	5) WSAGetOverlappedResult 호출해서 비동기 입출력 결과 확인 및 데이터 처리*/

	/* WSAGetOverlappedResult 함수
	 1) 비동기 소켓
	 2) 넘겨준 overlapped 구조체
	 3) 전송된 바이트 수
	 4) 비동기 입출력 작업이 끝날때까지 대기할지?	 false
	 5) 비동기 입출력 작업 관련 부가 정보. 거의 사용 안 함.*/


	while (true)
	{
		//클라이언트 접속 받기
		SOCKADDR_IN clientAddr;
		int32 addrLen = sizeof(clientAddr);
		SOCKET clientSocket;
		while (true)
		{
			clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
			// accpet함수가 성공하지 않은 상황
			if (clientSocket != INVALID_SOCKET)
				break;
			// 알고보니 아직 연결 요청한 client가 없는 상황
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			// 그것도 아니라면 문제가 발생한 상황
			return 0;
		}

		Session session = Session{ clientSocket };
		WSAEVENT wsaEvent = ::WSACreateEvent();
		// 접속받은 클라와 연동할 이벤트 객체를 넘겨주기 ( WSAEventSelect함수를 호출한것과 비슷한 효과 )
		session.overlapped.hEvent = wsaEvent;

		cout << "Client Connected !" << endl;

		while (true)
		{
			WSABUF wsaBuf;
			// recvBuffer는 WSARecv를 실행한 시점에 즉시 커널쪽 버퍼로 복사되지 않을 수도 있으므로 WSARecv가  완료되었다는 통보를 받을 때 까지는 손대면 안된다.
			wsaBuf.buf = session.recvBuffer;
			wsaBuf.len = BUFSIZE;

			DWORD recvLen = 0;
			DWORD flags = 0;
			/* 비동기 방식의 Recv를 진행하므로 즉시 성공할 수도 있고, 아닐 수도 있으므로 SOCKET_ERROR가 반드시 진짜 에러는 아닐 수도 있다.*/
			if (::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &session.overlapped, nullptr) == SOCKET_ERROR)
			{
				if (::WSAGetLastError() == WSA_IO_PENDING)
				{
					std::cout << "WSARecv has been pending\n";
					// Pending된 상태이므로 직접 이벤트 객체 모니터링
					::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
					::WSAGetOverlappedResult(session.socket, &session.overlapped, &recvLen, FALSE, &flags);
				}
				else//진짜 에러가 발생한 상황
				{
					// TODO : 에러/예외 처리
					break;
				}
			}

			cout << "Data Recv Len = " << recvLen << endl;
		}

		::closesocket(session.socket);
		::WSACloseEvent(wsaEvent);
	}

	// 윈속 종료
	::WSACleanup();
}