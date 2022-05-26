#pragma once

class ItemManager
{
	USE_LOCK;
public:
	void Item2Account();
	void Lock();
};

extern ItemManager GItemManager;