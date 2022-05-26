#include "pch.h"
#include "PlayerManager.h"
#include "ItemManager.h"

PlayerManager GPlayerManager;

void PlayerManager::Player2Item()
{
	WRITE_LOCK;
	GItemManager.Lock();
}

void PlayerManager::Lock()
{
	WRITE_LOCK;
}
