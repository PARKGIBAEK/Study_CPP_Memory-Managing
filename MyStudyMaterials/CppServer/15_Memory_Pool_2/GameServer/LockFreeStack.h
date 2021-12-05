#pragma once

/*-------------------
	1�� �õ�
-------------------*/

/*
	�̱� ������ ���α׷����� �۵��ϴ� ���� �����

	�� ����

	����(Stack) �����̹Ƿ� Header�� �ֻ���� �����͸� ����Ű���� ����Ǿ� �ִ�.

	�� �Լ�

	InitializeHead - �ʱ⿡ ����� nullptr�� �ʱ�ȭ ����
	PushEntrySList - Header�� ����Ű�� �ִ� ������ �ֻ�� ���������� ���ο� �����͸� �����ϰ�, Header�� ����Ű�� �ֻ�� �����͸� ���ο� �����͸� ����Ű���� �����Ѵ�

	�� ����ü ��� ����
	SListHeader�� �ֻ�� �����͸� ����Ű�� ���� ������ҷ� ���������.
	���� SListHeader�� ��� next�� nullptr�� ��� empty �����̴�
	SListEntry�� ����� ���� ��带 ����Ű�� ���� �������̴�

	�� ������ Push ����

	���ʷ� SListHeader�� �����,
	Data�� ������� SListHeader�� �������ش�.

	�ڵ� ����)
	SListHeader header;
	InitializeHeader(&header);
	Data* data = new Data();
	data->_hp = 10;
	data->_mp = 20;
	PushEntrySList(&header, data);

	�� ������ Pop ����
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
		2�� �õ�
--------------------/*

/*
  1�� �õ����� ���� ���� �ڷᱸ���� CAS�� �����Ͽ� ��Ƽ�����忡���� ������ ������ ���ñ����� �����ϱ�
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
		3�� �õ�
-------------------*/

/*
	2�� �õ����� �߻��� ABA Problem�� �ذ��ϱ� ����
	�����Ϳ� Tag�� �ٿ��� ����ϵ��� �����غ���
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
		union�� �޸𸮸� �����ϴ� ����̹Ƿ�
		alignment�� region�� 0���� �ʱ�ȭ�ϸ�
		depth, sequence, reserved, next�� ���� 0���� �ʱ�ȭ �ѰͰ� ����
		*/
		alignment = 0;
		region = 0;
	}

	union
	{
		struct
		{
			uint64 alignment;// ������ ���� Ȯ���� ���� counter�� Ȱ��
			uint64 region;// �������� ������ ����
		} DUMMYSTRUCTNAME;

		struct
		{
			//depth(16��Ʈ) �� sequence(48��Ʈ)�� ���� alignment(64��Ʈ)�� �޸� ũ�⿡ ����
			uint64 depth : 16; // stack�� ����(������ ����)
			uint64 sequence : 48; // ���° push/pop �۾����� ī��Ʈ
			//reserved(4��Ʈ) �� next(60��Ʈ)�� ���� region(64��Ʈ)�� �޸� ũ�⿡ ����
			uint64 reserved : 4;
			uint64 next : 60;
		} HeaderX64;
	};
};

void InitializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);