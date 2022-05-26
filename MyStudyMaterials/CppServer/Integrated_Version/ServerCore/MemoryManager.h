#pragma once
#include "Allocator.h"
using namespace std;

class MemoryPool;

/*-------------
	MemoryManager
---------------*/

class MemoryManager
{
	enum
	{
		// 0~1024 byte까지 32byte단위, ~2048 byte까지 128 btye단위, ~4096 byte까지 256 byte단위로 할당
		POOL_COUNT = (1024 / 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096,
	};

public:
	MemoryManager();
	~MemoryManager();

	void*	Allocate(int32 size);
	void	Release(void* ptr);

private:
	vector<MemoryPool*> pools; //할당된 모든 메모리를 해제하기 위해 메모리 참조를 모두 보관

	// 할당할 메모리 크기에 맞는 메모리 풀을 빠르게 찾기 위한 테이블(생성자 참고)
	MemoryPool* poolTable[MAX_ALLOC_SIZE + 1];
};


// 할당 연산자 new 대신 사용하는 Customed Allocator
template<typename Type, typename... Args>
Type* XNew(Args&&... args)
{
	// 메모리만 할당
	Type* memory = static_cast<Type*>(PoolAllocator::AllocateMemory(sizeof(Type)));
	// placement new를 통해 생성자 호출
	new(memory)Type(std::forward<Args>(args)...);
	return memory;
}

template<typename Type>
void XDelete(Type* obj)
{
	// obj의 소멸자 호출
	obj->~Type();
	// MemoryManager Pool에 반납
	PoolAllocator::ReleaseMemory(obj);
} 

template<typename Type, typename... Args>
std::shared_ptr<Type> MakeShared(Args&&... args)
{
	// shared_ptr로 반환해 주기
	return std::shared_ptr<Type>{ XNew<Type>(forward<Args>(args)...), XDelete<Type> };

	//이걸로하면 오류 남
	//return std::make_shared<Type>( XNew<Type>(forward<Args>(args)...), XDelete<Type> );
}