#pragma once

class GameSession;

using GameSessionRef = shared_ptr<GameSession>;

class GameSessionManager
{
public:
	// GameSessionRef를 STL컨테이너(set)에 추가
	void Add(GameSessionRef session);
	// GameSessionRef를 STL컨테이너(set)에서 제거
	void Remove(GameSessionRef session);
	// 모든 세션에게 데이터 전송
	void Broadcast(SendBufferRef sendBuffer);

private:
	USE_LOCK;
	Set<GameSessionRef> _sessions;
};

extern GameSessionManager GSessionManager;
