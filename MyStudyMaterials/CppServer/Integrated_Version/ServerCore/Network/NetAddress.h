#pragma once
#include "../Core/Types.h"
#include <WS2tcpip.h>


namespace ServerCore
{


class NetAddress
{
public:
	NetAddress() = default;
	NetAddress(SOCKADDR_IN _sockAddr);
	NetAddress(std::wstring _ip, uint16 _port);

	SOCKADDR_IN&	GetSockAddr();
	std::wstring			GetIpAddress();
	uint16			GetPort() ;

public:
	static IN_ADDR	Ip2Address(const WCHAR* ip);

private:
	SOCKADDR_IN		sockAddr = {};
};
}