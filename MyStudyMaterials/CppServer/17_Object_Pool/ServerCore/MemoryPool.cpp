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
	// ����� ��� ��带 ���� ����
	while (MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header)))
		::_aligned_free(memory);
}

MemoryHeader* MemoryPool::Pop()
{
	/* InterlockedPopEntrySList �Լ���
	_header�� ���� �ֱٿ� ���Ե� �����͸� ��ȯ,
	���� �ƹ��� �����͵� ������� ���� ��� nullptr ��ȯ*/
	MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header));
	
	// �޸� Ǯ���� ������ ������ �޸𸮰� ���ٸ� ���� �����(�Ҵ�)
	if (memory == nullptr)
	{//�޸� �Ҵ� �� ���� aligned_size�� �����Ͽ� ��ȯ
		memory = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(allocSize, SLIST_ALIGNMENT));
		// _aligned_malloc�� �޸� �Ҵ��� ���ϴ� ������ ������ �����Ͽ� �Ҵ��ϴ� Windows API
	}
	else
	{
		ASSERT_CRASH(memory->allocSize == 0);//allocSize�� 0�̶�� ���� ������ ���� �����̹Ƿ� �����߻�
		_reserveCount.fetch_sub(1);
	}

	_useCount.fetch_add(1);

	return memory;
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	ptr->allocSize = 0;

	// ����� ���� �޸𸮸� �ٽ� Pool�� �ݳ��Ѵ�
	::InterlockedPushEntrySList(&_header, static_cast<PSLIST_ENTRY>(ptr));

	_useCount.fetch_sub(1);
	_reserveCount.fetch_add(1);
}
