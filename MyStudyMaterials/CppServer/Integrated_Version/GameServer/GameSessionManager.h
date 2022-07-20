#pragma once

class GameSession;


class GameSessionManager
{
public:
	GameSessionManager()
	{
		std::cout << "Debug\n";
	}
	void Add(std::shared_ptr<GameSession> session);
	void Remove(std::shared_ptr<GameSession> session);
	void Broadcast(std::shared_ptr<SendBuffer> sendBuffer);

private:
	USE_LOCK;
	std::set<std::shared_ptr<GameSession>> _sessions;
	//Set<std::shared_ptr<GameSession>> _sessions;
};

extern GameSessionManager GSessionManager;
