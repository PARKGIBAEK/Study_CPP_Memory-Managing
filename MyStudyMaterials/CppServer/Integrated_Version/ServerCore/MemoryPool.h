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
	// [MemoryHeader][Data]
	MemoryHeader(int32 size) : allocSize(size) { }

	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		new(header) MemoryHeader(size); // placement new �� ����� ������ ȣ��
		return reinterpret_cast<void*>(++header);
	}

	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 allocSize;
	// TODO : �ʿ��� �߰� ����
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

	SLIST_HEADER	header;//�޸� Ǯ �����̳�( SLIST_HEADER�� MS�翡�� ���� Lock-Free Stack�� ���� ����̴�, ���ο��� ���Ǵ� ���� SLIST_ENTRY�̴� )
	int32			allocSize = 0;// allocSizeũ���� �޸𸮸� Ǯ�� �Ѵ�
	atomic<int32>	usedCount = 0;//�޸� Ǯ���� ������ ��� ���� ��ü�� ����
	atomic<int32>	reservedCount = 0;// �޸� Ǯ���� ������ ��ü�� ����
};

