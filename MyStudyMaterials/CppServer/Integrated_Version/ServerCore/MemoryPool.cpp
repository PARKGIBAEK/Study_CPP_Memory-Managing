#include "pch.h"
#include "MemoryPool.h"

/*-----------------
	MemoryPool
------------------*/

MemoryPool::MemoryPool(int32 allocSize) : allocSize(allocSize)
{
	::InitializeSListHead(&header); // SLIST_HEADER�� ���̳ʸ��� ��� 0���� �ʱ�ȭ
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
	/* InterlockedPopEntrySList �Լ���
	_header�� ���� �ֱٿ� ���Ե� �����͸� ��ȯ,
	���� �ƹ��� �����͵� ������� ���� ��� nullptr ��ȯ*/
	MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&header));

	// ������ ���� �Ҵ�
	if (memory == nullptr)
	{
		// _aligned_malloc�� CPU ��Ű���Ŀ� ����ȭ�� ������ ���� �޸� �ּҸ� 16�� ����� �����ش�(Ư�� MS���� �����ϴ� SLIST�� ����Ϸ��� �޸� ������ 16�� ����� �����־���Ѵ�)
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