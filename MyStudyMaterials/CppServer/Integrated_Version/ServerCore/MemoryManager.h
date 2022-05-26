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
		// 0~1024 byte���� 32byte����, ~2048 byte���� 128 btye����, ~4096 byte���� 256 byte������ �Ҵ�
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


// �Ҵ� ������ new ��� ����ϴ� Customed Allocator
template<typename Type, typename... Args>
Type* XNew(Args&&... args)
{
	// �޸𸮸� �Ҵ�
	Type* memory = static_cast<Type*>(PoolAllocator::AllocateMemory(sizeof(Type)));
	// placement new�� ���� ������ ȣ��
	new(memory)Type(std::forward<Args>(args)...);
	return memory;
}

template<typename Type>
void XDelete(Type* obj)
{
	// obj�� �Ҹ��� ȣ��
	obj->~Type();
	// MemoryManager Pool�� �ݳ�
	PoolAllocator::ReleaseMemory(obj);
} 

template<typename Type, typename... Args>
std::shared_ptr<Type> MakeShared(Args&&... args)
{
	// shared_ptr�� ��ȯ�� �ֱ�
	return std::shared_ptr<Type>{ XNew<Type>(forward<Args>(args)...), XDelete<Type> };

	//�̰ɷ��ϸ� ���� ��
	//return std::make_shared<Type>( XNew<Type>(forward<Args>(args)...), XDelete<Type> );
}