#include "GameSession.h"
#include "GameSessionManager.h"
#include "ClientPacketHandler.h"
#include "PacketHeader.h"
#include "Player.h"
#include "Room.h"
#include <iostream>

GameSession::~GameSession()
{
		std::cout << "~GameSession" << std::endl;
}

void GameSession::OnConnected()
{
	/*
	auto t = GetNetAddress();
	auto ip = t.GetIpAddress();
	auto port = t.GetPort();
	printf("[port : %d][ ip : %s] is connected\n", port, ip);
	*/
	GSessionManager.Add(std::static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::OnDisconnected()
{
	GSessionManager.Remove(std::static_pointer_cast<GameSession>(shared_from_this()));

	if (mCurrentPlayer)
	{
		if (auto room = mRoom.lock())
			room->DoAsync(&Room::Leave, mCurrentPlayer);
	}

	mCurrentPlayer = nullptr;
	mPlayers.clear();
}

void GameSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	std::shared_ptr< PacketSession> session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	// TODO : packetId �뿪 üũ
	ClientPacketHandler::HandlePacket(session, buffer, len);
}

void GameSession::OnSend(int32 len)
{
}