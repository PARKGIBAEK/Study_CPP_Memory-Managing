#include "pch.h"
#include "Allocator.h"
#include "Memory.h"

/*-------------------
	BaseAllocator
-------------------*/

void* BaseAllocator::Alloc(int32 size)
{
	return ::malloc(size);
}

void BaseAllocator::Release(void* ptr)
{
	::free(ptr);
}

/*-------------------
	StompAllocator
-------------------*/

void* StompAllocator::Alloc(int32 size)
{
	//�ּ� �Ҵ� ������ PAGE_SIZE�� �ϱ� ���� ����
	const int64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	/* �Ҵ�� �޸� �� size��ŭ ������ ��� ��ġ�� ��ȯ�ϱ� ����

	base                          base + offset
	 ��                                  ��
	 [            Allocated memory             ]

	*/
	const int64 dataOffset = pageCount * PAGE_SIZE - size;

	// pageCount x PAGE_SIZE��ŭ �Ҵ��ϰ� �Ҵ� �ּ��� �������� �޾ƿ�
	/* VirtualAlloc �ɼ�
	- MEM_RESERVE : �޸� �Ҵ� ����(����¡ ���Ͽ� ���� ������ ��������� �Ҵ����� �ʰ�, ���μ����� ���� �ּ� ���� ������ ����)

	- MEM_COMMIT : ����� �޸� �������� Memory Charge(�Ҵ��� �޸��� ũ��� ����¡ ������ ũ��)�� �Ҵ�. ���� �ּҿ� ������ ���������� �ʴ� �� ���� ������ �������� �Ҵ���� �ʴ´�.

	�� �������� �������� ���� ����� ���̶��?
	  MEM_COMMIT | MEM_RESERVE �ɼ��� ����ؾ��Ѵ�.
	  ��, �Ҵ�� ���ÿ� ����� ��츦 ���Ѵ�.
	  ( �޸� ������ 0���� �ʱ�ȭ ���� �����Ѵ� )

	- PAGE_READWRITE : �б�/���� ���� ���, ������ ������� �ɼ��� Ȱ��ȭ �� ��� Ŀ�Ե� �������� �ڵ带 ������ ��� �׼��� ���� �߻�*/
	void* baseAddress = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	/* VirtualAlloc �Լ��� ���� �Ҵ���� �޸��� �ּҿ��� ���� ����� �ּ��� ��ġ�� ��ȯ 	*/
	return static_cast<void*>(static_cast<int8*>(baseAddress) + dataOffset);
}

void StompAllocator::Release(void* ptr)
{
	// AllocateMemory �Լ����� �Ҵ��� ��ü �޸� �� ���� ����� �������� ��ȯ���� �ּ� ( baseAddress + dataOffset )
	const int64 address = reinterpret_cast<int64>(ptr);
	// ���� �ּҿ��� data offset�� �� �ּ� ( baseAddress + dataOffset ) - dataOffset
	// ex) base : 5000, dataOffset : 500, address : 5500
	//		 5500 - ( 5500 % 4096 ) = 5500 - 1404  = 

	const int64 baseAddress = address - (address % PAGE_SIZE);
	// MEM_RELEASE�ɼ��� ����� ��� VirtualAlloc�Լ��� ���� �Ҵ��� �޸��� base address�� ���ڷ� �־��־���Ѵ�
	::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);
}

/*-------------------
	PoolAllocator
-------------------*/

void* PoolAllocator::Alloc(int32 size)
{
	return GMemory->Allocate(size);
}

void PoolAllocator::Release(void* ptr)
{
	GMemory->Release(ptr);
}