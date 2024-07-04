#pragma once
// #include "../Network/ClientService.h"
#include "../Core/Core.h"
#include "../Core/Types.h"
#include <WS2tcpip.h> // SLIST 때문에 넣었음


namespace ServerCore
{
//DECLSPEC_ALIGN(SLIST_ALIGNMENT)
struct alignas(static_cast<int>(ALIGNMENT::SLIST_ALIGNMENT)) MemoryHeader : public SLIST_ENTRY
{
    // [MemoryHeader][Data] => Data영역이 실 사용 메모리 영역이며, 맨앞에 MemoryHeader영역은 메모리 관련 정보를 저장
    MemoryHeader(int32 _size);

    // MemoryHeader영역에 사이즈 기입하기
    static void* AttachHeader(MemoryHeader* _header, int32 _size);

    // 주소를 MemoryHeader 사이즈 만큼 앞으로 당겨서 반환
    static MemoryHeader* DetachHeader(void* _ptr);

    int32 allocSize;
    // 필요한 정보 추가 가능
};
}