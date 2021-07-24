#pragma once
#include "Types.h"

// ���ø��� �Էµ� Ÿ�Ե� �߿� Ư�� Ÿ���� ã�� ��ȯ
#pragma region TypeList
// 0�� ���ø� : TypeList��� ����ü�� ���ø��� �����ϱ� ���� �������־�� ��
template<typename... T>
struct TypeList;

/* 1�� ���ø� : 2�� ���ø��� �Բ� ����Ͽ� T �Ǵ� U �ڸ��� ��ͷ� TypeList<T,U>�� ���� �� �� �ְ� ���ش�

*/
template<typename T, typename U>
struct TypeList<T, U>
{
	using Head = T;
	using Tail = U;
};

// 2�� ���ø� : 2�� �̻��� ���ø� Ÿ�����ڸ� ���� �� �� �ֵ��� ���ش�
template<typename T, typename... U>
struct TypeList<T, U...>
{
	using Head = T;
	using Tail = TypeList<U...>;
};
/*
	��� ����)
	TypeList<Mage, Knight>::Head whoAMI; // ���ø��� Head�� MageŸ���� ��ȯ
	TypeList<Mage, Knight>::Tail whoAMI2; // ���ø��� Tail�� KnightŸ���� ��ȯ

	TypeList<Mage, TypeList<Knight, Archer>>::Head whoAMI3;  // ���ø��� Head�� MageŸ���� ��ȯ

	TypeList<Mage, TypeList<Knight, Archer>>::Tail::Head whoAMI4; // ���ø��� Tail::Head�� KnightŸ���� ��ȯ
	TypeList<Mage, TypeList<Knight, Archer>>::Tail::Tail whoAMI5; // ���ø��� Tail::Tail�� ArcherŸ���� ��ȯ
*/
#pragma endregion




//���ø��� ��� Ÿ���� ���ڷ� ����ִ����� ��ȯ
#pragma region Length

// 0�� ���ø� : Length��� ����ü�� ���ø��� �����ϱ� ���� �������־�� ��
template<typename T>
struct Length;

// 1�� ���ø� : �ƹ� Ÿ�Ե� �Է����� ���� ��� 0�� ��ȯ
template<>
struct Length<TypeList<>>
{
	enum { value = 0 };
};

// 2�� ���ø� : 1�� �̻��� Ÿ�����ڸ� ������ ��� ������ Ÿ�������� ������ŭ�� ��ȯ
template<typename T, typename... U>
struct Length<TypeList<T, U...>>
{
	//Length<TypeList<U...>>::value�κ��� ��ͷ� 0�� ���� ã�� �� ���� �ݺ��Ͽ� ����� �ջ�
	enum { value = 1 + Length<TypeList<U...>>::value };
};
/*
	��� ����)
	int32 len1 = Length<TypeList<Mage, Knight>>::value; 
	/*  value�� 2�� ��ȯ�ϰԵǸ�, �� ������ ������ ����.
	Length<TypeList<Mage, Knight>>::value���� �Ǿ��� Ÿ��Mage�� value(1)�� ġȯ�Ͽ�
	1 + Length<TypeList<Knight>>::value�� �����.
	�̾ Length<TypeList<Knight>>::value�� 1�� ġȯ�Ͽ� 1+1 �� �����.
	���������� �ƹ� Ÿ�Ե� �Էµ��� ���� ���ø�Length<TypeList<>>::value�� ġȯ�Ǿ� 0���� ġȯ�Ѵ�.
	���� ����� 1 + 1 + 0 = 2 �� �ȴ�.

	int32 len2 = Length<TypeList<Mage, Knight, Archer>>::value; // ���� �������� ��Ͱ� �ѹ� �� �߰��Ǿ� ���ø��� value�� 3�� ��ȯ�ϰԵȴ�

*/
#pragma endregion



// index�� �ش��ϴ� Ÿ���� ��ȯ
#pragma region TypeAt
// 0�� ���ø� : TL�� ���ø��� �ޱ� ���� �뵵�̸�, index�� 0�� �� ������ ���ī��Ʈ �뵵
template<typename TL, int32 index>
struct TypeAt;

