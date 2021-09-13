#include "pch.h"
#include "MemoryPool.h"

/*-----------------
	MemoryPool
------------------*/

MemoryPool::MemoryPool(int32 allocSize) : _allocSize(allocSize)
{
	::InitializeSListHead(&_header);
}

MemoryPool::~MemoryPool()
{
	while (MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header)))
		::_aligned_free(memory);
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	ptr->allocSize = 0;

	::InterlockedPushEntrySList(&_header, static_cast<PSLIST_ENTRY>(ptr));

	_useCount.fetch_sub(1);
	_reserveCount.fetch_add(1);
}

MemoryHeader* MemoryPool::Pop()
{
	/* InterlockedPopEntrySList는
	_header에 가장 최근에 삽입된 데이터를 반환,
	만약 아무런 데이터도 들어있지 않을 경우 nullptr 반환*/
	MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header));
	
	// 없으면 새로 만든다
	if (memory == nullptr)
	{//메모리 할당 시 직접 aligned_size를 설정하여 반환
		memory = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(_allocSize, SLIST_ALIGNMENT));
	}
	else
	{
		ASSERT_CRASH(memory->allocSize == 0);//allocSize가 0이라는 것은 비정상 적인 상태이므로 오류발생
		_reserveCount.fetch_sub(1);
	}

	_useCount.fetch_add(1);

	return memory;
}
