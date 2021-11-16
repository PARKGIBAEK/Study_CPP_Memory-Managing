#pragma once
#include "Types.h"

/* TMP (Template Meta Programming) : 템플릿 인자로 또 다른 템플릿을 전달할 수 있고,
	이것을 재귀적으로 구현할 수 있다는 점을 이용하는 방식이다 */



#pragma region TypeList
/* 0번 템플릿 : 
	템플릿의 재귀적 추론의 끝이 되는 빈 템플릿(empty template)이다.
   TypeList라는 재귀적 템플릿을 정의할 최하위 템플릿에 해당
   즉, 재귀 탈출 조건 같은 것이라고 보면 된다.*/
template<typename... T>
struct TypeList;

/* 1번 템플릿 : 
	2개의 템플릿 인자를 받는다. using 키워드로 Head와 Tail이라는 별칭을 정의.
   ( 2번 템플릿과 함께 사용하여 2번 템플릿 인자 T 또는 U 자리에 템플릿 재귀로 TypeList<T,U>를 전달할 목적 )
*/
template<typename T, typename U>
struct TypeList<T, U>
{
	using Head = T;
	using Tail = U;
};

/* 2번 템플릿 :    
   2개 이상의 템플릿 타입인자를 받는다.
   Head는 템플릿 추론시 재귀를 통해 계속 맨앞의 인자인 T를 지칭하며,
   Tail은 T를 제외한 나머지 템플릿 인자들을 모두 받는 TypeList<U...>를 재귀적으로 지칭한다. */
template<typename T, typename... U>
struct TypeList<T, U...>
{
	using Head = T;
	using Tail = TypeList<U...>;
};

/*
사용 예시)

TypeList<Mage, Knight>::Head whoAMI; // 템플릿의 Head는 Mage타입을 반환

TypeList<Mage, Knight>::Tail whoAMI2; // 템플릿의 Tail은 Knight타입을 반환

TypeList<Mage, TypeList<Knight, Archer>>::Head whoAMI3;  // 템플릿의 Head는 Mage타입을 반환

TypeList<Mage, TypeList<Knight, Archer>>::Tail::Head whoAMI4; // 템플릿의 Tail은 TypeList<Knight, Archer>로 치환되고, Tail::Head 는 Knight로 추론

TypeList<Mage, TypeList<Knight, Archer>>::Tail::Tail whoAMI5; // 템플릿의 Tail은 TypeList<Knight, Archer>로 치환되고, Tail::Tail는 Archer로 추론

*/
#pragma endregion




//템플릿에 몇개의 타입이 인자로 들어있는지를 반환하는 템플릿
#pragma region Length

/* 0번 템플릿 : 템플릿의 재귀적 추론의 끝이 되는 빈 템플릿(empty template)이다.
  2번 템플릿에서 재귀적 템플릿 추론을 할 때 최하위로 추론될 템플릿에 해당.
  즉, 재귀 탈출 조건 같은 것이라고 보면 된다.*/
template<typename T>
struct Length;

/* 1번 템플릿 : 템플릿 인자의 갯수가 0개일 경우 0을 반환
	인자로 전달되는 템플릿 TypeList에 인자의 갯수가 0개
*/
template<>
struct Length<TypeList<>>
{
	enum { value = 0 };
};


/* 2번 템플릿 : 1개 이상의 타입인자를 전달한 경우, 전달한 타입인자의 갯수를 반환하는 용도*/
template<typename T, typename... U>
struct Length<TypeList<T, U...>>
{
	//Length<TypeList<U...>>::value부분이 재귀로 0의 값을 반환 할 때 까지 반복하여 결과를 합산
	enum { value = 1 + Length<TypeList<U...>>::value };
};

