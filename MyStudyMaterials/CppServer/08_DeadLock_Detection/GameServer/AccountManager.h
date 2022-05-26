#pragma once

class AccountManager
{
	USE_LOCK;
public:
	void Account2Player();
	void Lock();
};

extern AccountManager GAccountManager;

