#pragma once
#include <memory>
#include "Types.h"
#include "Container.h"
#include "PacketSession.h"

class Player;
class Room;
class GameSession : public PacketSession
{
public:
	~GameSession() override;

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

public:
	Vector<std::shared_ptr<Player>> mPlayers;

	std::shared_ptr<Player> mCurrentPlayer;
	std::weak_ptr<Room> mRoom;
};