/*
	[ 사용 예시 1 ]

	int32 len1 = Length< TypeList<Mage, Knight> >::value;

	len1 = 2 가 된다. 추론과정은 아래와 같다.

	1)
	 Length<TypeList<Mage, Knight>>::value에서 TypeList의 첫번째 타입Mage는
	 Length<TypeList<T, U...>> 에서 T에 해당한다.
	 그렇기 때문에 value 는 1 + Length<TypeList<Knight>>::value 로 치환된다.

	2)
	 value의 상수 부분인 1을 제외한 Length<TypeList<Knight>>::value를 추론해야 한다.
	 그런데 인자가 1개 밖에 없는 TypeList를 인자로 받는 Length<TypeList<T>>같은 녀석이 보이지 않는다.
	 당황스럽겠지만 하나의 인자를 가지고도 Length<TypeList<T, U...>> 으로 들어올 수 있다.
	 이유는 U...은 인자가 없는것도 포함하기 때문이다.( 템플릿의 가변인자 부분은 인자가 없는것도 해당 됨)

	 따라서 굳이 아래과 같은 템플릿을 정의해줄 필요가 없는 것이다. (가독성을 위해 아래 내용을 추가로 정의해줘도 문제는 없음)

	 template<typename T>
	 struct Length<TypeList<T>>
	 {
	 	enum { value = 1 };
	 };

	그럼 이제 하나의 인자를 가지고 들어왔기 때문에 value = 1 + Length<TypeList<>>::value가 된다.

	Length<TypeList<>>::value는 0이므로 추론 재귀는 끝나게된다.
	추론 결과를 합산하면  1 + 1 + 0 = 2 가 된다.




	[ 사용 예시 2 ]

	int32 len2 = Length<TypeList<Mage, Knight, Archer>>::value;
	
	len2는 3이된다. 추론과정은 [사용 예시 1]의 추론과정 비슷하며, 재귀가 한번 더 추가되는 것 뿐이다..

	1) Length<TypeList<Mage, Knight, Archer>>::value 에서 Mage는 템플릿 인자 T에 해당하기 때문에 사라지고,
	   1 + Length<TypeList< Knight, Archer>>::value 로 치환된다.

   2) value 계산식에서 상수 1을 제외하고 Length<TypeList< Knight, Archer>>::value을 다시 계산하면,
		Knight가 T에 해당하기 때문에 사라지고,
		1 + 1 + Length<TypeList<Archer>>::value 가 된다.
		
		상수 부분을 제외한 Length<TypeList<Archer>>::value는
		1 + Length<TypeList<>>::value 로 치환되므로
		1 + 1 + 1 + Length<TypeList<>>::value 이 된다.
		Length<TypeList<>>::value 는 0이므로
		합산하면 1 + 1 + 1 + 0 = 4 라는 결과가 나온다.

*/
#pragma endregion



// index에 해당하는 타입을 반환
#pragma region TypeAt
// 0번 템플릿 : TL은 템플릿을 받기 위한 용도이며, index는 0이 될 때까지 재귀카운트 용도
template<typename TL, int32 index>
struct TypeAt;

// 1번 템플릿 : 2번 템플릿 형태에서 템플릿 인자가 하나씩 줄어 1번 템플릿 형태가 되면 가장 앞에 남아있는 Head 타입을 반환, 즉 index가 0이 되었을 때의 Head에 해당하는 타입을 반환
template<typename Head, typename... Tail>
struct TypeAt<TypeList<Head, Tail...>, 0>
{
	using Result = Head;
};

// 2번 템플릿 : TypeList템플릿의 타입 인자를 하나 없애고 index를 1씩 감소시켜 1번 템플릿의 형태가 될 때까지 재귀
template<typename Head, typename... Tail, int32 index>
struct TypeAt<TypeList<Head, Tail...>, index>
{
	using Result = typename TypeAt<TypeList<Tail...>, index - 1>::Result;
};
/*
	사용 예시)
	using TL = TypeList<class Player, class Mage, class Knight, class Archer>;

	TypeAt<TL, 0>::Result whoAmI6; // Player
	TypeAt<TL, 1>::Result whoAmI7; // Mage
	TypeAt<TL, 2>::Result whoAmI8; // Knight
	TypeAt<TL, 3>::Result whoAmI9; // Archer
*/
#pragma endregion



