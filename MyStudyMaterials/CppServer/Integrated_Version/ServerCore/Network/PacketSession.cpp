#include "PacketSession.h"

#include "../Network/PacketHeader.h"

namespace ServerCore
{

PacketSession::PacketSession()
{
}

PacketSession::~PacketSession()
{
}

std::shared_ptr<PacketSession> PacketSession::GetPacketSessionRef()
{
    return std::static_pointer_cast<PacketSession>(shared_from_this());
}

// [size(2)][id(2)][data....][size(2)][id(2)][data....]
int32 PacketSession::OnRecv(BYTE* buffer, int32 len)
{
    int32 processedLen = 0;

    while (true)
    {
        int32 dataSize = len - processedLen;
        // 최소한 헤더는 파싱할 수 있어야 한다
        if (dataSize < sizeof(PacketHeader))
            break;

        PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[processedLen]));

        // 파싱할 수 있을 만큼의 길의 패킷을 수신하지 않은 경우 종료(헤더에 기록된 패킷 크기 만큼 파싱할 수 있어야 함)
        if (dataSize < header.size)
            break;

        // 패킷 추출 & PacketHandler로 전달하여 패킷 처리
        OnRecvPacket(&buffer[processedLen], header.size);

        processedLen += header.size;
    }

    return processedLen;
}

void PacketSession::OnRecvPacket(BYTE* buffer, int32 len)
{
}
}