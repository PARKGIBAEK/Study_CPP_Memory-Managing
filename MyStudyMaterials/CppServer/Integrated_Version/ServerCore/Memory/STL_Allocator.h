#pragma once
#include "../Memory/PoolAllocator.h"
#include "../Core/Types.h"


namespace ServerCore
{
/*
    STL Container(std::vector, std::queue ��) ��� ��
    �޸� �Ҵ�/���� ���� �κ��� �⺻ Allocator�� ������� �ʰ�
    ���� ������ Allocator�� ����ϱ� ���� ���� Ŭ����.

    Ŀ���� Allocator�� ����� �� �Ҵ�/���� ���� �Լ� ���̹��� allocate�� deallocate�� ���� ��.
    ����(dependent name) : STL Container�� ���ڷ� �޴� Allocator��
    �޸� �Ҵ�/���� �� Allocator.allocate() ���� ������� �����Ǿ��ִ�.
    ��, Type ���������� �����Ǿ� �ִ�.
    ���� ���ø� ���ڷ� ���޹��� Allocator��
    allocate/deallocate��� ��� �Լ��� �����ϰ� �־�� �Ѵ�.
    IntelliSense�� ���ø� �������� �̸� �˾Ƴ��� ���ϱ� ������ ���� �õ� ������ compile error�� �߻��� ���� ������ �� ������ ����!
*/

template <typename T>
class STL_Allocator
{
public:
    using value_type = T;

    STL_Allocator()
    {
    }

    template <typename Other>
    STL_Allocator(const STL_Allocator<Other>&)
    {
    }

    // STL_Container���� �޸� �Ҵ��� ���� ���������� ȣ���ϰԵ� �Լ�
    T* allocate(size_t _count)
    {
        const int32 size = static_cast<int32>(_count * sizeof(T));
        return static_cast<T*>(PoolAllocator::AllocateMemory(size));
    }

    // STL_Container���� �޸� ������ ���� ���������� ȣ���ϰԵ� �Լ�
    void deallocate(T* _ptr, size_t _count)
    {
        PoolAllocator::ReleaseMemory(_ptr);
    }

    template <typename U>
    bool operator==(const STL_Allocator<U>&) { return true; }

    template <typename U>
    bool operator!=(const STL_Allocator<U>&) { return false; }
};
}
