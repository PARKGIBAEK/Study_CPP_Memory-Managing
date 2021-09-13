#pragma once
#include "Types.h"

// 템플릿에 입력된 타입들 중에 특정 타입을 찾아 반환
#pragma region TypeList
// 0번 템플릿 : TypeList라는 구조체로 템플릿을 정의하기 전에 구현해주어야 함
template<typename... T>
struct TypeList;

/* 1번 템플릿 : 2번 템플릿과 함께 사용하여 T 또는 U 자리에 재귀로 TypeList<T,U>를 정의 할 수 있게 해준다

*/
template<typename T, typename U>
struct TypeList<T, U>
{
	using Head = T;
	using Tail = U;
};

// 2번 템플릿 : 2개 이상의 템플릿 타입인자를 전달 할 수 있도록 해준다
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

	TypeList<Mage, TypeList<Knight, Archer>>::Tail::Head whoAMI4; // 템플릿의 Tail::Head는 Knight타입을 반환
	TypeList<Mage, TypeList<Knight, Archer>>::Tail::Tail whoAMI5; // 템플릿의 Tail::Tail는 Archer타입을 반환
*/
#pragma endregion




//템플릿에 몇개의 타입이 인자로 들어있는지를 반환
#pragma region Length

// 0번 템플릿 : Length라는 구조체로 템플릿을 정의하기 전에 구현해주어야 함
template<typename T>
struct Length;

// 1번 템플릿 : 아무 타입도 입력하지 않은 경우 0을 반환
template<>
struct Length<TypeList<>>
{
	enum { value = 0 };
};

// 2번 템플릿 : 1개 이상의 타입인자를 전달한 경우 전달한 타입인자의 갯수만큼을 반환
template<typename T, typename... U>
struct Length<TypeList<T, U...>>
{
	//Length<TypeList<U...>>::value부분이 재귀로 0의 값을 찾을 때 까지 반복하여 결과를 합산
	enum { value = 1 + Length<TypeList<U...>>::value };
};
/*
	사용 예시)
	int32 len1 = Length<TypeList<Mage, Knight>>::value; 
	/*  value는 2를 반환하게되며, 그 과정은 다음과 같다.
	Length<TypeList<Mage, Knight>>::value에서 맨앞의 타입Mage를 value(1)로 치환하여
	1 + Length<TypeList<Knight>>::value로 만든다.
	이어서 Length<TypeList<Knight>>::value를 1로 치환하여 1+1 로 만든다.
	마지막으로 아무 타입도 입력되지 않은 템플릿Length<TypeList<>>::value로 치환되어 0으로 치환한다.
	최종 결과는 1 + 1 + 0 = 2 가 된다.

	int32 len2 = Length<TypeList<Mage, Knight, Archer>>::value; // 위의 과정에서 재귀가 한번 더 추가되어 템플릿의 value는 3를 반환하게된다

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

// 2번 템플릿 : TypeList의 템플릿 타입 인자를 하나 없애고 index를 1 감소시켜 1번 템플릿의 형태가 될 때까지 재귀
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

// 2번 템플릿 : TypeList에 타입 인자가 남지 않은 형태까지 도달한 경우 -1을 반환
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
	//첫번째 인자 Head를 제거하고 두번째 인자를 Head로 가지는 형태 IndexOf<TypeList<Head, Tail...>, T>로 재귀하여 1번 또는 2번 탬플릿 형태가 될 때까지 value에 1을 더한다. 만약 T가 존재한다면 1번 탬플릿 형태에서 멈출 것이고 T를 찾지 못한다면 -1을 반환하게 될 것이다.

	enum { temp = IndexOf<TypeList<Tail...>, T>::value };

public:
	enum { value = (temp == -1) ? -1 : temp + 1 };
};

/*

	사용 예시)
	int32 index1 = IndexOf<TL, Mage>::value; // 1
	int32 index2 = IndexOf<TL, Archer>::value; // 3
	int32 index3 = IndexOf<TL, Dog>::value; // -1

	*/
#pragma endregion


//From타입에서 To타입으로 변환이 가능한지 체크해주는 템플릿
#pragma region Conversion
template<typename From, typename To>
class Conversion
{
private:
	using Small = __int8;
	using Big = __int32;

	// 아래의 두가지Test메서드에서 반환타입의 크기가 int8인지 int32인지에 의미를 두어야한다. 같은 0이지만 사이즈가 다른 타입을 반환하게 될 것이다.
	static Small Test(const To&) { return 0; }
	static Big Test(...) { return 0; }// TMP에서 사용되는 ...이라는 표현은 ~이외의 경우를 말한다.

	//이 함수가 반환하는 타입이 From이라는 것에 주목
	static From MakeFrom() { return 0; }

public:
	
