#pragma once
#include "../Core/Types.h"
#include <WS2tcpip.h> // SLIST 때문에 넣었음


namespace ServerCore
{
struct MemoryHeader;
/*-----------------
	MemoryPool
------------------*/

class alignas(static_cast<int>(ALIGNMENT::SLIST_ALIGNMENT)) MemoryPool
{
	public:
	MemoryPool(int32 _allocSize);
	~MemoryPool();

	void				Push(MemoryHeader* _ptr);
	MemoryHeader*		Pop();

	private:
	SLIST_HEADER		header;
	int32				allocSize;
	std::atomic<int32>	usedCount;
	std::atomic<int32>	reservedCount;
};
}