#pragma once

/*-------------------
	BaseAllocator
-------------------*/

/* 기본 Allocator, malloc과 free를 사용*/
class BaseAllocator
{
public:
	static void* Alloc(int32 size);
	/*{
		return ::malloc(size);
	}*/
	static void		Release(void* ptr);
	/*{
		::free(ptr);
	}*/
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
	static void* Alloc(int32 size);
 /*{
		const int64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
		const int64 dataOffset = pageCount * PAGE_SIZE - size;
		void* baseAddress = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		return static_cast<void*>(static_cast<int8*>(baseAddress) + dataOffset);
	}*/
	static void		Release(void* ptr);
 /*{
		const int64 address = reinterpret_cast<int64>(ptr);
		const int64 baseAddress = address - (address % PAGE_SIZE);
		::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);
	}*/
};

/*-------------------
	PoolAllocator
-------------------*/

class PoolAllocator
{
public:
	static void* Alloc(int32 size);
 /*{
		return GMemory->Allocate(size);
	}*/
	static void		Release(void* ptr);
 /*{
	GMemory->Release(ptr);
	}*/
};

/*-------------------
	STL Allocator
-------------------*/

template<typename T>
class StlAllocator
{
public:
	using value_type = T;

	StlAllocator() { }

	template<typename Other>
	StlAllocator(const StlAllocator<Other>&) { }

	T* allocate(size_t count)
	{
		const int32 size = static_cast<int32>(count * sizeof(T));
		return static_cast<T*>(xalloc(size));
	}

	void deallocate(T* ptr, size_t count)
	{
		xrelease(ptr);
	}
};