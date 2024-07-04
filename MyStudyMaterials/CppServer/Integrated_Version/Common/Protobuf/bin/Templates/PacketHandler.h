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

namespace {{parser.namespace}}
{

using namespace ServerCore;

using PacketHandlerFunc = std::function<bool(std::shared_ptr<PacketSession>&, BYTE*, int32)>;

extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum : uint16
{
{%- for pkt in parser.total_pkt %}
	PKT_{{pkt.name}} = {{pkt.id}},
{%- endfor %}
};

// Custom Handlers
bool Handle_INVALID(std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len);

{%- for pkt in parser.recv_pkt %}
bool Handle_{{pkt.name}}(std::shared_ptr<PacketSession>& session, Protocol::{{pkt.name}}& pkt);
{%- endfor %}

class {{output}}
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;

{%- for pkt in parser.recv_pkt %}
		GPacketHandler[PKT_{{pkt.name}}] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) {
			 return HandlePacket<Protocol::{{pkt.name}}>(Handle_{{pkt.name}}, session, buffer, len); 
		};
{%- endfor %}
	}

	static bool HandlePacket(std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}

{%- for pkt in parser.send_pkt %}
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::{{pkt.name}}& pkt)
	{
		return MakeSendBuffer(pkt, PKT_{{pkt.name}}); 
	}
{%- endfor %}

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