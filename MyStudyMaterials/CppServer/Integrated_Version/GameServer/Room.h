#pragma once
#include <memory>
#include <map>
#include "Job/JobQueue.h"
#include "Core/Types.h"
#include "Player.h"
#include "Network/SendBuffer.h"

namespace GameServer
{

using namespace ServerCore;

class Room : public JobQueue
{
public:
	virtual ~Room() override {}
	// 싱글쓰레드 환경인마냥 코딩
	void Enter(std::shared_ptr<Player> player);
	void Leave(std::shared_ptr<Player> player);
	void Broadcast(std::shared_ptr<SendBuffer> sendBuffer);

private:
	std::map<uint64, std::shared_ptr<Player>> _players;
};

extern std::shared_ptr<Room> GRoom;
}