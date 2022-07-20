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
		/*	0~1024 byte까지 32byte단위, 
			1025~2048 byte까지 128 btye단위, 
			2049~4096 byte까지 256 byte단위로 할당*/
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

//#define DefaultAllocator

#ifdef DefaultAllocator
template<typename T, typename... Args>
T* XNew(Args... _args)
{
	return new T(std::forward<Args>(_args)...);
}

template<typename T>
void XDelete(T* _obj)
{// 소멸자 호출 후 메모리 풀에 반납
	_obj->~T();
	delete _obj;
}

template<typename T, typename... Args>
std::shared_ptr<T> MakeShared(Args&&... args)
{
	return std::shared_ptr<T>(std::forward<Args>(args)...);
}
#else
template<typename T, typename... Args>
T* XNew(Args... _args)
{
	T* memory = static_cast<T*>(PoolAllocator::AllocateMemory(sizeof(T)));
	new(memory) T(std::forward<Args>(_args)...);
	return memory;
}

template<typename T>
void XDelete(T* _obj)
{// 소멸자 호출 후 메모리 풀에 반납
	_obj->~T();
	PoolAllocator::ReleaseMemory(_obj);
}


template<typename T, typename... Args>
std::shared_ptr<T> MakeShared(Args&&... args)
{
	// shared_ptr로 반환해 주기
	return std::shared_ptr<T>{ XNew<T>(forward<Args>(args)...),
								XDelete<T> };

	// 이걸로하면 오류 남
	//return std::make_shared<Type>( XNew<Type>(forward<Args>(args)...), XDelete<Type> );
}
#endif
