#include "MemoryPool.h"
#include "../Memory/MemoryHeader.h"
#include "../Core/CoreMacro.h"

// #define WIN32_LEAN_AND_MEAN
// #define NOMINMAX
// #include <winsock2.h>
// #include <mswsock.h>
// #include <windows.h>
// #include <ws2tcpip.h>

namespace ServerCore
{
MemoryPool::MemoryPool(int32 _allocSize) : allocSize(_allocSize)
{
	::InitializeSListHead(&header); // SLIST_HEADER�� ���̳ʸ��� ��� 0���� �ʱ�ȭ

	/*
	// �̸� ����� ����
	int32 initialCount = 4096 / _allocSize;
	for (size_t i = 0; i < initialCount; i++)
	{
		MemoryHeader* memory = reinterpret_cast<MemoryHeader*>(
			::_aligned_malloc(allocSize, SLIST_ALIGNMENT));
		::InterlockedPushEntrySList(&header, memory);
		reservedCount.fetch_add(1);
	}
	*/
}

MemoryPool::~MemoryPool()
{
	while (MemoryHeader* memory =
		static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&header)))
	{
		::_aligned_free(memory);
	}
}

void MemoryPool::Push(MemoryHeader* _ptr)
{
	_ptr->allocSize = 0;

	::InterlockedPushEntrySList(&header, static_cast<PSLIST_ENTRY>(_ptr));

	usedCount.fetch_sub(1);
	reservedCount.fetch_add(1);
}

MemoryHeader* MemoryPool::Pop()
{
	/* InterlockedPopEntrySList �Լ���
	_header�� ���� �ֱٿ� ���Ե� �����͸� ��ȯ,
	���� �ƹ��� �����͵� ������� ���� ��� nullptr ��ȯ*/
	MemoryHeader* memory = static_cast<MemoryHeader*>(
		::InterlockedPopEntrySList(&header));

	// ������ ���� �Ҵ�
	if (memory == nullptr)
	{
		// _aligned_malloc�� CPU ��Ű���Ŀ� ����ȭ�� ������ ���� �޸� �ּҸ� 16�� ����� �����ش�(Ư�� MS���� �����ϴ� SLIST�� ����Ϸ��� �޸� ������ 16�� ����� �����־���Ѵ�)
		memory = static_cast<MemoryHeader*>(
			::_aligned_malloc(allocSize, static_cast<int>(ALIGNMENT::SLIST_ALIGNMENT)));
	}
	else
	{
		ASSERT_CRASH(memory->allocSize == 0);
		reservedCount.fetch_sub(1);
	}

	usedCount.fetch_add(1);

	return memory;
}
}