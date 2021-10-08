#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "RefCounting.h"
#include "Memory.h"
#include "Allocator.h"

using TL = TypeList<class Player, class Mage, class Knight, class Archer>;

class Player
{
public:
	Player()
	{
		INIT_TL(Player);
	}
	virtual ~Player() { }

	DECLARE_TL
};

class Knight : public Player
{
public:
	Knight() { INIT_TL(Knight); }
};

class Mage : public Player
{

public:
	Mage() { INIT_TL(Mage); }
};

class Archer : public Player
{

public:
	Archer() { INIT_TL(Archer) }
};

class Dog
{

};

int main()
{
	/*Player p1 = new Knight();
	Player p2 = new Mage();*/

	//TypeList<Mage, Knight>::Head whoAMI;
	//TypeList<Mage, Knight>::Tail whoAMI2;

	//TypeList<Mage, TypeList<Knight, Archer>>::Head whoAMI3;
	
	//TypeList<Mage, TypeList<Knight, Archer>>::Tail::Head whoAMI4;
	//TypeList<Mage, TypeList<Knight, Archer>>::Tail::Tail whoAMI5;

	//int32 len1 = Length<TypeList<Mage, Knight>>::value;
	//int32 len2 = Length<TypeList<Mage, Knight, Archer>>::value;
	//
	//
	//// 3*3 
	//TypeAt<TL, 0>::Result whoAMI6;
	//TypeAt<TL, 1>::Result whoAMI7;
	//TypeAt<TL, 2>::Result whoAMI8;

	//int32 index1 = IndexOf<TL, Mage>::value;
	//int32 index2 = IndexOf<TL, Archer>::value;
	//int32 index3 = IndexOf<TL, Dog>::value;

	//bool canConvert1 = Conversion<Player, Knight>::isConversible;
	//bool canConvert2 = Conversion<Knight, Player>::isConversible;
	//bool canConvert3 = Conversion<Knight, Dog>::isConversible;

	{
	Player* player = new Knight();
	//Player* player = new Player();

	if(bool canCast = CanCast<Knight*>(player))
		Knight* knight = TypeCast<Knight*>(player);
		

	delete player;
	}

	{
		shared_ptr<Player> player = MakeShared<Knight>();

		if (bool canCast = CanCast<Knight>(player))
			player = TypeCast<Knight>(player);
		else if (bool canCast = CanCast<Mage>(player))
			player = TypeCast<Mage>(player);
		
	}
	
	//for (int32 i = 0; i < 5; i++)
	//{
	//	GThreadManager->Launch([]()
	//		{
	//			while (true)
	//			{
	//			}
	//		});
	//}

	//GThreadManager->Join();
}