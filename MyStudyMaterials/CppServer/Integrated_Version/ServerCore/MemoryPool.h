#pragma once

enum
{
	SLIST_ALIGNMENT = 16
};

/*-----------------
	MemoryHeader
------------------*/

DECLSPEC_ALIGN(SLIST_ALIGNMENT)
struct MemoryHeader : public SLIST_ENTRY
{
	// [MemoryHeader][Data] => Data������ �� ��� �޸� �����̸�, �Ǿտ� MemoryHeader������ �޸� ���� ������ ����
	MemoryHeader(int32 _size) : allocSize(_size) { }

	// MemoryHeader������ ������ �����ϱ�
	static void* AttachHeader(MemoryHeader* _header, int32 _size)
	{
		// placement new������� �޸� �պκп� MemoryHeader ������ ȣ���ϰ�, ������ ����
		new(_header) MemoryHeader(_size); 
		// �޸� ��� �޺κ��� �ǻ�� �޸� �����̹Ƿ� MemoryHeader������ ��ŭ �ǳʶ� ��ġ�� ��ȯ
		return reinterpret_cast<void*>(++_header);
	}

	// �ּҸ� MemoryHeader ������ ��ŭ ������ ��ܼ� ��ȯ
	static MemoryHeader* DetachHeader(void* ptr)
	{
		/* �ǻ�� �޸� ����(Data)�� ���� �ּ�(NodePtr)�տ��� MemoryHeader�� �پ��ְ� �ش���ġ�� �޸��Ҵ� ���� ���̴�
			���� ptr���� MemoryHeader������ ��ŭ�� ������ ��ܼ� ��ȯ���־��
			�޸� ������ �����ϴ� */
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 allocSize;
	// �ʿ��� ���� �߰� ����
};

/*-----------------
	MemoryPool
------------------*/

class alignas(SLIST_ALIGNMENT) MemoryPool
{
public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

	void				Push(MemoryHeader* ptr);
	MemoryHeader*		Pop();

private:

	SLIST_HEADER		header;
	int32				allocSize = 0;
	std::atomic<int32>	usedCount = 0;
	std::atomic<int32>	reservedCount = 0;
};