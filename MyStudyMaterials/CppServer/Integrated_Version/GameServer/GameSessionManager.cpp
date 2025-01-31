#include "GameSession.h"
#include "Network/SendBuffer.h"
#include "GameSessionManager.h"
#include "Network/Session.h"
#include <iostream>

namespace GameServer
{

GameSessionManager GSessionManager;

GameSessionManager::GameSessionManager()
{
	std::cout << "Debug\n";
}

void GameSessionManager::Add(std::shared_ptr<GameSession> session)
{
	WRITE_LOCK;
	_sessions.insert(session);
}

void GameSessionManager::Remove(std::shared_ptr<GameSession> session)
{
	WRITE_LOCK;
	_sessions.erase(session);
}

void GameSessionManager::Broadcast(std::shared_ptr<SendBuffer> sendBuffer)
{
	WRITE_LOCK;
	for (std::shared_ptr<GameSession> session : _sessions)
	{
		session->Send(sendBuffer);
	}
}
}