	enum
	{
		/*Test메서드에 MakeFrom함수가 반환한 From타입을 인자로 전달했을 때 From타입이 To타입으로 변환이 가능하다면 첫번째 Test함수가 호출된다.
		 변환할 수 없는 경우 Big을 반환하는 Test메서드가 호출 될 것이다.
		 따라서 아래의 sizeof(Test(MakeFrom())) == sizeof(Small) 표현식은 From에서 To로 변환이 가능한 경우에만 참이된다.*/
		isConversible = sizeof(Test(MakeFrom())) == sizeof(Small)
	};
};

/*

	사용 예시)

	* Knight는 Player를 상속받은 하위 객체이며, Dog는 전혀 관련없는 객체 일 경우의 결과
	
	bool canConvert1 = Conversion<Player, Knight>::isConversible; // 0
	bool canConvert2 = Conversion<Knight, Player>::isConversible; // 1
	bool canConvert3 = Conversion<Knight, Dog>::isConversible; // 0

*/
#pragma endregion

#pragma region TypeCast

// 어떤 숫자 자체를 하나의 클래스로 인지하도록 만들어 줌
template<int32 v>
struct Int2Type
{
	enum { value = v };
};

template<typename TL>
class TypeConversion
{
public:
	enum
	{
		//템플릿 인자 갯수
		length = Length<TL>::value
	};

	TypeConversion()
	{
		
		MakeTable(Int2Type<0>(), Int2Type<0>());
		/*
			위의 메서드는 아래의 코드를 컴파일 타임에 분석할 수 없기 때문에 TMP를 이용해 만든 매서드이다.
			
		for (size_t i = 0; i < length; i++)
		{
			for (size_t j = 0; j < length; j++)
			{
				using FromType = typename TypeAt<TL, i>::Result;
				using ToType = typename TypeAt<TL, j>::Result;
				if (Conversion<const FromType*, const ToType*>::isConversible)
					s_convert[i][j] = true;
				else
					s_convert[i][j] = false;
			}
		}
		*/
	}

	template<int32 i, int32 j>
	static void MakeTable(Int2Type<i>, Int2Type<j>)
	{
		using FromType = typename TypeAt<TL, i>::Result;
		using ToType = typename TypeAt<TL, j>::Result;

		if (Conversion<const FromType*, const ToType*>::isConversible)
			s_convert[i][j] = true;
		else
			s_convert[i][j] = false;

		MakeTable(Int2Type<i>(), Int2Type<j + 1>());
	}
	/*
	위의 MakeTable메서드에서 j값이 length가 되었을 때
	j는 0으로 초기화하고 i를 1 증가하여 반복하도록 함
	 	  */
	template<int32 i>
	static void MakeTable(Int2Type<i>, Int2Type<length>)
	{
		MakeTable(Int2Type<i + 1>(), Int2Type<0>());
	}

	//i가 length에 도달하면 템플릿 재귀를 멈추는 역할
	template<int j>
	static void MakeTable(Int2Type<length>, Int2Type<j>)
	{
	}

	// CanConvert가 호출되면 TypeConversion 객체가 생성되면서 생성자에서 s_convert테이블이 자동으로 채워지게 된다.
	static inline bool CanConvert(int32 from, int32 to)
	{
		static TypeConversion conversion; // TL의 형태별로 하나만 생성 되도록 하기 위해 static으로 선언
		return s_convert[from][to]; 
	}

public:
	/*length가 컴파일 타임에 알 수 있기 때문에 s_convert도 컴파일 타임에 결정된다.
	 s_convert는 인접 행렬 그래프처럼 from에서 to로 변환이 가능한지 true/false여부를 반환해줌*/
	static bool s_convert[length][length];
};

template<typename TL>
bool TypeConversion<TL>::s_convert[length][length];

// 포인터 타입 To에서 포인터 타입 From으로 변환이 가능한지를 확인하는 메서드
template<typename To, typename From>
To TypeCast(From* ptr)
{
	if (ptr == nullptr)
		return nullptr;

	using TL = typename From::TL;

	/* remove_pointer_t는 템플릿 인자 To로 전달받은 포인터 타입에서 포인터를 없앤 타입으로 만들어준다*/
	if (TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, remove_pointer_t<To>>::value))
		return static_cast<To>(ptr);

	return nullptr;
}

//위 메서드의 shared_ptr 버전이다
template<typename To, typename From>
shared_ptr<To> TypeCast(shared_ptr<From> ptr)
{
	if (ptr == nullptr)
		return nullptr;

	using TL = typename From::TL;

	if (TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, remove_pointer_t<To>>::value))
		return static_pointer_cast<To>(ptr);//static_pointer_cast는 shared_ptr을 대상하로하는  형변환이다

	return nullptr;
}

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
	return TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, remove_pointer_t<To>>::value);
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
	return TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, remove_pointer_t<To>>::value);
}

#pragma endregion

#define DECLARE_TL		using TL = TL; int32 _typeId;
#define INIT_TL(Type)	_typeId = IndexOf<TL, Type>::value;
