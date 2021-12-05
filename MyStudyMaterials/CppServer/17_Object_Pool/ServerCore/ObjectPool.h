#pragma once
#include "Types.h"
#include "MemoryPool.h"

/*
	��	Memory Pooling�� Object Pooling�� ������

	- Memory Pooling : Memory Pool�� ���� Class�� �����ϴ� ����̸�
							 �޸� ������ �߻����� ��� ������ ã�� ��ƴ�.

	- Object Pooling : �� Class���� ���������� Memory Pooling�� �ϴ� ����̸�
							 �޸� ������ �߻� �� � Class�� Object Pool���� �߻��� ������  �ľ��Ͽ� ���� �߻� ������ ������ �� �ִ�.
*/

template<typename Type>
class ObjectPool
{
public:
	template<typename... Args>
	static Type* Pop(Args&&... args)
	{
#ifdef _STOMP
		MemoryHeader* ptr = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(s_allocSize));
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(ptr, s_allocSize));
#else
		// Ǯ���� �޸𸮸� ������ �� �޸� ũ�⸦ ���� ������ �ش�
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(s_pool.Pop(), s_allocSize));
#endif
		new(memory)Type(forward<Args>(args)...); // placement new : memory��ġ���� Type�� �����ڸ� ȣ��
		return memory;
	}

	static void Push(Type* obj)
	{
		//�Ҹ��ڸ� ȣ���� �� Ǯ�� �ݳ�
		obj->~Type();
#ifdef _STOMP
		StompAllocator::Release(MemoryHeader::DetachHeader(obj));
#else
		// �ݳ��� �� MemoryHeader�� �����Ͽ� �ݳ�
		s_pool.Push(MemoryHeader::DetachHeader(obj));
#endif
	}
	// custom deleter�� Push�� �����ϸ� �Ҹ� �� �ڵ����� Ǯ�� ��ȯ�Ѵ�
	static shared_ptr<Type> MakeShared()
	{
		shared_ptr<Type> ptr = { Pop(), Push };
		return ptr;
	}

private:
	/* template class�� static �Ӽ��� �پ��ٰ��ؼ� �������� �� �ϳ��� ������ �� �ִ� ���� �ƴ϶�
	 ����� <Type>���� �� �ϳ��� ������ �� �ִ� */
	static int32		s_allocSize;
	static MemoryPool	s_pool;
};
/*
	������ �������� template�� 
*/
template<typename Type>
int32 ObjectPool<Type>::s_allocSize = sizeof(Type) + sizeof(MemoryHeader);

template<typename Type>
MemoryPool ObjectPool<Type>::s_pool{ s_allocSize };