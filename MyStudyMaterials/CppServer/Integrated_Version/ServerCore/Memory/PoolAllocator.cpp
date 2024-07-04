#include "PoolAllocator.h"

#include "../Core/CoreInitializer.h"
#include "../Memory/MemoryManager.h"



namespace ServerCore
{
void* PoolAllocator::AllocateMemory(int32 size)
{
    return GMemoryManager->Allocate(size);
}

void PoolAllocator::ReleaseMemory(void* ptr)
{
    GMemoryManager->Release(ptr);
}
}