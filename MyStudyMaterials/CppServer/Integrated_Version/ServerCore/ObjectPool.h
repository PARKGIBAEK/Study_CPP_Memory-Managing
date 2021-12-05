#pragma once
#include "Types.h"
#include "MemoryPool.h"

/* 
	※	Memory Pooling과 Object Pooling의 차이점

	- Memory Pooling : Memory Pool을 여러 Class가 공유하는 방식이며
							 메모리 오염이 발생했을 경우 원인을 찾기 어렵다.

	- Object Pooling : 각 Class마다 독립적으로 Memory Pooling을 하는 방식이며
							 메모리 오염이 발생 시 어떤 Class의 Object Pool에서 발생한 것인지  파악하여 문제 발생 지점을 유추할 수 있다.
*/

template<typename Type>
class ObjectPool
{
public:
	template<typename... Args>
	static Type* Pop(Args&&... args)
	{
		// Stomp Allocator와 Base Allocator 선택 분기 매크로
#ifdef _STOMP
		MemoryHeader* ptr = reinterpret_cast<MemoryHeader*>(StompAllocator::AllocateMemory(s_allocSize));
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(ptr, s_allocSize));
#else
		// 풀에서 메모리를 꺼내올 때 메모리 크기를 같이 전달해 준다
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(s_pool.Pop(), s_allocSize));
#endif		
		new(memory)Type(forward<Args>(args)...); // placement new
		return memory;
	}

	static void Push(Type* obj)
	{
		obj->~Type();
#ifdef _STOMP
		StompAllocator::ReleaseMemory(MemoryHeader::DetachHeader(obj));
#else
		s_pool.Push(MemoryHeader::DetachHeader(obj));
#endif
	}

	template<typename... Args>
	static shared_ptr<Type> MakeShared(Args&&... args)
	{
		shared_ptr<Type> ptr = { Pop(forward<Args>(args)...), Push };
		return ptr;
	}

private:
	/* template class는 static 속성이 붙었다고해서 전역으로 단 하나만 존재할 수 있는 것이 아니라
	 선언된 <Type>별로 각 하나씩 존재할 수 있다 */
	static int32		s_allocSize;
	static MemoryPool	s_pool;
};

template<typename Type>
int32 ObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryHeader);

template<typename Type>
MemoryPool ObjectPool<Type>::s_pool{ s_allocSize };