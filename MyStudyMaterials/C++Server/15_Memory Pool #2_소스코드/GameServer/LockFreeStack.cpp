#include "pch.h"
#include "LockFreeStack.h"

/*-------------------
	1차 시도
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
		2차 시도
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
	//최상단 데이터를 꺼내옴
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
		3차 시도
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

	/* 64비트 모드에서는 포인터 변수인 entry가 가리키는 메모리 주소의 마지막 자리는 0이다. 즉, 16의 배수이다. 
	* 	  예) 0x000001B57D6F2540   16진수의는 한자리당 4비트에 해당한다.
	
	따라서 16의 배수로 된 주소의 하위4비트는 0이 되므로 bit shift >> 4를 해서 사용해도 문제가 없다.
	
	* MS에서 구현한 SList를 사용한다면 반드시 16바이트 정렬을 해줘야한다
	- by Microsoft -
	*/
	desired.HeaderX64.next = (((uint64)entry) >> 4);

	while (true)
	{
		expected = *header;

		// 이 사이에 header가 변경될 수도 있으므로 검증 필요

		//위에서 bit shift>>4 했던 것을 복원하는 과정
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

		/* Pop을 수행하는 쓰레드가 둘 이상이라면
		entry를 참조하려는 순간 다른 쓰레드에서 entry가 가리키는 메모리를 날려버릴 수 있으며,
		그럴 경우에는 Use-After-Free 문제 발생 가능*/
		desired.HeaderX64.next = ((uint64)entry->next) >> 4;
		desired.HeaderX64.depth = expected.HeaderX64.depth - 1;//스택의 depth가 줄었다는 것을 표기
		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;// 몇번째 push/pop 작업인지 표기

		if (::InterlockedCompareExchange128((int64*)header, desired.region, desired.alignment, (int64*)&expected) == 1)
			break;
	}

	return entry;
}