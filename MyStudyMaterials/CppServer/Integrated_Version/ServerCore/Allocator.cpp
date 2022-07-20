#include "pch.h"
#include "Allocator.h"
//#include "Memory.h"

/*-------------------
	BaseAllocator
-------------------*/

void* BaseAllocator::AllocateMemory(int32 size)
{
	return ::malloc(size);
}

void BaseAllocator::ReleaseMemory(void* ptr)
{
	::free(ptr);
}

/*-------------------
	StompAllocator
-------------------*/

void* StompAllocator::AllocateMemory(int32 size)
{
	//메모리 할당 단위를 PAGE_SIZE로 하기 위한 과정
	const int64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	/* 할당한 메모리 중 마지막 메모리의 주소를 기준으로
		size만큼 앞으로 당긴 위치를 반환하기 위함
		
		예시) 4096 byte 할당 받았고, 메모리의 시작점이 0x0000'0000이라고 가정했을 때
		    사용할 메모리 크기가 16이라면
			 시작 주소(0000) + 할당받은 메모리 크기(4096) - 사용할 메모리 크기(16) = Offset(4080)
			 0x0000'0000 + 0x0000'1000 - 0x0000'0010 = 0x0000'0FF0 이 된다.

	BaseAddress(0000)                      Offset(4080)
			↓                                  ↓
			[            Allocated memory             ]
															 ↑
														  메모리 끝 주소(4096)
	*/
	const int64 offset = pageCount * PAGE_SIZE - size;
	
	// pageCount x PAGE_SIZE만큼 할당하고 할당 주소의 시작점을 받아옴
	/* VirtualAlloc 옵션
	- MEM_RESERVE : 메모리 할당 예약(페이징 파일에 실제 물리적 저장공간을 할당하지 않고, 프로세스의 가상 주소 공간 범위를 예약)
	 
	- MEM_COMMIT : 예약된 메모리 페이지에 MemoryManager Charge(할당한 메모리의 크기와 페이징 파일의 크기)를 할당.
					단, 가상 주소에 실제로 엑세스하지 않는 한 실제 물리적 페이지는 할당되지 않는다.
	
	※ 페이지를 예약과 동시에 할당하려면 ?
	  MEM_COMMIT | MEM_RESERVE 옵션을 사용하면 된다.
	  즉, 할당과 동시에 사용할 경우를 말한다.
	  ( 메모리 내용이 0으로 초기화 됨을 보장한다 )

	- PAGE_READWRITE : 읽기/쓰기 접근 허용, 데이터 실행방지 옵션이 활성화 된 경우 커밋된 영역에서 코드를 실행할 경우 액세스 위반 발생*/
	void* baseAddress = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	/* VirtualAlloc 함수를 통해 할당받은 메모리의 주소에서 실제 사용할 주소의 위치를 반환 	*/
	return static_cast<void*>(static_cast<int8*>(baseAddress) + offset);
}

void StompAllocator::ReleaseMemory(void* ptr)
{
	// AllocateMemory 함수에서 할당한 전체 메모리 중 실제 사용할 목적으로 반환받은 주소 ( baseAddress + offset )
	const int64 address = reinterpret_cast<int64>(ptr);

	// 위의 주소에서 offset을 뺀 주소 ( baseAddress + offset ) - offset
	// ex) base : 5000, offset : 500, address : 5500
	//		 5500 - ( 5500 % 4096 ) = 5500 - 1404  = 4096

	const int64 baseAddress = address - (address % PAGE_SIZE);
	// MEM_RELEASE옵션을 사용할 경우 VirtualAlloc함수를 통해 할당한 메모리의 base address를 인자로 넣어주어야한다
	::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);
}

/*-------------------
	PoolAllocator
-------------------*/

void* PoolAllocator::AllocateMemory(int32 size)
{
	return GMemoryManager->Allocate(size);
}

void PoolAllocator::ReleaseMemory(void* ptr)
{
	GMemoryManager->Release(ptr);
}