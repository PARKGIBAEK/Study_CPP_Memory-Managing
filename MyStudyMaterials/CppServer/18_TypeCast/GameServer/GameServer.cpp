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

// 템플릿에서 using TL = typename From::TL; 이라는 코드가 있기 때문에 TL을 클래스 내부에서 가지고 있어야 함
using TL = TypeList<class Player, class Mage, class Knight, class Archer>;

class Player
{
public:
	Player()
	{// 생성자에서 찾아서 자신의 Type을 찾아서 typeId를 입력
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


template<typename... T>
struct MyTemplate;

template< typename T, typename U>
struct MyTemplate<T, U>
{
	using Head = T;
	using Tail = U;
};

template<typename T, typename... U>
struct MyTemplate<T, U...>
{
	using RecursiveTemplate = MyTemplate<U...>;
};

//위와 같이 정의된 템플릿이 있을 때
struct A;
struct B;
struct C;


MyTemplate<MyTemplate< A, B, C>::RecursiveTemplate >;
// 위 코드는 MyTemplate<MyTemplate<B, C>> 와 같다
//즉, 가변인자 템플릿에 해당하는 U...부분은 타입인자만 올수 있는 것이 아니라 MyTemplate<B, C>과 같은 템플릿이 올 수도 있다는 것이다.


int main()
{
	/*Player p1 = new Knight();
	Player p2 = new Mage();*/

	//TypeList<Mage, Knight>::Head whoAMI;
	//TypeList<Mage, Knight>::Tail whoAMI2;

	//TypeList<Mage, TypeList<Knight, Archer>>::Head whoAMI3;

	/*TypeList<Mage, TypeList<Knight, Archer>>::Tail::Head whoAMI4;
	TypeList<Mage, TypeList<Knight, Archer>>::Tail::Tail whoAMI5;

	int32 len1 = Length<TypeList<Mage, Knight>>::value;
	int32 len2 = Length<TypeList<Mage, Knight, Archer>>::value;*/
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

		if (bool canCast = CanCast<Knight*>(player))
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