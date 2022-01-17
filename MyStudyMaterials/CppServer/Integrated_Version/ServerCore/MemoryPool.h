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
	MemoryHeader(int32 size) : allocSize(size) { }

	// MemoryHeader������ ������ �����ϱ�
	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		// placement new������� �޸� �պκп� MemoryHeader ������ ȣ���ϰ�, ������ ����
		new(header) MemoryHeader(size); 
		// �޸� ��� �޺κ��� �ǻ�� �޸� �����̹Ƿ� MemoryHeader������ ��ŭ �ǳʶ� ��ġ�� ��ȯ
		return reinterpret_cast<void*>(++header);
	}

	static MemoryHeader* DetachHeader(void* ptr)
	{
		/* �ǻ�� �޸� ����(Data)�� ���� �ּ�(ptr)�տ��� MemoryHeader�� �پ��ְ� �ش���ġ�� �޸��Ҵ� ���� ���̴�
			���� ptr���� MemoryHeader������ ��ŭ�� ������ ��ܼ� ��ȯ���־��
			�޸� ������ �����ϴ� */
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 allocSize;
	// �ʿ��� �߰� ���� �߰� ����
};

/*-----------------
	MemoryPool
------------------*/

DECLSPEC_ALIGN(SLIST_ALIGNMENT)
class MemoryPool
{
public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

	void			Push(MemoryHeader* ptr);
	MemoryHeader* Pop();

private:

	SLIST_HEADER	header;//�޸� Ǯ �����̳�( SLIST_HEADER�� MS�翡�� ���� Interlocked Singly-Linked List�� ���� ����̴�, ���ο��� ���Ǵ� ���� SLIST_ENTRY�̴� )
	int32			allocSize = 0;// allocSizeũ���� �޸𸮸� Ǯ�� �Ѵ�
	std::atomic<int32>	usedCount = 0; // �޸� Ǯ���� ������ ��� ���� ��ü�� ����
	std::atomic<int32>	reservedCount = 0; // �޸� Ǯ���� ������ ��ü�� ����
};

