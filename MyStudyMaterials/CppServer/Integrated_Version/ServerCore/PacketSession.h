#pragma once
#include <memory>
#include "Session.h"
#include "Types.h"

/*-----------------
    PacketSession
------------------*/



class PacketSession : public Session
{
public:
    PacketSession();
    ~PacketSession() override;

    std::shared_ptr<PacketSession> GetPacketSessionRef();

protected:
    int32 OnRecv(BYTE* buffer, int32 len) override;
    virtual void OnRecvPacket(BYTE* buffer, int32 len);
};
