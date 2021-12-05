#include "pch.h"
#include "MemoryPool.h"

/*-----------------
	MemoryPool
------------------*/

MemoryPool::MemoryPool(int32 allocSize) : _allocSize(allocSize)
{
	::InitializeSListHead(&_header);// header �ʱ�ȭ
}

MemoryPool::~MemoryPool()
{
	/*
	MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header));
	while (memory!=nullptr)
		::_aligned_free(memory);
	*/
	while (MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header)))
		::_aligned_free(memory);
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	ptr->allocSize = 0;

	::InterlockedPushEntrySList(&_header, static_cast<PSLIST_ENTRY>(ptr));

	_allocCount.fetch_sub(1);
}

MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header));

	// ������ ���� �����
	if (memory == nullptr)
	{
		//_aligned_malloc�� �޸� �Ҵ��� ���ϴ� ������ ������ �����Ͽ� �Ҵ��ϴ� ������� api
		memory = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(_allocSize, SLIST_ALIGNMENT_SIZE));
	}
	else
	{
		ASSERT_CRASH(memory->allocSize == 0);
	}

	_allocCount.fetch_add(1);

	return memory;
}
