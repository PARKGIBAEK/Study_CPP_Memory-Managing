#include "../Memory/MemoryManager.h"
#include "../Memory/MemoryPool.h"
#include "../Core/CoreInitializer.h"
#include "../Core/CoreMacro.h"
#include "../Memory/MemoryHeader.h"


namespace ServerCore
{
MemoryManager::MemoryManager()
{
	int32 size = 0;
	int32 tableIndex = 0;

	/* 각 사이즈별 메모리 풀 생성

	- 32 byte, 64 byte, 96 byte, 128 byte... 1024 byte 단위 사이즈의
	 MemoryManager Pool들을 각각 생성한다.

	 예를들면
	 32 byte 단위 사이즈 MemoryManager Pool은 1~32 byte 단위 크기의 메모리를 할당해주고,
	 64 byte 단위 사이즈 MemoryManager Pool은 33~64 byte 단위 크기의 메모리를 할당해주는 방식이다.
	*/
	for (size = 32; size <= 1024; size += 32) // 메모리 풀 사이즈 32씩 증가
	{
		MemoryPool* pool = new MemoryPool(size);
		pools.push_back(pool);

		while (tableIndex <= size)
		{
			poolTable[tableIndex] = pool;
			tableIndex++;
			/* 예시) poolTable[0~32]는 32 bytes짜리 MemoryPool을 참조
					  poolTable[33~64]는 64 bytes짜리 MemoryPool을 참조 */
		}
	}

	for (size = 1024 + 128; size <= 2048; size += 128) // 메모리 풀 사이즈 128씩 증가
	{
		MemoryPool* pool = new MemoryPool(size);
		pools.push_back(pool);

		while (tableIndex <= size)
		{
			poolTable[tableIndex] = pool;
			tableIndex++;
		}
	}

	for (size = 2048 + 256; size <= MAX_ALLOC_SIZE; size += 256) // 메모리 풀 사이즈 256씩 증가
	{
		MemoryPool* pool = new MemoryPool(size);
		pools.push_back(pool);

		while (tableIndex <= size)
		{
			poolTable[tableIndex] = pool;
			tableIndex++;
		}
	}
}

MemoryManager::~MemoryManager()
{
	for (MemoryPool* pool : pools)
		delete pool; // MemoryPool 소멸자 호출

	pools.clear();
}

void* MemoryManager::Allocate(int32 size)
{
	//G_CoreGlobal = new CoreGlobal();

	// CoreInitializer::Init(); // 전역 변수 초기화
	MemoryHeader* header = nullptr;
	const int32 allocSize = size + sizeof(MemoryHeader);// 맨앞에 메모리 헤더를 붙이기 위해 MemoryHeader 사이즈 만큼 더 크게 생성

#ifdef _STOMP_ALLOCATOR
	header = reinterpret_cast<MemoryHeader*>(StompAllocator::AllocateMemory(allocSize));
#else
	if (allocSize > MAX_ALLOC_SIZE)
	{// 메모리 풀링 최대 크기를 벗어나면 일반 할당

		header = reinterpret_cast<MemoryHeader*>(
			::_aligned_malloc(allocSize, static_cast<int>(ALIGNMENT::SLIST_ALIGNMENT)));
	}
	else
	{
		// 메모리 풀에서 꺼내온다
		header =GMemoryManager->poolTable[allocSize]->Pop();
	}
#endif	

	return MemoryHeader::AttachHeader(header, allocSize);// MemoryHeader뒷부분의 실사용 영역의 주소 반환
}

void MemoryManager::Release(void* ptr)
{
	MemoryHeader* header = MemoryHeader::DetachHeader(ptr);

	const int32 allocSize = header->allocSize;
	ASSERT_CRASH(allocSize > 0);

#ifdef _STOMP_ALLOCATOR
	StompAllocator::ReleaseMemory(header);
#else
	if (allocSize > MAX_ALLOC_SIZE)
	{
		// 메모리 풀링 최대 크기를 벗어나면 일반 해제
		::_aligned_free(header);
	}
	else
	{
		// 메모리 풀에 반납한다
		GMemoryManager->poolTable[allocSize]->Push(header);
	}
#endif	
}
}