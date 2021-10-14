#include "pch.h"
#include "NetAddress.h"

/*--------------
	NetAddress
---------------*/

NetAddress::NetAddress(SOCKADDR_IN _sockAddr) : sockAddr(_sockAddr)
{
}

NetAddress::NetAddress(wstring _ip, uint16 _port)
{
	::memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr = Ip2Address(_ip.c_str());
	sockAddr.sin_port = ::htons(_port);
}

wstring NetAddress::GetIpAddress()
{
	WCHAR buffer[100];
	::InetNtopW(AF_INET, &sockAddr.sin_addr, buffer, len32(buffer));
	return wstring(buffer);
}

IN_ADDR NetAddress::Ip2Address(const WCHAR* _ip)
{
	IN_ADDR address;
	::InetPtonW(AF_INET, _ip, &address);
	return address;
}
