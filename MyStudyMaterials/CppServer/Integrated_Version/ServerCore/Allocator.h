#pragma once
#include <memory>
/*-------------------
	BaseAllocator
-------------------*/

class BaseAllocator
{
public:
	// malloc ����
	static void*	AllocateMemory(int32 size);
	// free ����
	static void		ReleaseMemory(void* ptr);
};

/*-------------------
	StompAllocator
-------------------*/
/*
	�� Stomp Allocator ��� ����
	: �޸� ������ �߻��� ��� �޸� ���� �������� ���� ũ������ �߻��ϰ��Ͽ� ���׸� ã�Ƴ��µ� �����ϴ�.
	 
 */
class StompAllocator
{
	enum { PAGE_SIZE = 0x1000 };

public:
	// PAGE_SIZE��ŭ�� ������ �ּ� ������ �Ҵ��Ѵ�
	static void*	AllocateMemory(int32 size);

	static void		ReleaseMemory(void* ptr);
};

/*-------------------
	PoolAllocator
-------------------*/

class PoolAllocator
{
public:
	static void*	AllocateMemory(int32 size);
	static void		ReleaseMemory(void* ptr);
};



/*-------------------
	STL Allocator
-------------------*/

/*
 STL Container(std::vector, std::queue ���) ��� �� �޸� �Ҵ�/���� ���� �κ��� �⺻ Allocator�� ������� �ʰ� ���� ������ Allocator�� ����ϱ� ���� ���� Ŭ����.
 
  Ŀ���� Allocator�� ����� �� �Ҵ�/���� ���� �Լ� ���̹��� allocate�� deallocate�� ������Ѵ�.
  �ֳ��ϸ� STL Container���� ���ø����� �����Ǿ� ������, 
  STL Container���� �Ҵ�/������ ���캸�� Allocator.allocate() �� ���� ������� �����Ǿ��ִ�.
  ���� ���ø� ���ڷ� ���޹��� Allocator�� allocate/deallocate��� ��� �Լ��� ���������� �ʴ��� 
  IntelliSense�� ���ø� �������� �̸� �˾Ƴ��� ���ϱ� ������ ���� �õ� ������ compile error�� �߻��� ���� ������ �� ������ ����!(�Լ��� ��ġ ������� �ϴ� ����:dependent name)
*/
template<typename T>
class STL_Allocator
{
public:
	using value_type = T;

	STL_Allocator() { }

	template<typename Other>
	STL_Allocator(const STL_Allocator<Other>&) { }

	// STL_Container���� �޸� �Ҵ��� ���� ���������� ȣ���ϰԵ� �Լ�
	T* allocate(size_t count)
	{
		const int32 size = static_cast<int32>(count * sizeof(T));
		return static_cast<T*>(PoolAllocator:: AllocateMemory(size));
	}
	// STL_Container���� �޸� ������ ���� ���������� ȣ���ϰԵ� �Լ�
	void deallocate(T* ptr, size_t count)
	{
		PoolAllocator::ReleaseMemory(ptr);
	}

	template<typename U>
	bool operator==(const STL_Allocator<U>&) { return true; }

	template<typename U>
	bool operator!=(const STL_Allocator<U>&) { return false; }
};