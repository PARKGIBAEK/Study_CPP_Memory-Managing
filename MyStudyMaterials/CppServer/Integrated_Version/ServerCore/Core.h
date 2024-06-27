// Core.h
#pragma once
// #include <winsock2.h>
// #include <ws2tcpip.h>
// // #include <mswsock.h>
// #define WIN32_LEAN_AND_MEAN
// #define NOMINMAX
// #include <windows.h>
#pragma comment(lib, "ws2_32.lib")

/* 주의 사항
* #include 할 때 <ws2tcpip.h>와 <mswsock.h> 중 하나만 사용할 것
* 가능하면 mswsock.h는 사용하지 말 것*/