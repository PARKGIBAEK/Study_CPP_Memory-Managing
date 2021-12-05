#pragma once

/*-------------------
	1차 시도
-------------------*/

/*
	싱글 쓰레드 프로그램에서 작동하는 스택 만들기

	※ 구조

	스택(Stack) 구조이므로 Header는 최상단의 데이터를 가리키도록 설계되어 있다.

	※ 함수

	InitializeHead - 초기에 헤더를 nullptr로 초기화 해줌
	PushEntrySList - Header가 가리키고 있던 기존의 최상단 데이터위에 새로운 데이터를 연결하고, Header가 가리키던 최상단 데이터를 새로운 데이터를 가리키도록 갱신한다

	※ 구조체 멤버 설명
	SListHeader는 최상단 데이터를 가리키기 위한 헤더역할로 만들어졌다.
	따라서 SListHeader의 멤버 next가 nullptr일 경우 empty 상태이다
	SListEntry는 연결된 다음 노드를 가리키기 위한 포인터이다

	※ 데이터 Push 과정

	최초로 SListHeader를 만들고,
	Data를 만든다음 SListHeader에 연결해준다.

	코드 예시)
	SListHeader header;
	InitializeHeader(&header);
	Data* data = new Data();
	data->_hp = 10;
	data->_mp = 20;
	PushEntrySList(&header, data);

	※ 데이터 Pop 과정
	Data* popData = (Data*)PopEntrySList(&header);
*/


/*

struct SListEntry
{
	SListEntry* next;
};

struct SListHeader
{
	SListEntry* next = nullptr;
};

void InitializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);
*/


//=========================================================


/*-------------------
		2차 시도
--------------------/*

/*
  1차 시도에서 만든 스택 자료구조를 CAS를 응용하여 멀티쓰레드에서도 동작이 가능한 스택구조로 변경하기
*/

/*
struct SListEntry
{
	SListEntry* next;
};

struct SListHeader
{
	SListEntry* next = nullptr;
};

void InitializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);
*/

//=========================================================


/*-------------------
		3차 시도
-------------------*/

/*
	2차 시도에서 발생한 ABA Problem을 해결하기 위해
	데이터에 Tag를 붙여서 사용하도록 구현해보자
*/

//__declspec(align(16))
DECLSPEC_ALIGN(16)
struct SListEntry
{
	SListEntry* next;
};

DECLSPEC_ALIGN(16)
struct SListHeader
{
	SListHeader()
	{
		/*
		union은 메모리를 공유하는 방식이므로
		alignment와 region을 0으로 초기화하면
		depth, sequence, reserved, next를 각각 0으로 초기화 한것과 같음
		*/
		alignment = 0;
		region = 0;
	}

	union
	{
		struct
		{
			uint64 alignment;// 스레드 경합 확인을 위한 counter로 활용
			uint64 region;// 데이터의 포인터 저장
		} DUMMYSTRUCTNAME;

		struct
		{
			//depth(16비트) 와 sequence(48비트)가 위의 alignment(64비트)의 메모리 크기에 대응
			uint64 depth : 16; // stack의 깊이(데이터 갯수)
			uint64 sequence : 48; // 몇번째 push/pop 작업인지 카운트
			//reserved(4비트) 와 next(60비트)가 위의 region(64비트)의 메모리 크기에 대응
			uint64 reserved : 4;
			uint64 next : 60;
		} HeaderX64;
	};
};

void InitializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);