#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "PlayerManager.h"
#include "AccountManager.h"
#include "ItemManager.h"

int main()
{

	// 데드락 걸기
	// Account -> Player -> Item -> Account
	GThreadManager->Launch([=]
		{
			while (true)
			{
				cout << "Account -> Player" << '\n';
				GAccountManager.Account2Player();
				this_thread::sleep_for(100ms);
			}
		}
	);

	GThreadManager->Launch([=]
		{
			while (true)
			{
				cout << "Player -> Item" << '\n';
				GPlayerManager.Player2Item();
				this_thread::sleep_for(100ms);
			}
		}
	);

	GThreadManager->Launch([=]
		{
			while (true)
			{
				cout << "Item -> Account" << '\n';
				GItemManager.Item2Account();
				this_thread::sleep_for(100ms);
			}
		}
	);


	GThreadManager->Join();
}