#pragma once
#include "Types.h"
/*-------------------
    StompAllocator
-------------------*/
/*
    ※ Stomp Allocator 사용 목적
    : 메모리 오염이 발생할 경우 메모리 접근 위반으로 인한 크래쉬를 발생하게하여 버그를 찾아내는데 용이하다.

 */
class StompAllocator
{
    enum { PAGE_SIZE = 0x1000 };

public:
    // PAGE_SIZE만큼의 공간을 최소 단위로 할당한다
    static void*	AllocateMemory(int32 size);

    static void		ReleaseMemory(void* ptr);
};