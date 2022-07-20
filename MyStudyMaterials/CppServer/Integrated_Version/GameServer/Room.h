#pragma once
#include "JobQueue.h"

class Room : public JobQueue
{
public:
	// 싱글쓰레드 환경인마냥 코딩
	void Enter(std::shared_ptr<class Player> player);
	void Leave(std::shared_ptr<class Player> player);
	void Broadcast(std::shared_ptr<class SendBuffer> sendBuffer);

private:
	map<uint64, std::shared_ptr<class Player>> _players;
};

extern shared_ptr<Room> GRoom;