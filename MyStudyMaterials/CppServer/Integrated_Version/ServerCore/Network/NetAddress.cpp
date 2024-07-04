#include "../Network/NetAddress.h"
// #include <ws2tcpip.h>
// #define WIN32_LEAN_AND_MEAN
// #define NOMINMAX
// #include <windows.h>
#include <string>
#include <cstring>



namespace ServerCore
{


NetAddress::NetAddress(SOCKADDR_IN _sockAddr) : sockAddr(_sockAddr)
{
}

NetAddress::NetAddress(std::wstring _ip, uint16 _port)
{
    ::memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr = Ip2Address(_ip.c_str());
    sockAddr.sin_port = ::htons(_port);
}

SOCKADDR_IN& NetAddress::GetSockAddr()
{
    return sockAddr;
}

std::wstring NetAddress::GetIpAddress()
{
    WCHAR buffer[100];
    ::InetNtopW(AF_INET, &sockAddr.sin_addr, buffer, len32(buffer));
    return std::wstring(buffer);
}

uint16 NetAddress::GetPort()
{
    return ::ntohs(sockAddr.sin_port);
}

IN_ADDR NetAddress::Ip2Address(const WCHAR* _ip)
{
    IN_ADDR address;
    ::InetPtonW(AF_INET, _ip, &address);
    return address;
}
}