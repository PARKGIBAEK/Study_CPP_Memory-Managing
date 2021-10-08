#pragma once

class GameSession;

using GameSessionRef = shared_ptr<GameSession>;

class GameSessionManager
{
public:
	// GameSessionRef�� STL�����̳�(set)�� �߰�
	void Add(GameSessionRef session);
	// GameSessionRef�� STL�����̳�(set)���� ����
	void Remove(GameSessionRef session);
	// ��� ���ǿ��� ������ ����
	void Broadcast(SendBufferRef sendBuffer);

private:
	USE_LOCK;
	Set<GameSessionRef> _sessions;
};

extern GameSessionManager GSessionManager;