// 1�� ���ø� : 2�� ���ø� ���¿��� ���ø� ���ڰ� �ϳ��� �پ� 1�� ���ø� ���°� �Ǹ� ���� �տ� �����ִ� Head Ÿ���� ��ȯ, �� index�� 0�� �Ǿ��� ���� Head�� �ش��ϴ� Ÿ���� ��ȯ
template<typename Head, typename... Tail>
struct TypeAt<TypeList<Head, Tail...>, 0>
{
	using Result = Head;
};

// 2�� ���ø� : TypeList�� ���ø� Ÿ�� ���ڸ� �ϳ� ���ְ� index�� 1 ���ҽ��� 1�� ���ø��� ���°� �� ������ ���
template<typename Head, typename... Tail, int32 index>
struct TypeAt<TypeList<Head, Tail...>, index>
{
	using Result = typename TypeAt<TypeList<Tail...>, index - 1>::Result;
};
/*
	��� ����)
	using TL = TypeList<class Player, class Mage, class Knight, class Archer>;

	TypeAt<TL, 0>::Result whoAmI6; // Player
	TypeAt<TL, 1>::Result whoAmI7; // Mage
	TypeAt<TL, 2>::Result whoAmI8; // Knight
	TypeAt<TL, 3>::Result whoAmI9; // Archer
*/
#pragma endregion



// ���ø� ����Ʈ TL���� T�� ���° �ε��������� ��ȯ
#pragma  region IndexOf
// 0�� ���ø� : TL�� �Ǵٸ� ���ø��� �޴� �뵵, T�� TL�� ����ִ� ���ø� ���� �� �ϳ� ã�� �뵵
template<typename TL, typename T>
struct IndexOf;

// 1�� ���ø� : TypeList�� ù��° Ÿ���� T�� ��ġ�ϴ� ������ ���ø��� �� ��� value�� 0�� �ȴ�
template<typename... Tail, typename T>
struct IndexOf<TypeList<T, Tail...>, T>
{
	enum { value = 0 };
};

// 2�� ���ø� : TypeList�� Ÿ�� ���ڰ� ���� ���� ���±��� ������ ��� -1�� ��ȯ
template<typename T>
struct IndexOf<TypeList<>, T>
{
	enum { value = -1 };
};

// 3�� ���ø� : temp�� ù��° ���ø� ������ Head�� ������ ���¸� 1, 2�� ���ø� �� �ϳ��� ġȯ ������ ������ ����Ͽ�
template<typename Head, typename... Tail, typename T>
struct IndexOf<TypeList<Head, Tail...>, T>
{
private:
	//ù��° ���� Head�� �����ϰ� �ι�° ���ڸ� Head�� ������ ���� IndexOf<TypeList<Head, Tail...>, T>�� ����Ͽ� 1�� �Ǵ� 2�� ���ø� ���°� �� ������ value�� 1�� ���Ѵ�. ���� T�� �����Ѵٸ� 1�� ���ø� ���¿��� ���� ���̰� T�� ã�� ���Ѵٸ� -1�� ��ȯ�ϰ� �� ���̴�.

	enum { temp = IndexOf<TypeList<Tail...>, T>::value };

public:
	enum { value = (temp == -1) ? -1 : temp + 1 };
};

/*

	��� ����)
	int32 index1 = IndexOf<TL, Mage>::value; // 1
	int32 index2 = IndexOf<TL, Archer>::value; // 3
	int32 index3 = IndexOf<TL, Dog>::value; // -1

	*/
#pragma endregion


//FromŸ�Կ��� ToŸ������ ��ȯ�� �������� üũ���ִ� ���ø�
#pragma region Conversion
template<typename From, typename To>
class Conversion
{
private:
	using Small = __int8;
	using Big = __int32;

	// �Ʒ��� �ΰ���Test�޼��忡�� ��ȯŸ���� ũ�Ⱑ int8���� int32������ �ǹ̸� �ξ���Ѵ�. ���� 0������ ����� �ٸ� Ÿ���� ��ȯ�ϰ� �� ���̴�.
	static Small Test(const To&) { return 0; }
	static Big Test(...) { return 0; }// TMP���� ���Ǵ� ...�̶�� ǥ���� ~�̿��� ��츦 ���Ѵ�.

	//�� �Լ��� ��ȯ�ϴ� Ÿ���� From�̶�� �Ϳ� �ָ�
	static From MakeFrom() { return 0; }

public:
	
