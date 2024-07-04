#pragma once
#include <memory>
#include <set>
#include "Core/CoreMacro.h"
#include "Network/SendBuffer.h"

namespace GameServer
{

using namespace ServerCore;

class GameSession;


class GameSessionManager
{
public:
	GameSessionManager();
	void Add(std::shared_ptr<GameSession> session);
	void Remove(std::shared_ptr<GameSession> session);
	void Broadcast(std::shared_ptr<SendBuffer> sendBuffer);

private:
	USE_LOCK;
	std::set<std::shared_ptr<GameSession>> _sessions;
};

extern GameSessionManager GSessionManager;
}