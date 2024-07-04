#pragma once
#include <memory>
#include "../Network/Session.h"
#include "../Core/Types.h"


namespace ServerCore
{


class PacketSession : public Session
{
public:
    PacketSession();
    virtual ~PacketSession() override;

    std::shared_ptr<PacketSession> GetPacketSessionRef();

protected:
    int32 OnRecv(BYTE* buffer, int32 len) override;
    virtual void OnRecvPacket(BYTE* buffer, int32 len);
};
}