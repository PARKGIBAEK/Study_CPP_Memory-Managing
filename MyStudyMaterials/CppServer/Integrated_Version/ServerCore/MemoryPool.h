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
	MemoryHeader(int32 _size) : allocSize(_size) { }

	// MemoryHeader영역에 사이즈 기입하기
	static void* AttachHeader(MemoryHeader* _header, int32 _size)
	{
		// placement new기법으로 메모리 앞부분에 MemoryHeader 생성자 호출하고, 사이즈 기입
		new(_header) MemoryHeader(_size); 
		// 메모리 헤더 뒷부분이 실사용 메모리 영역이므로 MemoryHeader사이즈 만큼 건너뛴 위치를 반환
		return reinterpret_cast<void*>(++_header);
	}

	// 주소를 MemoryHeader 사이즈 만큼 앞으로 당겨서 반환
	static MemoryHeader* DetachHeader(void* ptr)
	{
		/* 실사용 메모리 영역(Data)의 시작 주소(NodePtr)앞에는 MemoryHeader가 붙어있고 해당위치가 메모리할당 시작 점이다
			따라서 ptr에서 MemoryHeader사이즈 만큼을 앞으로 당겨서 반환해주어야
			메모리 해제가 가능하다 */
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 allocSize;
	// 필요한 정보 추가 가능
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