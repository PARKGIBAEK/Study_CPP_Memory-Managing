#pragma once
#include <memory>
/*-------------------
	BaseAllocator
-------------------*/

class BaseAllocator
{
public:
	// malloc 래핑
	static void*	AllocateMemory(int32 size);
	// free 래핑
	static void		ReleaseMemory(void* ptr);
};

/*-------------------
	StompAllocator
-------------------*/
/*
	※ Stomp Allocator 사용 목적
	: 메모리 오염이 발생할 경우 메모리 접근 위반으로 인한 크래쉬를 발생하게하여 버그를 찾아내는데 용이하다.
	 
 */
class StompAllocator
{
	enum { PAGE_SIZE = 0x1000 };

public:
	// PAGE_SIZE만큼의 공간을 최소 단위로 할당한다
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
 STL Container(std::vector, std::queue 등등) 사용 시 메모리 할당/해제 관련 부분을 기본 Allocator를 사용하지 않고 직접 구현한 Allocator를 사용하기 위해 만든 클래스.
 
  커스텀 Allocator를 사용할 때 할당/해제 관련 함수 네이밍은 allocate와 deallocate로 맞춰야한다.
  왜냐하면 STL Container들은 템플릿으로 구현되어 있으며, 
  STL Container들의 할당/해제를 살펴보면 Allocator.allocate() 와 같은 방식으로 구현되어있다.
  따라서 템플릿 인자로 전달받은 Allocator가 allocate/deallocate라는 멤버 함수를 포함하지를 않더라도 
  IntelliSense가 템플릿 에러까지 미리 알아내지 못하기 때문에 빌드 시도 전에는 compile error가 발생할 것을 예상할 수 없으니 주의!(함수명 일치 시켜줘야 하는 이유:dependent name)
*/
template<typename T>
class STL_Allocator
{
public:
	using value_type = T;

	STL_Allocator() { }

	template<typename Other>
	STL_Allocator(const STL_Allocator<Other>&) { }

	// STL_Container에서 메모리 할당을 위해 내부적으로 호출하게될 함수
	T* allocate(size_t count)
	{
		const int32 size = static_cast<int32>(count * sizeof(T));
		return static_cast<T*>(PoolAllocator:: AllocateMemory(size));
	}
	// STL_Container에서 메모리 해제를 위해 내부적으로 호출하게될 함수
	void deallocate(T* ptr, size_t count)
	{
		PoolAllocator::ReleaseMemory(ptr);
	}

	template<typename U>
	bool operator==(const STL_Allocator<U>&) { return true; }

	template<typename U>
	bool operator!=(const STL_Allocator<U>&) { return false; }
};