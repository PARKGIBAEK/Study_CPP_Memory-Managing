#pragma once
#include "../Core/Types.h"

namespace ServerCore
{
class BaseAllocator
{
public:
    // malloc 래핑
    static void*	AllocateMemory(int32 size);
    // free 래핑
    static void		ReleaseMemory(void* ptr);
};
}