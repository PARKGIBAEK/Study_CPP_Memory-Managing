#pragma once
#include "Types.h"
#include "MemoryPool.h"

/* 
	��	Memory Pool�� Object Pool�� ����

	 * Memory Pool : 
	 - Memory Pool�� ���� Class�� �����ϴ� ����̸�
	   �޸� ������ �߻����� ��� ������ ã�� ��ƴ�.


	 * Object Pool : 
	 - Object Pool�� template class�̸�, static �ɹ��� �����ϱ� ������
	   �� class�� template ���ڷ� ȣ���ϴ� ��쿡�� �ν��Ͻ̵ȴ�.
	   �ٽ� ���ؼ� Class���� Pooling�ϴ� ����̱� ������ allocSize�� class�� ���� �ٸ���. 
	   �޸� ���� �߻� �� � Class�� Object Pool���� �߻��� ������ ���� �ľ��� �� �ִ�.
*/

template<typename Type>
class ObjectPool
{
public:
	// ȣ���� Type �����ڿ� �´� ���ڸ� �������ִ� �� ���� ����
	template<typename... Args>
	static Type* Pop(Args&&... args)
	{
#ifdef _STOMP_ALLOCATOR
		MemoryHeader* ptr = reinterpret_cast<MemoryHeader*>(
			StompAllocator::AllocateMemory(s_allocSize));

		Type* memory = static_cast<Type*>(
			MemoryHeader::AttachHeader(ptr, s_allocSize));
#else
		// �޸�Ǯ���� �޸𸮸� ������ �� �޸� ũ�⸦ ���� ������ �ش�
		Type* memory = static_cast<Type*>(
			MemoryHeader::AttachHeader(s_pool.Pop(), s_allocSize));
#endif
		// Type �����ڿ� �´� ���ڸ� ����
		new(memory)Type(std::forward<Args>(args)...); // placement new
		return memory;
	}
	// ����� ���� ��ü�� ObjectPool�� �ݳ�
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
	/* template class�� static �Ӽ��� �پ��ٰ��ؼ� �������� �� �ϳ��� ������ �� �ִ� ���� �ƴ϶�
	 ����� <Type>���� �� �ϳ��� ������ �� �ִ� */
	static int32		s_allocSize;
	static MemoryPool	s_pool;
};

template<typename Type>
int32 ObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryHeader);

template<typename Type>
MemoryPool ObjectPool<Type>::s_pool{ s_allocSize };