#include "ClientPacketHandler.h"
#include "Player.h"
#include "Room.h"
#include "GameSession.h"
#include "Memory/MemoryManager.h"


namespace GameServer
{

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_C_LOGIN(std::shared_ptr<PacketSession>& session, Protocol::C_LOGIN& pkt)
{
	std::shared_ptr<GameSession> gameSession = std::static_pointer_cast<GameSession>(session);

	// TODO : Validation 체크

	Protocol::S_LOGIN loginPkt;
	loginPkt.set_success(true);

	// DB에서 플레이어 데이터를 가져오기
	// GameSession에 플레이어 데이터 추가 (메모리)

	// ID 생성 (DB 아이디가 아니고, 게임 아이디)
	static std::atomic<uint64> idGenerator = 1;

	{
		Protocol::Player* player = loginPkt.add_players();
		// player->mutable_name()->assign("DB_extracted_name");
		// player->set_playertype(Protocol::PLAYER_TYPE_KNIGHT);
		std::shared_ptr<Player> playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		// playerRef->name = player->name();
		// playerRef->type = player->playertype();
		playerRef->ownerSession = gameSession;

		gameSession->mPlayers.push_back(playerRef);
	}

	{
		auto player = loginPkt.add_players();
		// player->set_name(u8"DB_extracted_name_2");
		//player->set_playertype(Protocol::PLAYER_TYPE_MAGE);

		std::shared_ptr<Player> playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		// playerRef->name = player->name();
		// playerRef->type = player->playertype();
		playerRef->ownerSession = gameSession;

		gameSession->mPlayers.push_back(playerRef);
	}

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C_ENTER_GAME(std::shared_ptr<PacketSession>& session, Protocol::C_ENTER_GAME& pkt)
{
	std::shared_ptr<GameSession> gameSession = std::static_pointer_cast<GameSession>(session);

	uint64 index = pkt.playerindex();
	// TODO : Validation

	gameSession->mCurrentPlayer = gameSession->mPlayers[index]; // READ_ONLY?
	gameSession->mRoom = GRoom;

	GRoom->DoAsync(&Room::Enter, gameSession->mCurrentPlayer);

	Protocol::S_ENTER_GAME enterGamePkt;
	enterGamePkt.set_success(true);
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGamePkt);
	gameSession->mCurrentPlayer->ownerSession->Send(sendBuffer);

	return true;
}

bool Handle_C_CHAT(std::shared_ptr<PacketSession>& session, Protocol::C_CHAT& pkt)
{
	std::cout << pkt.msg() << std::endl;

	Protocol::S_CHAT chatPkt;
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);

	GRoom->DoAsync(&Room::Broadcast, sendBuffer);

	return true;
}
}