	enum
	{
		/*Test�޼��忡 MakeFrom�Լ��� ��ȯ�� FromŸ���� ���ڷ� �������� �� FromŸ���� ToŸ������ ��ȯ�� �����ϴٸ� ù��° Test�Լ��� ȣ��ȴ�.
		 ��ȯ�� �� ���� ��� Big�� ��ȯ�ϴ� Test�޼��尡 ȣ�� �� ���̴�.
		 ���� �Ʒ��� sizeof(Test(MakeFrom())) == sizeof(Small) ǥ������ From���� To�� ��ȯ�� ������ ��쿡�� ���̵ȴ�.*/
		isConversible = sizeof(Test(MakeFrom())) == sizeof(Small)
	};
};

/*

	��� ����)

	* Knight�� Player�� ��ӹ��� ���� ��ü�̸�, Dog�� ���� ���þ��� ��ü �� ����� ���
	
	bool canConvert1 = Conversion<Player, Knight>::isConversible; // 0
	bool canConvert2 = Conversion<Knight, Player>::isConversible; // 1
	bool canConvert3 = Conversion<Knight, Dog>::isConversible; // 0

*/
#pragma endregion

#pragma region TypeCast

// � ���� ��ü�� �ϳ��� Ŭ������ �����ϵ��� ����� ��
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
		//���ø� ���� ����
		length = Length<TL>::value
	};

	TypeConversion()
	{
		
		MakeTable(Int2Type<0>(), Int2Type<0>());
		/*
			���� �޼���� �Ʒ��� �ڵ带 ������ Ÿ�ӿ� �м��� �� ���� ������ TMP�� �̿��� ���� �ż����̴�.
			
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
	���� MakeTable�޼��忡�� j���� length�� �Ǿ��� ��
	j�� 0���� �ʱ�ȭ�ϰ� i�� 1 �����Ͽ� �ݺ��ϵ��� ��
	 	  */
	template<int32 i>
	static void MakeTable(Int2Type<i>, Int2Type<length>)
	{
		MakeTable(Int2Type<i + 1>(), Int2Type<0>());
	}

	//i�� length�� �����ϸ� ���ø� ��͸� ���ߴ� ����
	template<int j>
	static void MakeTable(Int2Type<length>, Int2Type<j>)
	{
	}

	// CanConvert�� ȣ��Ǹ� TypeConversion ��ü�� �����Ǹ鼭 �����ڿ��� s_convert���̺��� �ڵ����� ä������ �ȴ�.
	static inline bool CanConvert(int32 from, int32 to)
	{
		static TypeConversion conversion; // TL�� ���º��� �ϳ��� ���� �ǵ��� �ϱ� ���� static���� ����
		return s_convert[from][to]; 
	}

public:
	/*length�� ������ Ÿ�ӿ� �� �� �ֱ� ������ s_convert�� ������ Ÿ�ӿ� �����ȴ�.
	 s_convert�� ���� ��� �׷���ó�� from���� to�� ��ȯ�� �������� true/false���θ� ��ȯ����*/
	static bool s_convert[length][length];
};

template<typename TL>
bool TypeConversion<TL>::s_convert[length][length];

// ������ Ÿ�� To���� ������ Ÿ�� From���� ��ȯ�� ���������� Ȯ���ϴ� �޼���
template<typename To, typename From>
To TypeCast(From* ptr)
{
	if (ptr == nullptr)
		return nullptr;

	using TL = typename From::TL;

	/* remove_pointer_t�� ���ø� ���� To�� ���޹��� ������ Ÿ�Կ��� �����͸� ���� Ÿ������ ������ش�*/
	if (TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, remove_pointer_t<To>>::value))
		return static_cast<To>(ptr);

	return nullptr;
}

//�� �޼����� shared_ptr �����̴�
template<typename To, typename From>
shared_ptr<To> TypeCast(shared_ptr<From> ptr)
{
	if (ptr == nullptr)
		return nullptr;

	using TL = typename From::TL;

	if (TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, remove_pointer_t<To>>::value))
		return static_pointer_cast<To>(ptr);//static_pointer_cast�� shared_ptr�� ����Ϸ��ϴ�  ����ȯ�̴�

	return nullptr;
}

//����ȯ ���� ���θ� ��ȯ
template<typename To, typename From>
bool CanCast(From* ptr)
{
	/*
		��� ����)
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

//shared_ptr�� ����ȯ ���� ���θ� ��ȯ
template<typename To, typename From>
bool CanCast(shared_ptr<From> ptr)
{
	/*
	��� ����)
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
