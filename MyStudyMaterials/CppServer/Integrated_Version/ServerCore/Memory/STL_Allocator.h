#pragma once
#include "../Memory/PoolAllocator.h"
#include "../Core/Types.h"


namespace ServerCore
{
/*
    STL Container(std::vector, std::queue 등) 사용 시
    메모리 할당/해제 관련 부분을 기본 Allocator를 사용하지 않고
    직접 구현한 Allocator를 사용하기 위해 만든 클래스.

    커스텀 Allocator를 사용할 때 할당/해제 관련 함수 네이밍은 allocate와 deallocate로 맞출 것.
    이유(dependent name) : STL Container는 인자로 받는 Allocator를
    메모리 할당/해제 시 Allocator.allocate() 같은 방식으로 구현되어있다.
    즉, Type 의존적으로 구현되어 있다.
    따라서 템플릿 인자로 전달받은 Allocator가
    allocate/deallocate라는 멤버 함수를 포함하고 있어야 한다.
    IntelliSense가 템플릿 에러까지 미리 알아내지 못하기 때문에 빌드 시도 전에는 compile error가 발생할 것을 예상할 수 없으니 주의!
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

    // STL_Container에서 메모리 할당을 위해 내부적으로 호출하게될 함수
    T* allocate(size_t _count)
    {
        const int32 size = static_cast<int32>(_count * sizeof(T));
        return static_cast<T*>(PoolAllocator::AllocateMemory(size));
    }

    // STL_Container에서 메모리 해제를 위해 내부적으로 호출하게될 함수
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
