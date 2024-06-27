#pragma once
#include "JobQueue.h"
#include "Types.h"
#include <memory>
#include <map>

class Player;
class SendBuffer;
class Room : public JobQueue
{
public:
	// �̱۾����� ȯ���θ��� �ڵ�
	void Enter(std::shared_ptr<Player> player);
	void Leave(std::shared_ptr<Player> player);
	void Broadcast(std::shared_ptr<SendBuffer> sendBuffer);

private:
	std::map<uint64, std::shared_ptr<Player>> _players;
};

extern std::shared_ptr<Room> GRoom;