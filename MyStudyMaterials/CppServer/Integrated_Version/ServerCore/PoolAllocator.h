#pragma once
#include "Types.h"

/*-------------------
    PoolAllocator
-------------------*/

class PoolAllocator
{
public:
    static void*	AllocateMemory(int32 size);
    static void		ReleaseMemory(void* ptr);
};