// 템플릿 리스트 TL에서 T가 몇번째 인덱스인지를 반환
#pragma  region IndexOf
// 0번 템플릿 : TL은 또다른 템플릿을 받는 용도, T는 TL에 들어있는 템플릿 인자 중 하나 찾는 용도
template<typename TL, typename T>
struct IndexOf;

// 1번 템플릿 : TypeList에 첫번째 타입이 T와 일치하는 형태의 템플릿이 될 경우 value는 0이 된다
template<typename... Tail, typename T>
struct IndexOf<TypeList<T, Tail...>, T>
{
	enum { value = 0 };
};

// 2번 템플릿 : 3번 탬플릿으로부터 재귀 추론을 진행하여 TypeList에 타입 인자가 남지 않을 때까지 도달한 형태
template<typename T>
struct IndexOf<TypeList<>, T>
{
	enum { value = -1 };
};

// 3번 템플릿 : temp는 첫번째 템플릿 인자인 Head를 제거한 형태를 1, 2번 템플릿 중 하나로 치환 가능할 때까지 재귀하여
template<typename Head, typename... Tail, typename T>
struct IndexOf<TypeList<Head, Tail...>, T>
{
private:
	//첫번째 인자 Head를 제거하고, 두번째 인자를 Head로 만들어 재귀하는 형태 
	enum { temp = IndexOf<TypeList<Tail...>, T>::value };

public:
	// 최종적인 temp를 추론하기 위해 IndexOf<TypeList<Head, Tail...>, T>로 재귀하며, 1번 또는 2번 탬플릿 형태가 될 때까지 value에 1을 더한다. 만약 T가 존재한다면 1번 탬플릿 형태에서 멈출 것이며 그 때까지 재귀 추론한 횟수가 value가 될 것이다. 만약 T를 찾지 못한다면 -1을 반환하게 될 것이다.
	enum { value = (temp == -1) ? -1 : temp + 1 };
};

/*

	사용 예시)
	 
	using TL = TypeList<Mage,Archer,Knight>;

	int32 index1 = IndexOf<TL, Mage>::value; // 1
	int32 index2 = IndexOf<TL, Archer>::value; // 2
	int32 index3 = IndexOf<TL, Dog>::value; // -1

*/
#pragma endregion
  

//From타입에서 To타입으로 변환이 가능한지 체크해주는 템플릿
#pragma region IsConversible
template<typename From, typename To>
class IsConversible
{
private:
	using Small = __int8;
	using Big = __int32;

	// 아래의 두가지Test메서드에서 반환타입의 크기가 int8인지 int32인지에 의미를 두어야한다. 같은 0이지만 사이즈가 다른 타입을 반환하게 될 것이다. 또한 동일한 함수명을 가진 Test함수를 추론할 때 매개변수가 ...로 되어있을 경우 추론 우선순위가 낮아진다. 그리고 함수의 시그니처만 정의하면 되고 바디는 굳이 정의하지 않아도 된다.
	static Small Test(const To&) { return 0; }
	static Big Test(...) { return 0; }// TMP에서 사용되는 ...이라는 표현은 ~이외의 경우를 말한다.

	//이 함수가 반환하는 타입이 From이라는 것에 주목
	static From MakeFrom() { return 0; }

public:

	enum
	{
		/* Test메서드에 MakeFrom함수가 반환한 From타입을 인자로 전달했을 때 From타입이 To타입으로 변환이 가능하다면 첫번째 Test함수가 호출된다.
		 변환할 수 없는 경우 Big을 반환하는 Test메서드가 호출 될 것이다.
		 따라서 아래의 sizeof(Test(MakeFrom())) == sizeof(Small) 표현식은 From에서 To로 변환이 가능한 경우에만 참이된다.*/
		isConversible = sizeof(Test(MakeFrom())) == sizeof(Small)
	};
};

