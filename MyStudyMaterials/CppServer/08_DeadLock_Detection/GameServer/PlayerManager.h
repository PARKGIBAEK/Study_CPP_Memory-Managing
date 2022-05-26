#pragma once

class PlayerManager
{
	USE_LOCK;
public:
	void Player2Item();
	void Lock();
};

extern PlayerManager GPlayerManager;