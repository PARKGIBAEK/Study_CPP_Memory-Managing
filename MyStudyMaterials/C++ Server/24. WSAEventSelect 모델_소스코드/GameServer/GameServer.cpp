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
	int32 sendBytes = 0;
};

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
		return 0;

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

	// WSAEventSelect = (WSAEventSelect 함수가 핵심이 되는)
	// 소켓과 관련된 네트워크 이벤트를 [이벤트 객체]를 통해 감지

	// 이벤트 객체 관련 함수들
	// 생성 : WSACreateEvent (수동 리셋 Manual-Reset + Non-Signaled 상태 시작)
	// 삭제 : WSACloseEvent
	// 신호 상태 감지 : WSAWaitForMultipleEvents
	// 구체적인 네트워크 이벤트 알아내기 : WSAEnumNetworkEvents

	// 소켓 <-> 이벤트 객체 연동
	// WSAEventSelect(socket, event, networkEvents);
	// - 관심있는 네트워크 이벤트
	// FD_ACCEPT : 접속한 클라가 있음 accept
	// FD_READ : 데이터 수신 가능 recv, recvfrom
	// FD_WRITE : 데이터 송신 가능 send, sendto
	// FD_CLOSE : 상대가 접속 종료
	// FD_CONNECT : 통신을 위한 연결 절차 완료
	// FD_OOB

	// 주의 사항
	// WSAEventSelect 함수를 호출하면, 해당 소켓은 자동으로 넌블로킹 모드 전환
	// accept() 함수가 리턴하는 소켓은 listenSocket과 동일한 속성을 갖는다
	// - 따라서 clientSocket은 FD_READ, FD_WRITE 등을 다시 등록 필요
	// - 드물게 WSAEWOULDBLOCK 오류가 뜰 수 있으니 예외 처리 필요
	// 중요)
	// - 이벤트 발생 시, 적절한 소켓 함수 호출해야 함
	// - 아니면 다음 번에는 동일 네트워크 이벤트가 발생 X
	// ex) FD_READ 이벤트 떴으면 recv() 호출해야 하고, 안하면 FD_READ 두 번 다시 X

	// 1) count, event
	// 2) waitAll : 모두 기다림? 하나만 완료 되어도 OK?
	// 3) timeout : 타임아웃
	// 4) 지금은 false
	// return : 완료된 첫번째 인덱스
	// WSAWaitForMultipleEvents

	// 1) socket
	// 2) eventObject : socket 과 연동된 이벤트 객체 핸들을 넘겨주면, 이벤트 객체를 non-signaled
	// 3) networkEvent : 네트워크 이벤트 / 오류 정보가 저장
	// WSAEnumNetworkEvents

	vector<WSAEVENT> wsaEvents;
	vector<Session> sessions;
	sessions.reserve(100);

	// WSACreateEvent : 이벤트 객체 생성
	WSAEVENT listenEvent = ::WSACreateEvent();
	wsaEvents.emplace_back(listenEvent);
	sessions.emplace_back(Session{ listenSocket });

	/* WSAEventSelect 함수
	-  listenSocket 과 listenEvent 객체를 연결해 줌
		FD_ACCEPT 또는 FD_CLOSE 관련 동작이 감지 될 경우 이벤트 객체에 알려준다	 */
	if (::WSAEventSelect(listenSocket, listenEvent, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR)
		return 0;

	while (true)
	{/*	WSAWaitForMultipleEvents 함수란?
			- 이벤트 객체를 모니터링하는 역할을 한다.
			cEvents : 이벤트 객체 갯수
			lphEvents : 이벤트 객체 배열
			fWaitAll : FALSE는 이벤트 객체 중 하나라도 signaled 되면 반환, TRUE는 이벤트 객체 배열 전체가 signaled 되면 반환
			WSA_INFINITE : 타임아웃 시간 제한 없음
			fAlertable : I/O Completion routines 모델을 사용할 때 입력하는 인자다.
			사용하지 않을 경우 FALSE를 입력한다.
	 */
		int32 index = ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, WSA_INFINITE, FALSE);
		if (index == WSA_WAIT_FAILED)
			continue;
		/* WSAWaitForMultipleEvents함수의 반환 값에서 WSA_WAIT_EVENT_0 을 빼면 이벤트 객체 배열에서 이벤트가 발생한 이벤트 객체의 인덱스 번호가 된다.
		 만약 둘 이상의 이벤트 객체가 감지되었다면 가장 순번이 빠른 인덱스가 되겠다.*/
		index -= WSA_WAIT_EVENT_0;

		//::WSAResetEvent(wsaEvents[index]);

		WSANETWORKEVENTS networkEvents;
		/* WSAEnumNetworkEvents 함수란?
		- WSAWaitForMultipleEvents함수에서 탐지된 이벤트 객체의 자세한 이벤트 정보를 알아내는 역할을한다.
		 s : 소켓
		 hEventObject : 자세한 이벤트 정보를 알아낼 이벤트 객체. ( 동시에 이벤트 객체를 Non-signaled 상태로 리셋해준다)
		 lpNetworkEvents : hEventObject에 전달한 이벤트 객체의 자세한 이벤트 정보를 입력할 WSANETWORKEVENTS 객체 주소
		 */
		if (::WSAEnumNetworkEvents(sessions[index].socket, wsaEvents[index], &networkEvents) == SOCKET_ERROR)
			continue;

		// Listener 소켓 체크
		if (networkEvents.lNetworkEvents & FD_ACCEPT)
		{
			// Error-Check
			if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
				continue;

			SOCKADDR_IN clientAddr;
			int32 addrLen = sizeof(clientAddr);

			SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
			if (clientSocket != INVALID_SOCKET)
			{
				cout << "Client Connected" << endl;

				WSAEVENT clientEvent = ::WSACreateEvent();
				// Event 객체 배열과 Session 배열의 인덱스를 매칭시켜 사용
				wsaEvents.emplace_back(clientEvent);
				sessions.emplace_back(Session{ clientSocket });
				//클라이언트 소켓과 이벤트 객체를 연동시키기
				if (::WSAEventSelect(clientSocket, clientEvent, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
					return 0;
			}
		}

		// Client Session 소켓 체크
		if (networkEvents.lNetworkEvents & FD_READ || networkEvents.lNetworkEvents & FD_WRITE)
		{
			// Error-Check
			if ((networkEvents.lNetworkEvents & FD_READ) /*read이벤트가 발생한 경우*/
				&& (networkEvents.iErrorCode[FD_READ_BIT] != 0)/*read이벤트가 알고보니 에러였을 경우*/)
				continue;
			// Error-Check
			if ((networkEvents.lNetworkEvents & FD_WRITE) /* write이벤트가 발생한 경우*/ 
				&& (networkEvents.iErrorCode[FD_WRITE_BIT] != 0)/*write이벤트가 알고보니 에러였을 경우*/)
				continue;

			Session& s = sessions[index];

			// Read
			if (s.recvBytes == 0)
			{
				int32 recvLen = ::recv(s.socket, s.recvBuffer, BUFSIZE, 0);
				if (recvLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK/* 에러가 WSAEWOULDBLOCK가 아닐 경우 심각한 에러가 발생한 것*/)
				{
					// TODO : Remove Session ( recv에러가 발생한 소켓과 이벤트 객체의 인덱스를 찾아서 배열에서 제거
					continue;
				}

				s.recvBytes = recvLen;
				cout << "Recv Data = " << recvLen << endl;
			}

			// Write
			if (s.recvBytes > s.sendBytes)//에코 서버 이므로 보낸 데이터가 받은 데이터보다 작은 경우는 송신할 차례를 의미
			{
				int32 sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
				if (sendLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK)
				{
					// TODO : Remove Session
					continue;
				}
				// 보낼 데이터가 100% 송신이 되지 않을 경우를 대비해서 커서로 체크하기
				s.sendBytes += sendLen;
				if (s.recvBytes == s.sendBytes)
				{
					s.recvBytes = 0;
					s.sendBytes = 0;
				}

				cout << "Send Data = " << sendLen << endl;
			}
		}

		// FD_CLOSE 처리
		if (networkEvents.lNetworkEvents & FD_CLOSE)
		{
			// TODO : Remove Socket
		}
	}

	
	// 윈속 종료
	::WSACleanup();
}