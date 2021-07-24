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
		new(header)MemoryHeader(size); // placement new : header위치에 MemoryHeader의 생성자를 호출하여 할당
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

	// 메모리 풀에 반환하기(넣어주기)
	void			Push(MemoryHeader* ptr);
	// 메모리 풀에서 꺼내오기
	MemoryHeader*	Pop();

private:
	SLIST_HEADER	_header;//메모리 풀로 사용될 컨테이너(SLIST_HEADER는 MS사에서 만든 Lock-Free Stack의 시작 노드이다, 내부에서 사용되는 노드는 SLIST_ENTRY를 사용한다)
	int32			_allocSize = 0;
	atomic<int32>	_useCount = 0;//메모리 풀에서 꺼내어 사용 중인 객체의 갯수
	atomic<int32>	_reserveCount = 0;// 메모리 풀에서 생성된 객체의 갯수
};