/*

	사용 예시)

	* Knight는 Player를 상속받은 하위 객체이며, Dog는 전혀 관련없는 객체이다.

	class Player {}
	class Knight : public Player {}
	class Dog {}

	bool canConvert1 = IsConversible<Player, Knight>::isConversible; // 0
	bool canConvert2 = IsConversible<Knight, Player>::isConversible; // 1
	bool canConvert3 = IsConversible<Knight, Dog>::isConversible; // 0

*/
#pragma endregion





#pragma region TypeCast

// 정수를 하나의 클래스로 인지하도록 만들어 줌
template<int32 v>
struct Int2Type
{
	enum { value = v };
};

//주석에 적혀있는 번호 순서대로 읽으면 이해하기 편함
template<typename TL>
class TypeConversion
{
public:
	enum
	{
		// 1. 템플릿 인자 갯수 추론
		length = Length<TL>::value
	};


	TypeConversion()
	{

		MakeTable(Int2Type<0>(), Int2Type<0>());
		/*
		   3.

			아래의 코드에서 i와 j를 컴파일 타임에 정할 수 없기 때문에 
			템플릿 Int2Type 이용하여 MakeTalbe함수를 만든 것이다.

		for (size_t i = 0; i < length; i++)
		{
			for (size_t j = 0; j < length; j++)
			{
				using FromType = typename TypeAt<TL, i>::Result;
				using ToType = typename TypeAt<TL, j>::Result;
				if (IsConversible<const FromType*, const ToType*>::isConversible)
					s_convert[i][j] = true;
				else
					s_convert[i][j] = false;
			}
		}
		*/
	}
	
	/*	4. 매개변수 i와 j에 따라 MakeTable 함수가 생성된다. 여러개의 템플릿 함수가 생성될 수 있다는 의미*/
	template<int32 i, int32 j>
	static void MakeTable(Int2Type<i>, Int2Type<j>)
	{
		using FromType = typename TypeAt<TL, i>::Result;
		using ToType = typename TypeAt<TL, j>::Result;

		if (IsConversible<const FromType*, const ToType*>::isConversible)
			s_convert[i][j] = true; // FromType 에서 ToType으로 변환 가능
		else
			s_convert[i][j] = false; // FromType 에서 ToType으로 변환 불가능

		// 5. 여기서 재귀적으로 템플릿 함수를 생성한다
		MakeTable(Int2Type<i>(), Int2Type<j + 1>());
	}
	/*
	  6. 바로 위의 MakeTable템플릿 함수가 재귀적으로 MakeTable함수를 생성하는 과정에서 j가 length에 도달하면, j는 0으로 초기화하고 i를 1 증가시킨다
	 */
	template<int32 i>
	static void MakeTable(Int2Type<i>, Int2Type<length>)
	{
		MakeTable(Int2Type<i + 1>(), Int2Type<0>());
	}

	// 7.  i가 length에 도달하면 템플릿 재귀를 멈추는 역할
	template<int j>
	static void MakeTable(Int2Type<length>, Int2Type<j>)
	{
	}

	// 8.  CanConvert가 호출되면 TypeConversion 객체가 생성되면서 생성자에서 s_convert테이블이 자동으로 채워지게 된다.
	static inline bool CanConvert(int32 from, int32 to)
	{
		static TypeConversion conversion; // TL의 형태별로 하나만 생성 되도록 하기 위해 static으로 선언
		return s_convert[from][to];//from에서 to로 변환 가능 여부를 반환
	}

public:
	/* 
	  2.
	  length를 컴파일 타임에 추론할 수 있기 때문에
	  s_convert[length][length]의 사이즈도 컴파일 타임에 추론 가능하다.
	 s_convert는 인접 행렬 그래프처럼 from에서 to로 변환이 가능한지 true/false여부를 반환해줌*/
	static bool s_convert[length][length];
};

//static 멤버 변수이기 때문에 초기화를 외부에서 해줘야한다
template<typename TL>
bool TypeConversion<TL>::s_convert[length][length];




