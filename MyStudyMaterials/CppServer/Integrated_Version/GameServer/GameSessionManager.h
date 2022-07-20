#pragma once

class GameSession;

//using GameSessionRef = shared_ptr<GameSession>;

class GameSessionManager
{
public:
	GameSessionManager()
	{
		std::cout << "Debug\n";
	}
	void Add(GameSessionRef session);
	void Remove(GameSessionRef session);
	void Broadcast(SendBufferRef sendBuffer);

private:
	USE_LOCK;
	std::set<GameSessionRef> _sessions;
	//Set<GameSessionRef> _sessions;
};

extern GameSessionManager GSessionManager;
