#pragma once
#include "../Core/Types.h"



namespace ServerCore
{
class PoolAllocator
{
public:
    static void*	AllocateMemory(int32 size);
    static void		ReleaseMemory(void* ptr);
};
}