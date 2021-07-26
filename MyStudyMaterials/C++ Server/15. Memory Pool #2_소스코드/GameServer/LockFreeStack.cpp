#include "pch.h"
#include "LockFreeStack.h"

/*-------------------
	1�� �õ�
-------------------*/

/*

void InitializeHead(SListHeader* header)
{
	header->next = nullptr;
}

void PushEntrySList(SListHeader* header, SListEntry* entry)
{
	entry->next = header->next;
	header->next = entry;
}

SListEntry* PopEntrySList(SListHeader* header)
{
	SListEntry* first = header->next;

	if (first != nullptr)
		header->next = first->next;

	return first;
}
*/


//=========================================================


/*-------------------
		2�� �õ�
--------------------/*

/*
void InitializeHead(SListHeader* header)
{
	header->next = nullptr;
}

void PushEntrySList(SListHeader* header, SListEntry* entry)
{
	entry->next = header->next;

	while (::InterlockedCompareExchange64((int64*)&header->next, (int64)entry, (int64)entry->next) != entry)
	{

	}
}

// [][]
// Header[ next ]
SListEntry* PopEntrySList(SListHeader* header)
{
	//�ֻ�� �����͸� ������
	SListEntry* expected = header->next;

	// ABA Problem
	while (expected && ::InterlockedCompareExchange64((int64*)&header->next, (int64)expected->next, (int64)expected) != expected)
	{

	}

	return expected;
}
*/


//=========================================================


/*-------------------
		3�� �õ�
-------------------*/

void InitializeHead(SListHeader* header)
{
	header->alignment = 0;
	header->region = 0;
}

void PushEntrySList(SListHeader* header, SListEntry* entry)
{
	
	SListHeader expected{};
	SListHeader desired{};

	/* 64��Ʈ ��忡���� ������ ������ entry�� ����Ű�� �޸� �ּ��� ������ �ڸ��� 0�̴�. ��, 16�� ����̴�. 
	* 	  ��) 0x000001B57D6F2540   16�����Ǵ� ���ڸ��� 4��Ʈ�� �ش��Ѵ�.
	
	���� 16�� ����� �� �ּ��� ����4��Ʈ�� 0�� �ǹǷ� bit shift >> 4�� �ؼ� ����ص� ������ ����.
	
	* MS���� ������ SList�� ����Ѵٸ� �ݵ�� 16����Ʈ ������ ������Ѵ�
	- by Microsoft -
	*/
	desired.HeaderX64.next = (((uint64)entry) >> 4);

	while (true)
	{
		expected = *header;

		// �� ���̿� header�� ����� ���� �����Ƿ� ���� �ʿ�

		//������ bit shift>>4 �ߴ� ���� �����ϴ� ����
		entry->next = (SListEntry*)(((uint64)expected.HeaderX64.next) << 4);

		desired.HeaderX64.depth = expected.HeaderX64.depth + 1;
		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;

		if (::InterlockedCompareExchange128((int64*)header, desired.region, desired.alignment, (int64*)&expected) == 1)
			break;
	}
}

SListEntry* PopEntrySList(SListHeader* header)
{
	SListHeader expected = {};
	SListHeader desired = {};
	SListEntry* entry = nullptr;

	while (true)
	{
		expected = *header;

		entry = (SListEntry*)(((uint64)expected.HeaderX64.next) << 4);//
		if (entry == nullptr)
			break;

		/* Pop�� �����ϴ� �����尡 �� �̻��̶��
		entry�� �����Ϸ��� ���� �ٸ� �����忡�� entry�� ����Ű�� �޸𸮸� �������� �� ������,
		�׷� ��쿡�� Use-After-Free ���� �߻� ����*/
		desired.HeaderX64.next = ((uint64)entry->next) >> 4;
		desired.HeaderX64.depth = expected.HeaderX64.depth - 1;//������ depth�� �پ��ٴ� ���� ǥ��
		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;// ���° push/pop �۾����� ǥ��

		if (::InterlockedCompareExchange128((int64*)header, desired.region, desired.alignment, (int64*)&expected) == 1)
			break;
	}

	return entry;
}