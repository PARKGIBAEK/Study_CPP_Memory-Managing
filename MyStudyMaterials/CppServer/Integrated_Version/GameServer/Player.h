#pragma once
#include <memory>
#include <string>
#include "Types.h"
#include "Enum.pb.h"
class GameSession;

class Player
{
public:
	uint64							playerId = 0;
	std::string						name;
	Protocol::PlayerType			type = Protocol::PLAYER_TYPE_NONE;
	std::shared_ptr<GameSession>	ownerSession; // Cycle
};

