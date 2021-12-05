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
		new(header) MemoryHeader(size); // placement new 를 사용한 생성자 호출
		return reinterpret_cast<void*>(++header);
	}

	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 allocSize;
	// TODO : 필요한 추가 정보
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

	SLIST_HEADER	header;//메모리 풀 컨테이너( SLIST_HEADER는 MS사에서 만든 Lock-Free Stack의 시작 노드이다, 내부에서 사용되는 노드는 SLIST_ENTRY이다 )
	int32			allocSize = 0;// allocSize크기의 메모리를 풀링 한다
	atomic<int32>	usedCount = 0;//메모리 풀에서 꺼내어 사용 중인 객체의 갯수
	atomic<int32>	reservedCount = 0;// 메모리 풀에서 생성된 객체의 갯수
};

