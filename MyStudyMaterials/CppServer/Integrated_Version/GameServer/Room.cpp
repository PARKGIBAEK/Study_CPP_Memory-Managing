#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"

shared_ptr<Room> GRoom = std::make_shared<Room>();

void Room::Enter(std::shared_ptr<Player> player)
{
	_players[player->playerId] = player;
}

void Room::Leave(std::shared_ptr<Player> player)
{
	_players.erase(player->playerId);
}

void Room::Broadcast(std::shared_ptr<SendBuffer> sendBuffer)
{
	for (auto& p : _players)
	{
		p.second->ownerSession->Send(sendBuffer);
	}
}