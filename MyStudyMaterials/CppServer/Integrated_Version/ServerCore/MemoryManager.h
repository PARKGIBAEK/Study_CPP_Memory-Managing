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
		/*	0~1024 byte���� 32byte����, 
			1025~2048 byte���� 128 btye����, 
			2049~4096 byte���� 256 byte������ �Ҵ�*/
		POOL_COUNT = (1024 / 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096,
	};

public:
	MemoryManager();
	~MemoryManager();

	void*	Allocate(int32 size);
	void	Release(void* ptr);

private:
	vector<MemoryPool*> pools; //�Ҵ�� ��� �޸𸮸� �����ϱ� ���� �޸� ������ ��� ����

	// �Ҵ��� �޸� ũ�⿡ �´� �޸� Ǯ�� ������ ã�� ���� ���̺�(������ ����)
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
{// �Ҹ��� ȣ�� �� �޸� Ǯ�� �ݳ�
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
{// �Ҹ��� ȣ�� �� �޸� Ǯ�� �ݳ�
	_obj->~T();
	PoolAllocator::ReleaseMemory(_obj);
}


template<typename T, typename... Args>
std::shared_ptr<T> MakeShared(Args&&... args)
{
	// shared_ptr�� ��ȯ�� �ֱ�
	return std::shared_ptr<T>{ XNew<T>(forward<Args>(args)...),
								XDelete<T> };

	// �̰ɷ��ϸ� ���� ��
	//return std::make_shared<Type>( XNew<Type>(forward<Args>(args)...), XDelete<Type> );
}
#endif
