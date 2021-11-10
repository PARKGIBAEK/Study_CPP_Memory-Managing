#pragma once
#include "Allocator.h"
using namespace std;

class MemoryPool;

/*-------------
	Memory
---------------*/

class Memory
{
	enum
	{
		// ~1024까지 32단위, ~2048까지 128단위, ~4096까지 256단위
		POOL_COUNT = (1024 / 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096
	};

public:
	Memory();
	~Memory();

	void*	Allocate(int32 size);
	void	Release(void* ptr);

private:
	vector<MemoryPool*> _pools;

	// 메모리 크기 <-> 메모리 풀
	// O(1) 빠르게 찾기 위한 테이블
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];
};

// new 연산자 대신 사용
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
	// Memory Pool에 반납
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