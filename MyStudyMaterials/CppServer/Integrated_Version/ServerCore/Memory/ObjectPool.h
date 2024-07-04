#pragma once
#include <memory>
#include "../Memory/MemoryHeader.h"
#include "../Core/Types.h"
#include "../Memory/MemoryPool.h"


namespace ServerCore
{
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

template <typename Type>
class ObjectPool
{
public:
    template <typename... Args>
    static Type* Pop(Args&&... args)
    {
        Type* memory = static_cast<Type*>(
            MemoryHeader::AttachHeader(s_pool.Pop(), s_allocSize));
        new(memory) Type(std::forward<Args>(args)...); // placement new
        return memory;
    }

    static void Push(Type* obj)
    {
        obj->~Type(); // 소멸자 호출
        s_pool.Push(MemoryHeader::DetachHeader(obj));
    }

    template <typename... Args>
    static std::shared_ptr<Type> MakeShared(Args&&... args)
    {
        std::shared_ptr<Type> ptr = {Pop(std::forward<Args>(args)...), Push};
        return ptr;
    }

private:
    static int32 s_allocSize;
    static MemoryPool s_pool;
};

template <typename Type>
int32 ObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryHeader);

template <typename Type>
MemoryPool ObjectPool<Type>::s_pool{s_allocSize};
}
