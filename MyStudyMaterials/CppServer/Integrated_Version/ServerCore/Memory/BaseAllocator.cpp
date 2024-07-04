#include "BaseAllocator.h"


namespace ServerCore
{
void* BaseAllocator::AllocateMemory(int32 size)
{
    return ::malloc(size);
}

void BaseAllocator::ReleaseMemory(void* ptr)
{
    ::free(ptr);
}
}