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
		2차 시도
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

void PushEntrySList(SListHeader* header, SListEntry* newNode)
{

	SListHeader expected{};
	SListHeader desired{};

	/* 64비트 모드에서는 메모리 주소는 항상 16의 배수이다.
		그러므로 메모리 주소를 16진수로 표현할 경우 최하위 자리수는 항상 0이다.
		또한 메모리 주소를 2진수로 표현한다면 하위 4자리는 항상 0이다.
		따라서 이 주소를 다른 변수에 bit shift >> 4로 저장해더라도
		bit shift << 4를 해주면 복구된다

		64비트 모드 메모리 주소 : 0x000001B57D6F2540 (16진수 한자리는 4비트에 해당)

		MS에서 구현한 SList를 사용하기 위해 16바이트 정렬을 하는 것임.	*/
	desired.HeaderX64.next = (((uint64)newNode) >> 4); // Push할 노드의 메모리 주소 저장

	while (true)
	{
		expected = *header;//head값 복사

		// 이 사이에 header가 변경될 수도 있으므로 검증 필요
		//위에서 bit shift>>4 했던 것을 복원하는 과정

		// newNode->next를 기존header->next로 바꿔준다(노드 연결 과정)
		newNode->next = (SListEntry*)(((uint64)expected.HeaderX64.next) << 4);


		desired.HeaderX64.depth = expected.HeaderX64.depth + 1;
		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;
		
		// header와 expected가 같을 경우 header를 expected로 변환하고 1을 반환, 다르면  0을 반환
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

		/* Pop을 수행하는 쓰레드가 둘 이상이라면
		entry를 참조하려는 순간 다른 쓰레드에서 entry가 가리키는 메모리를 날려버릴 수 있으며,
		그럴 경우에는 Use-After-Free 문제 발생 가능*/
		desired.HeaderX64.next = ((uint64)popNode->next) >> 4;
		desired.HeaderX64.depth = expected.HeaderX64.depth - 1;//스택의 depth가 줄었다는 것을 표기
		desired.HeaderX64.sequence = expected.HeaderX64.sequence + 1;// 몇번째 push/pop 작업인지 표기

		if (::InterlockedCompareExchange128((int64*)header, desired.region, desired.alignment, (int64*)&expected) == 1)
			break;
	}

	return popNode;
}