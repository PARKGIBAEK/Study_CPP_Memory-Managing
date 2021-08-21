#pragma once
#include "NetAddress.h"

/*----------------
	SocketUtils
-----------------*/

class SocketUtils
{
public:
	//�Լ� �����ͷ� ����� �͵�( IOCP�� �����Ͽ� ��� )
	static LPFN_CONNECTEX		ConnectEx;
	static LPFN_DISCONNECTEX	DisconnectEx;
	static LPFN_ACCEPTEX		AcceptEx;

public:
	static void Init();
	static void Clear();
	//� �Լ� �ּҸ� �޾��ִ� ������ �Լ�
	static bool BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn);
	//������ TCP������ �����ϴ� �Լ�
	static SOCKET CreateSocket();

	static bool SetLinger(SOCKET socket, uint16 onoff, uint16 linger);
	static bool SetReuseAddress(SOCKET socket, bool flag);// Setting Socket as reusable
	static bool SetRecvBufferSize(SOCKET socket, int32 size);// Setting RecvBuffer Size
	static bool SetSendBufferSize(SOCKET socket, int32 size);// Setting SendBuffer Size
	static bool SetTcpNoDelay(SOCKET socket, bool flag);// Setting Nagle Algorithm
	static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket);// Applying the option in socket same as listening socket

	static bool Bind(SOCKET socket, NetAddress netAddr);
	static bool BindAnyAddress(SOCKET socket, uint16 port);
	static bool Listen(SOCKET socket, int32 backlog = SOMAXCONN);
	static void Close(SOCKET& socket);
};

template<typename T>
static inline bool SetSockOpt(SOCKET socket, int32 level, int32 optName, T optVal)
{
	return SOCKET_ERROR != ::setsockopt(socket, level, optName, reinterpret_cast<char*>(&optVal), sizeof(T));
}