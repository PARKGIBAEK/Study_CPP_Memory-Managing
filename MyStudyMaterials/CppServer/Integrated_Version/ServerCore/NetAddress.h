#pragma once

/*--------------
	NetAddress
---------------*/

class NetAddress
{
public:
	NetAddress() = default;
	NetAddress(SOCKADDR_IN _sockAddr);
	NetAddress(wstring _ip, uint16 _port);

	SOCKADDR_IN&	GetSockAddr() { return sockAddr; }
	wstring			GetIpAddress();
	uint16			GetPort() { return ::ntohs(sockAddr.sin_port); }

public:
	static IN_ADDR	Ip2Address(const WCHAR* ip);

private:
	SOCKADDR_IN		sockAddr = {};
};

