#pragma once
#include <memory>
#include <string>
#include "Core/Types.h"
// #include "Enum.pb.h"
#include "GameSession.h"

namespace GameServer
{

using namespace ServerCore;

class Player
{
public:
	uint64							playerId = 0;
	std::string						name;
	// Protocol::PlayerType			type = Protocol::PLAYER_TYPE_NONE;
	std::shared_ptr<GameSession>	ownerSession; // Cycle
};
}