#include "pch.h"
#include "ItemManager.h"
#include "AccountManager.h"

ItemManager GItemManager;

void ItemManager::Item2Account()
{
	WRITE_LOCK;
	GAccountManager.Lock();
}

void ItemManager::Lock()
{
	WRITE_LOCK;
}
