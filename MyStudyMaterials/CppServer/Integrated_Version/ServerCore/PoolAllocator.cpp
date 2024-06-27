#include "PoolAllocator.h"

#include "CoreGlobal.h"
#include "MemoryManager.h"

/*-------------------
    PoolAllocator
-------------------*/

void* PoolAllocator::AllocateMemory(int32 size)
{
    return GMemoryManager->Allocate(size);
}

void PoolAllocator::ReleaseMemory(void* ptr)
{
    GMemoryManager->Release(ptr);
}