#include "pch.h"
#include "MemoryPool.h"

/*-----------------
	MemoryPool
------------------*/

MemoryPool::MemoryPool(int32 allocSize) : allocSize(allocSize)
{
	::InitializeSListHead(&header); // SLIST_HEADER의 바이너리를 모두 0으로 초기화
}

MemoryPool::~MemoryPool()
{
	while (MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&header)))
		::_aligned_free(memory);
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	ptr->allocSize = 0;

	::InterlockedPushEntrySList(&header, static_cast<PSLIST_ENTRY>(ptr));

	usedCount.fetch_sub(1);
	reservedCount.fetch_add(1);
}

MemoryHeader* MemoryPool::Pop()
{
	/* InterlockedPopEntrySList 함수는
	_header에 가장 최근에 삽입된 데이터를 반환,
	만약 아무런 데이터도 들어있지 않을 경우 nullptr 반환*/
	MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&header));

	// 없으면 새로 할당
	if (memory == nullptr)
	{
		// _aligned_malloc은 CPU 아키텍쳐에 최적화된 동작을 위해 메모리 주소를 16의 배수로 맞춰준다(특히 MS에서 제공하는 SLIST를 사용하려면 메모리 정렬을 16의 배수로 맞춰주어야한다)
		memory = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(allocSize, SLIST_ALIGNMENT));
	}
	else
	{
		ASSERT_CRASH(memory->allocSize == 0);
		reservedCount.fetch_sub(1);
	}

	usedCount.fetch_add(1);

	return memory;
}