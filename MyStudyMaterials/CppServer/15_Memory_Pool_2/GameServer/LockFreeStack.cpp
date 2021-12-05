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

void PushEntrySList(SListHeader* header, SListEntry* newNode)
{
	newNode->next = header->next;
	header->next = newNode;
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

void PushEntrySList(SListHeader* header, SListEntry* newNode)
{
	newNode->next = header->next;

	while (::InterlockedCompareExchange64((int64*)&header->next, (int64)newNode, (int64)newNode->next) != newNode)
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

void PushEntrySList(SListHeader* header, SListEntry* newNode)
{

	SListHeader expected{};
	SListHeader desired{};

	/* 64��Ʈ ��忡���� �޸� �ּҴ� �׻� 16�� ����̴�.
		�׷��Ƿ� �޸� �ּҸ� 16������ ǥ���� ��� ������ �ڸ����� �׻� 0�̴�.
		���� �޸� �ּҸ� 2������ ǥ���Ѵٸ� ���� 4�ڸ��� �׻� 0�̴�.
		���� �� �ּҸ� �ٸ� ������ bit shift >> 4�� �����ش���
		bit shift << 4�� ���ָ� �����ȴ�

		64��Ʈ ��� �޸� �ּ� : 0x000001B57D6F2540 (16���� ���ڸ��� 4��Ʈ�� �ش�)

		MS���� ������ SList�� ����ϱ� ���� 16����Ʈ ������ �ϴ� ����.	*/
	desired.HeaderX64.next = (((uint64)newNode) >> 4); // Push�� ����� �޸� �ּ� ����

	while (true)
	{
		expected = *header;//head�� ����

		// �� ���̿� header�� ����� ���� �����Ƿ� ���� �ʿ�
		//������ bit shift>>4 �ߴ� ���� �����ϴ� ����

		// newNode->next�� ����header->next�� �ٲ��ش�(��� ���� ����)
		newNode->next = (SListEntry*)(((uint64)expected.HeaderX64.next) << 4);


		desired.HeaderX64.depth = expected.HeaderX64.depth + 1;
		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;
		
		// header�� expected�� ���� ��� header�� expected�� ��ȯ�ϰ� 1�� ��ȯ, �ٸ���  0�� ��ȯ
		if (::InterlockedCompareExchange128((int64*)header, desired.region, desired.alignment, (int64*)&expected) == 1)
			break;
	}
}

SListEntry* PopEntrySList(SListHeader* header)
{
	SListHeader expected = {};
	SListHeader desired = {};
	SListEntry* popNode = nullptr;

	while (true)
	{
		expected = *header;

		popNode = (SListEntry*)(((uint64)expected.HeaderX64.next) << 4);//
		if (popNode == nullptr)
			break;

		/* Pop�� �����ϴ� �����尡 �� �̻��̶��
		entry�� �����Ϸ��� ���� �ٸ� �����忡�� entry�� ����Ű�� �޸𸮸� �������� �� ������,
		�׷� ��쿡�� Use-After-Free ���� �߻� ����*/
		desired.HeaderX64.next = ((uint64)popNode->next) >> 4;
		desired.HeaderX64.depth = expected.HeaderX64.depth - 1;//������ depth�� �پ��ٴ� ���� ǥ��
		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;// ���° push/pop �۾����� ǥ��

		if (::InterlockedCompareExchange128((int64*)header, desired.region, desired.alignment, (int64*)&expected) == 1)
			break;
	}

	return popNode;
}