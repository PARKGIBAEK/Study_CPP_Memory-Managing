#pragma once
#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <boost/lockfree/queue.hpp>
#include "MemoryHeader.h"

namespace ServerCore
{
// aligned_malloc은 아키텍쳐에 최적화된 동작을 위해 메모리 주소를 alignment의 배수로 맞춰준다
inline void* aligned_malloc(size_t size, size_t alignment)
{
#if defined(_WIN32) || defined(_WIN64)
    return _aligned_malloc(size, alignment);
#else
    void* ptr = nullptr;
    if (posix_memalign(&ptr, alignment, size) != 0) {
        return nullptr;
    }
    return ptr;
#endif
}

inline void aligned_free(void* ptr) {
#if defined(_WIN32) || defined(_WIN64)
    _aligned_free(ptr);
#else
    free(ptr);
#endif
}

class alignas(static_cast<int>(MEMORY_ALIGNMENT::FOR_MEMORY_POOL)) MemoryPool
{
    static constexpr int Q_SIZE = 0x10'000;

    public:
    MemoryPool(int32_t _allocSize)
        : m_queue(Q_SIZE), allocSize(_allocSize)
    {
    }

    ~MemoryPool()
    {
        MemoryHeader* header;
        while (m_queue.pop(header))
        {
            aligned_free(header);
        }
    }

    void Push(MemoryHeader* _ptr)
    {
        _ptr->allocSize = 0; // pool에 복원시켜 주기 위해 0으로 초기화
        m_queue.push(_ptr);

        usedCount.fetch_sub(1);
        reservedCount.fetch_add(1);
    }

    MemoryHeader* Pop()
    {
        /* InterlockedPopEntrySList 함수는
        _header에 가장 최근에 삽입된 데이터를 반환,
        만약 아무런 데이터도 들어있지 않을 경우 nullptr 반환*/
        MemoryHeader* memory;
        if (!m_queue.pop(memory)) // 없으면 새로 할당
        {
            memory = static_cast<MemoryHeader*>(aligned_malloc(
                allocSize, static_cast<int>(MEMORY_ALIGNMENT::FOR_MEMORY_POOL)));
        }
        else
        {
            // ASSERT_CRASH(memory->allocSize == 0)
            reservedCount.fetch_sub(1);
        }
        usedCount.fetch_add(1);
        return memory;
    }

    private:
    boost::lockfree::queue<MemoryHeader*> m_queue;
    int32_t allocSize;
    std::atomic<int32_t> usedCount;
    std::atomic<int32_t> reservedCount;
};
}