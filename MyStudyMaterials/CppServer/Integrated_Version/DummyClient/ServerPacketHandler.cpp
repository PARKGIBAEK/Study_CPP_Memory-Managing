#include "pch.h"
#include "ServerPacketHandler.h"

namespace DummyClient {
PacketHandlerFunc GPacketHandler[UINT16_MAX];

// 직접 컨텐츠 작업자

bool Handle_INVALID(std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_S_LOGIN(std::shared_ptr<PacketSession>& session, Protocol::S_LOGIN& pkt)
{
	if (pkt.success() == false)
		return true;

	if (pkt.players().size() == 0)
	{
		// 캐릭터 생성창
	}

	// 입장 UI 버튼 눌러서 게임 입장
	Protocol::C_ENTER_GAME enterGamePkt;
	enterGamePkt.set_playerindex(0); // 첫번째 캐릭터로 입장
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
	session->Send(sendBuffer);

	return true;
}

bool Handle_S_ENTER_GAME(std::shared_ptr<PacketSession>& session, Protocol::S_ENTER_GAME& pkt)
{
	// TODO
	return true;
}

bool Handle_S_CHAT(std::shared_ptr<PacketSession>& session, Protocol::S_CHAT& pkt)
{
	std::cout << pkt.msg() << std::endl;
	return true;
}
}