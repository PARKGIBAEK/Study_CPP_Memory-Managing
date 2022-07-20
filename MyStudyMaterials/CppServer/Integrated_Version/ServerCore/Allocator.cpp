#include "pch.h"
#include "Allocator.h"
//#include "Memory.h"

/*-------------------
	BaseAllocator
-------------------*/

void* BaseAllocator::AllocateMemory(int32 size)
{
	return ::malloc(size);
}

void BaseAllocator::ReleaseMemory(void* ptr)
{
	::free(ptr);
}

/*-------------------
	StompAllocator
-------------------*/

void* StompAllocator::AllocateMemory(int32 size)
{
	//�޸� �Ҵ� ������ PAGE_SIZE�� �ϱ� ���� ����
	const int64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	/* �Ҵ��� �޸� �� ������ �޸��� �ּҸ� ��������
		size��ŭ ������ ��� ��ġ�� ��ȯ�ϱ� ����
		
		����) 4096 byte �Ҵ� �޾Ұ�, �޸��� �������� 0x0000'0000�̶�� �������� ��
		    ����� �޸� ũ�Ⱑ 16�̶��
			 ���� �ּ�(0000) + �Ҵ���� �޸� ũ��(4096) - ����� �޸� ũ��(16) = Offset(4080)
			 0x0000'0000 + 0x0000'1000 - 0x0000'0010 = 0x0000'0FF0 �� �ȴ�.

	BaseAddress(0000)                      Offset(4080)
			��                                  ��
			[            Allocated memory             ]
															 ��
														  �޸� �� �ּ�(4096)
	*/
	const int64 offset = pageCount * PAGE_SIZE - size;
	
	// pageCount x PAGE_SIZE��ŭ �Ҵ��ϰ� �Ҵ� �ּ��� �������� �޾ƿ�
	/* VirtualAlloc �ɼ�
	- MEM_RESERVE : �޸� �Ҵ� ����(����¡ ���Ͽ� ���� ������ ��������� �Ҵ����� �ʰ�, ���μ����� ���� �ּ� ���� ������ ����)
	 
	- MEM_COMMIT : ����� �޸� �������� MemoryManager Charge(�Ҵ��� �޸��� ũ��� ����¡ ������ ũ��)�� �Ҵ�.
					��, ���� �ּҿ� ������ ���������� �ʴ� �� ���� ������ �������� �Ҵ���� �ʴ´�.
	
	�� �������� ����� ���ÿ� �Ҵ��Ϸ��� ?
	  MEM_COMMIT | MEM_RESERVE �ɼ��� ����ϸ� �ȴ�.
	  ��, �Ҵ�� ���ÿ� ����� ��츦 ���Ѵ�.
	  ( �޸� ������ 0���� �ʱ�ȭ ���� �����Ѵ� )

	- PAGE_READWRITE : �б�/���� ���� ���, ������ ������� �ɼ��� Ȱ��ȭ �� ��� Ŀ�Ե� �������� �ڵ带 ������ ��� �׼��� ���� �߻�*/
	void* baseAddress = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	/* VirtualAlloc �Լ��� ���� �Ҵ���� �޸��� �ּҿ��� ���� ����� �ּ��� ��ġ�� ��ȯ 	*/
	return static_cast<void*>(static_cast<int8*>(baseAddress) + offset);
}

void StompAllocator::ReleaseMemory(void* ptr)
{
	// AllocateMemory �Լ����� �Ҵ��� ��ü �޸� �� ���� ����� �������� ��ȯ���� �ּ� ( baseAddress + offset )
	const int64 address = reinterpret_cast<int64>(ptr);

	// ���� �ּҿ��� offset�� �� �ּ� ( baseAddress + offset ) - offset
	// ex) base : 5000, offset : 500, address : 5500
	//		 5500 - ( 5500 % 4096 ) = 5500 - 1404  = 4096

	const int64 baseAddress = address - (address % PAGE_SIZE);
	// MEM_RELEASE�ɼ��� ����� ��� VirtualAlloc�Լ��� ���� �Ҵ��� �޸��� base address�� ���ڷ� �־��־���Ѵ�
	::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);
}

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