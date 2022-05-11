#pragma once
#include "Types.h"
#include "MemoryPool.h"

/* 
	※	Memory Pool과 Object Pool의 차이

	 * Memory Pool : 
	 - Memory Pool을 여러 Class가 공유하는 방식이며
	   메모리 오염이 발생했을 경우 원인을 찾기 어렵다.


	 * Object Pool : 
	 - Object Pool은 template class이며, static 맴버만 존재하기 때문에
	   각 class를 template 인자로 호출하는 경우에만 인스턴싱된다.
	   다시 말해서 Class별로 Pooling하는 방식이기 때문에 allocSize가 class에 따라 다르다. 
	   메모리 오염 발생 시 어떤 Class의 Object Pool에서 발생한 것인지 쉽게 파악할 수 있다.
*/

template<typename Type>
class ObjectPool
{
public:
	// 호출할 Type 생성자에 맞는 인자를 전달해주는 것 잊지 말기
	template<typename... Args>
	static Type* Pop(Args&&... args)
	{
#ifdef _STOMP_ALLOCATOR
		MemoryHeader* ptr = reinterpret_cast<MemoryHeader*>(
			StompAllocator::AllocateMemory(s_allocSize));

		Type* memory = static_cast<Type*>(
			MemoryHeader::AttachHeader(ptr, s_allocSize));
#else
		// 메모리풀에서 메모리를 꺼내올 때 메모리 크기를 같이 전달해 준다
		Type* memory = static_cast<Type*>(
			MemoryHeader::AttachHeader(s_pool.Pop(), s_allocSize));
#endif
		// Type 생성자에 맞는 인자를 전달
		new(memory)Type(std::forward<Args>(args)...); // placement new
		return memory;
	}
	// 사용이 끝난 객체를 ObjectPool에 반납
	static void Push(Type* obj)
	{
		obj->~Type();
#ifdef _STOMP_ALLOCATOR
		StompAllocator::ReleaseMemory(MemoryHeader::DetachHeader(obj));
#else
		s_pool.Push(MemoryHeader::DetachHeader(obj));
#endif
	}
	
	template<typename... Args>
	static std::shared_ptr<Type> MakeShared(Args&&... args)
	{
		std::shared_ptr<Type> ptr = { Pop(std::forward<Args>(args)...), Push };
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