/* =============================================================================
 여기서 부터는 TypeConversion 템플릿을 편하게 사용할 수 있는 여러가지 기능들이다.
==============================================================================*/

/*
	템플릿 테스트 용 샘플코드 )
	
	#define DECLARE_TL		using TL = TL; int32 typeId;
	#define INIT_TL(Type)	typeId = IndexOf<TL, Type>::value;

	using TL = TypeList<class Player, class Mage, class Knight, class Archer>;

	class Player
	{
	public:
		Player()
		{
			INIT_TL(Player)  // 생성자에서 찾아서 자신의 Type을 찾아서 typeId를 입력
		}
		virtual ~Player() { }

		DECLARE_TL
	};

	class Knight : public Player
	{
	public:
		Knight() { INIT_TL(Knight) }
	};

	class Mage : public Player
	{

	public:
		Mage() { INIT_TL(Mage) }
	};

	class Archer : public Player
	{

	public:
		Archer() { INIT_TL(Archer) }
	};

*/


// 포인터 타입 To에서 포인터 타입 From으로 변환이 가능한지를 확인하는 메서드(dynamic_cast의 템플릿 버전이라고 생각하면 된다)
template<typename To, typename From>
To TypeCast(From* ptr)
{
	if (ptr == nullptr)
		return nullptr;
	
	// From에 해당하는 class에 반드시 TL을 정의해줘야 함
	using TL = typename From::TL;// typename 키워드를 붙여야지만 type이라고 생각하고 추론한다

	// remove_pointer_t는 템플릿 인자 To로 전달받은 포인터 타입에서 포인터를 없앤 타입으로 만들어준다
	if (TypeConversion<TL>::CanConvert(ptr->typeId, IndexOf<TL, remove_pointer_t<To>>::value))
		return static_cast<To>(ptr);

	return nullptr;
}


//위 메서드의 shared_ptr 버전
template<typename To, typename From>
std::shared_ptr<To> TypeCast(std::shared_ptr<From> ptr)
{
	if (ptr == nullptr)
		return nullptr;

	using TL = typename From::TL;

	if (TypeConversion<TL>::CanConvert(ptr->typeId, IndexOf<TL, remove_pointer_t<To>>::value))
		return static_pointer_cast<To>(ptr);//static_pointer_cast는 shared_ptr를 대상으로 사용하는 형변환 기능

	return nullptr;
}
/*
	사용 예제 )

	std::shared_ptr<Knight> knight = std::make_shared<Knight>(); // knight 생성
	std::shared_ptr<Player> player = TypeCast<Player>(knight); // 
	bool canCast = CanCast<Player>(knight);

*/

//형변환 가능 여부를 반환
template<typename To, typename From>
bool CanCast(From* ptr)
{
	/*
		사용 예시)
	Player* player = new Knight();
	//Player* player = new Player();

	if(bool canCast = CanCast<Knight*>(player))
		Knight* knight = TypeCast<Knight*>(player);

	delete player;
	*/
	if (ptr == nullptr)
		return false;

	using TL = typename From::TL;
	return TypeConversion<TL>::CanConvert(ptr->typeId, IndexOf<TL, remove_pointer_t<To>>::value);
}

//shared_ptr의 형변환 가능 여부를 반환
template<typename To, typename From>
bool CanCast(shared_ptr<From> ptr)
{
	/*
	사용 예시)
	shared_ptr<Player> player = MakeShared<Knight>();

	if (bool canCast = CanCast<Knight>(player))
		player = TypeCast<Knight>(player);
	else if (bool canCast = CanCast<Mage>(player))
		player = TypeCast<Mage>(player);
	*/
	if (ptr == nullptr)
		return false;

	using TL = typename From::TL;
	return TypeConversion<TL>::CanConvert(ptr->typeId, IndexOf<TL, remove_pointer_t<To>>::value);
}

#pragma endregion

#define DECLARE_TL		using TL = TL; int32 typeId;
#define INIT_TL(Type)	typeId = IndexOf<TL, Type>::value;
