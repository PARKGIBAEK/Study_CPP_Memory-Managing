#pragma once
#include "Protocol.pb.h"
#include "ServerPacketHandler.h"

namespace DummyClient
{

class ServerSession : public PacketSession
{
public:
    ~ServerSession()
    {
        std::cout << "~ServerSession" << std::endl;
    }

    virtual void OnConnected() override
    {
        Protocol::C_LOGIN pkt;
        auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
        Send(sendBuffer);
    }

    virtual void OnRecvPacket(BYTE* buffer, int32 len) override
    {
        std::shared_ptr<PacketSession> session = GetPacketSessionRef();
        PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

        // TODO : packetId 대역 체크
        ServerPacketHandler::HandlePacket(session, buffer, len);
    }

    virtual void OnSend(int32 len) override
    {
        std::cout << "OnSend Len = " << len << std::endl;
    }

    virtual void OnDisconnected() override
    {
        std::cout << "Disconnected" << std::endl;
    }
};
}
