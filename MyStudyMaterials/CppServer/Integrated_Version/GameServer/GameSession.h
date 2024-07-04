#pragma once
#include <memory>
#include "Core/Types.h"
#include "Memory/Container.h"
#include "Network/PacketSession.h"
//#include "Player.h"

namespace GameServer
{


using namespace ServerCore;

class Room;
class Player;

class GameSession : public PacketSession
{
public:
	virtual ~GameSession() override;

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

public:
	Vector<std::shared_ptr<Player>> mPlayers;

	std::shared_ptr<Player> mCurrentPlayer;
	std::weak_ptr<Room> mRoom;
};
}