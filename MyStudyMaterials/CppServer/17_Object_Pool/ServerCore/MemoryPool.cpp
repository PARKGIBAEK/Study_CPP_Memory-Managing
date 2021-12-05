#include "pch.h"
#include "MemoryPool.h"

/*-----------------
	MemoryPool
------------------*/

MemoryPool::MemoryPool(int32 allocSize) : allocSize(allocSize)
{
	::InitializeSListHead(&_header);
}

MemoryPool::~MemoryPool()
{
	// 연결된 모든 노드를 전부 해제
	while (MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header)))
		::_aligned_free(memory);
}

MemoryHeader* MemoryPool::Pop()
{
	/* InterlockedPopEntrySList 함수는
	_header에 가장 최근에 삽입된 데이터를 반환,
	만약 아무런 데이터도 들어있지 않을 경우 nullptr 반환*/
	MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header));
	
	// 메모리 풀에서 꺼내줄 여분의 메모리가 없다면 새로 만든다(할당)
	if (memory == nullptr)
	{//메모리 할당 시 직접 aligned_size를 설정하여 반환
		memory = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(allocSize, SLIST_ALIGNMENT));
		// _aligned_malloc은 메모리 할당을 원하는 사이즈 단위로 정렬하여 할당하는 Windows API
	}
	else
	{
		ASSERT_CRASH(memory->allocSize == 0);//allocSize가 0이라는 것은 비정상 적인 상태이므로 오류발생
		_reserveCount.fetch_sub(1);
	}

	_useCount.fetch_add(1);

	return memory;
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	ptr->allocSize = 0;

	// 사용이 끝난 메모리를 다시 Pool에 반납한다
	::InterlockedPushEntrySList(&_header, static_cast<PSLIST_ENTRY>(ptr));

	_useCount.fetch_sub(1);
	_reserveCount.fetch_add(1);
}
