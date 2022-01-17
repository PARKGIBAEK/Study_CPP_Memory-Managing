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
	// [MemoryHeader][Data] => Data영역이 실 사용 메모리 영역이며, 맨앞에 MemoryHeader영역은 메모리 관련 정보를 저장
	MemoryHeader(int32 size) : allocSize(size) { }

	// MemoryHeader영역에 사이즈 기입하기
	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		// placement new기법으로 메모리 앞부분에 MemoryHeader 생성자 호출하고, 사이즈 기입
		new(header) MemoryHeader(size); 
		// 메모리 헤더 뒷부분이 실사용 메모리 영역이므로 MemoryHeader사이즈 만큼 건너뛴 위치를 반환
		return reinterpret_cast<void*>(++header);
	}

	static MemoryHeader* DetachHeader(void* ptr)
	{
		/* 실사용 메모리 영역(Data)의 시작 주소(ptr)앞에는 MemoryHeader가 붙어있고 해당위치가 메모리할당 시작 점이다
			따라서 ptr에서 MemoryHeader사이즈 만큼을 앞으로 당겨서 반환해주어야
			메모리 해제가 가능하다 */
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 allocSize;
	// 필요한 추가 정보 추가 가능
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

	SLIST_HEADER	header;//메모리 풀 컨테이너( SLIST_HEADER는 MS사에서 만든 Interlocked Singly-Linked List의 시작 노드이다, 내부에서 사용되는 노드는 SLIST_ENTRY이다 )
	int32			allocSize = 0;// allocSize크기의 메모리를 풀링 한다
	std::atomic<int32>	usedCount = 0; // 메모리 풀에서 꺼내어 사용 중인 객체의 갯수
	std::atomic<int32>	reservedCount = 0; // 메모리 풀에서 생성된 객체의 갯수
};

