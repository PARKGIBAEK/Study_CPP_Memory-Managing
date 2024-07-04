#pragma once
#include <functional>
#include <memory>
#include "Network/Session.h"
#include "Protocol.pb.h"
#include "Core/Types.h"
#include "Network/SendBuffer.h"
#include "Core/CoreInitializer.h"
#include "Core/CoreMacro.h"
#include "Network/PacketSession.h"
#include "Network/PacketHeader.h"
#include "Network/SendBufferManager.h"

namespace DummyClient
{

using namespace ServerCore;

using PacketHandlerFunc = std::function<bool(std::shared_ptr<PacketSession>&, BYTE*, int32)>;

extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum : uint16
{
	PKT_C_LOGIN = 1000,
	PKT_S_LOGIN = 1001,
	PKT_C_ENTER_GAME = 1002,
	PKT_S_ENTER_GAME = 1003,
	PKT_C_CHAT = 1004,
	PKT_S_CHAT = 1005,
};

// Custom Handlers
bool Handle_INVALID(std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len);
bool Handle_S_LOGIN(std::shared_ptr<PacketSession>& session, Protocol::S_LOGIN& pkt);
bool Handle_S_ENTER_GAME(std::shared_ptr<PacketSession>& session, Protocol::S_ENTER_GAME& pkt);
bool Handle_S_CHAT(std::shared_ptr<PacketSession>& session, Protocol::S_CHAT& pkt);

class ServerPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[PKT_S_LOGIN] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) {
			 return HandlePacket<Protocol::S_LOGIN>(Handle_S_LOGIN, session, buffer, len); 
		};
		GPacketHandler[PKT_S_ENTER_GAME] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) {
			 return HandlePacket<Protocol::S_ENTER_GAME>(Handle_S_ENTER_GAME, session, buffer, len); 
		};
		GPacketHandler[PKT_S_CHAT] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) {
			 return HandlePacket<Protocol::S_CHAT>(Handle_S_CHAT, session, buffer, len); 
		};
	}

	static bool HandlePacket(std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_LOGIN& pkt)
	{
		return MakeSendBuffer(pkt, PKT_C_LOGIN); 
	}
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_ENTER_GAME& pkt)
	{
		return MakeSendBuffer(pkt, PKT_C_ENTER_GAME); 
	}
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_CHAT& pkt)
	{
		return MakeSendBuffer(pkt, PKT_C_CHAT); 
	}

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static std::shared_ptr<SendBuffer> MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		std::shared_ptr<SendBuffer> sendBuffer = GSendBufferManager->Open(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};
}