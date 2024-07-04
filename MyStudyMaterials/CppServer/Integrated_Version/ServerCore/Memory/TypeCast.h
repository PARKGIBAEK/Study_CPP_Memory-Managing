#pragma once
#include "../Core/Types.h"

namespace ServerCore
{
/* TMP (Template Meta Programming) : ���ø� ���ڷ� �� �ٸ� ���ø��� ������ �� �ְ�,
	�̰��� ��������� ������ �� �ִٴ� ���� �̿��ϴ� ����̴� */


#pragma region TypeList
/* 0�� ���ø� : 
	���ø��� ����� �߷��� ���� �Ǵ� �� ���ø�(empty template)�̴�.
   TypeList��� ����� ���ø��� ������ ������ ���ø��� �ش�
   ��, ��� Ż�� ���� ���� ���̶�� ���� �ȴ�.*/
template <typename... T>
struct TypeList;

/* 1�� ���ø� : 
	2���� ���ø� ���ڸ� �޴´�. using Ű����� Head�� Tail�̶�� ��Ī�� ����.
   ( 2�� ���ø��� �Բ� ����Ͽ� 2�� ���ø� ���� T �Ǵ� U �ڸ��� ���ø� ��ͷ� TypeList<T,U>�� ������ ���� )
*/
template <typename T, typename U>
struct TypeList<T, U>
{
    using Head = T;
    using Tail = U;
};

/* 2�� ���ø� :    
   2�� �̻��� ���ø� Ÿ�����ڸ� �޴´�.
   Head�� ���ø� �߷н� ��͸� ���� ��� �Ǿ��� ������ T�� ��Ī�ϸ�,
   Tail�� T�� ������ ������ ���ø� ���ڵ��� ��� �޴� TypeList<U...>�� ��������� ��Ī�Ѵ�. */
template <typename T, typename... U>
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

TypeList<Mage, TypeList<Knight, Archer>>::Tail::Head whoAMI4; // ���ø��� Tail�� TypeList<Knight, Archer>�� ġȯ�ǰ�, Tail::Head �� Knight�� �߷�

TypeList<Mage, TypeList<Knight, Archer>>::Tail::Tail whoAMI5; // ���ø��� Tail�� TypeList<Knight, Archer>�� ġȯ�ǰ�, Tail::Tail�� Archer�� �߷�

*/
#pragma endregion


//���ø��� ��� Ÿ���� ���ڷ� ����ִ����� ��ȯ�ϴ� ���ø�
#pragma region Length

/* 0�� ���ø� : ���ø��� ����� �߷��� ���� �Ǵ� �� ���ø�(empty template)�̴�.
  2�� ���ø����� ����� ���ø� �߷��� �� �� �������� �߷е� ���ø��� �ش�.
  ��, ��� Ż�� ���� ���� ���̶�� ���� �ȴ�.*/
template <typename T>
struct Length;

/* 1�� ���ø� : ���ø� ������ ������ 0���� ��� 0�� ��ȯ
	���ڷ� ���޵Ǵ� ���ø� TypeList�� ������ ������ 0��
*/
template <>
struct Length<TypeList<>>
{
    enum { value = 0 };
};


/* 2�� ���ø� : 1�� �̻��� Ÿ�����ڸ� ������ ���, ������ Ÿ�������� ������ ��ȯ�ϴ� �뵵*/
template <typename T, typename... U>
struct Length<TypeList<T, U...>>
{
    //Length<TypeList<U...>>::value�κ��� ��ͷ� 0�� ���� ��ȯ �� �� ���� �ݺ��Ͽ� ����� �ջ�
    enum { value = 1 + Length<TypeList<U...>>::value };
};

/*
	[ ��� ���� 1 ]

	int32 len1 = Length< TypeList<Mage, Knight> >::value;

	len1 = 2 �� �ȴ�. �߷а����� �Ʒ��� ����.

	1)
	 Length<TypeList<Mage, Knight>>::value���� TypeList�� ù��° Ÿ��Mage��
	 Length<TypeList<T, U...>> ���� T�� �ش��Ѵ�.
	 �׷��� ������ value �� 1 + Length<TypeList<Knight>>::value �� ġȯ�ȴ�.

	2)
	 value�� ��� �κ��� 1�� ������ Length<TypeList<Knight>>::value�� �߷��ؾ� �Ѵ�.
	 �׷��� ���ڰ� 1�� �ۿ� ���� TypeList�� ���ڷ� �޴� Length<TypeList<T>>���� �༮�� ������ �ʴ´�.
	 ��Ȳ���������� �ϳ��� ���ڸ� ������ Length<TypeList<T, U...>> ���� ���� �� �ִ�.
	 ������ U...�� ���ڰ� ���°͵� �����ϱ� �����̴�.( ���ø��� �������� �κ��� ���ڰ� ���°͵� �ش� ��)

	 ���� ���� �Ʒ��� ���� ���ø��� �������� �ʿ䰡 ���� ���̴�. (�������� ���� �Ʒ� ������ �߰��� �������൵ ������ ����)

	 template<typename T>
	 struct Length<TypeList<T>>
	 {
		 enum { value = 1 };
	 };

	�׷� ���� �ϳ��� ���ڸ� ������ ���Ա� ������ value = 1 + Length<TypeList<>>::value�� �ȴ�.

	Length<TypeList<>>::value�� 0�̹Ƿ� �߷� ��ʹ� �����Եȴ�.
	�߷� ����� �ջ��ϸ�  1 + 1 + 0 = 2 �� �ȴ�.




	[ ��� ���� 2 ]

	int32 len2 = Length<TypeList<Mage, Knight, Archer>>::value;
	
	len2�� 3�̵ȴ�. �߷а����� [��� ���� 1]�� �߷а��� ����ϸ�, ��Ͱ� �ѹ� �� �߰��Ǵ� �� ���̴�..

	1) Length<TypeList<Mage, Knight, Archer>>::value ���� Mage�� ���ø� ���� T�� �ش��ϱ� ������ �������,
	   1 + Length<TypeList< Knight, Archer>>::value �� ġȯ�ȴ�.

   2) value ���Ŀ��� ��� 1�� �����ϰ� Length<TypeList< Knight, Archer>>::value�� �ٽ� ����ϸ�,
		Knight�� T�� �ش��ϱ� ������ �������,
		1 + 1 + Length<TypeList<Archer>>::value �� �ȴ�.
		
		��� �κ��� ������ Length<TypeList<Archer>>::value��
		1 + Length<TypeList<>>::value �� ġȯ�ǹǷ�
		1 + 1 + 1 + Length<TypeList<>>::value �� �ȴ�.
		Length<TypeList<>>::value �� 0�̹Ƿ�
		�ջ��ϸ� 1 + 1 + 1 + 0 = 4 ��� ����� ���´�.

*/
#pragma endregion


// index�� �ش��ϴ� Ÿ���� ��ȯ
#pragma region TypeAt
// 0�� ���ø� : TL�� ���ø��� �ޱ� ���� �뵵�̸�, index�� 0�� �� ������ ���ī��Ʈ �뵵
template <typename TL, int32 index>
struct TypeAt;

// 1�� ���ø� : 2�� ���ø� ���¿��� ���ø� ���ڰ� �ϳ��� �پ� 1�� ���ø� ���°� �Ǹ� ���� �տ� �����ִ� Head Ÿ���� ��ȯ, �� index�� 0�� �Ǿ��� ���� Head�� �ش��ϴ� Ÿ���� ��ȯ
template <typename Head, typename... Tail>
struct TypeAt<TypeList<Head, Tail...>, 0>
{
    using Result = Head;
};

// 2�� ���ø� : TypeList���ø��� Ÿ�� ���ڸ� �ϳ� ���ְ� index�� 1�� ���ҽ��� 1�� ���ø��� ���°� �� ������ ���
template <typename Head, typename... Tail, int32 index>
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
template <typename TL, typename T>
struct IndexOf;

// 1�� ���ø� : TypeList�� ù��° Ÿ���� T�� ��ġ�ϴ� ������ ���ø��� �� ��� value�� 0�� �ȴ�
template <typename... Tail, typename T>
struct IndexOf<TypeList<T, Tail...>, T>
{
    enum { value = 0 };
};

// 2�� ���ø� : 3�� ���ø����κ��� ��� �߷��� �����Ͽ� TypeList�� Ÿ�� ���ڰ� ���� ���� ������ ������ ����
template <typename T>
struct IndexOf<TypeList<>, T>
{
    enum { value = -1 };
};

// 3�� ���ø� : temp�� ù��° ���ø� ������ Head�� ������ ���¸� 1, 2�� ���ø� �� �ϳ��� ġȯ ������ ������ ����Ͽ�
template <typename Head, typename... Tail, typename T>
struct IndexOf<TypeList<Head, Tail...>, T>
{
private:
    //ù��° ���� Head�� �����ϰ�, �ι�° ���ڸ� Head�� ����� ����ϴ� ���� 
    enum { temp = IndexOf<TypeList<Tail...>, T>::value };

public:
    // �������� temp�� �߷��ϱ� ���� IndexOf<TypeList<Head, Tail...>, T>�� ����ϸ�, 1�� �Ǵ� 2�� ���ø� ���°� �� ������ value�� 1�� ���Ѵ�. ���� T�� �����Ѵٸ� 1�� ���ø� ���¿��� ���� ���̸� �� ������ ��� �߷��� Ƚ���� value�� �� ���̴�. ���� T�� ã�� ���Ѵٸ� -1�� ��ȯ�ϰ� �� ���̴�.
    enum { value = (temp == -1) ? -1 : temp + 1 };
};

/*

	��� ����)
	 
	using TL = TypeList<Mage,Archer,Knight>;

	int32 index1 = IndexOf<TL, Mage>::value; // 1
	int32 index2 = IndexOf<TL, Archer>::value; // 2
	int32 index3 = IndexOf<TL, Dog>::value; // -1

*/
#pragma endregion


//FromŸ�Կ��� ToŸ������ ��ȯ�� �������� üũ���ִ� ���ø�
#pragma region IsConversible
template <typename From, typename To>
class IsConversible
{
private:
    using Small = __int8;
    using Big = __int32;

    // �Ʒ��� �ΰ���Test�޼��忡�� ��ȯŸ���� ũ�Ⱑ int8���� int32������ �ǹ̸� �ξ���Ѵ�. ���� 0������ ����� �ٸ� Ÿ���� ��ȯ�ϰ� �� ���̴�. ���� ������ �Լ����� ���� Test�Լ��� �߷��� �� �Ű������� ...�� �Ǿ����� ��� �߷� �켱������ ��������. �׸��� �Լ��� �ñ״�ó�� �����ϸ� �ǰ� �ٵ�� ���� �������� �ʾƵ� �ȴ�.
    static Small Test(const To&) { return 0; }
    static Big Test(...) { return 0; } // TMP���� ���Ǵ� ...�̶�� ǥ���� ~�̿��� ��츦 ���Ѵ�.

    //�� �Լ��� ��ȯ�ϴ� Ÿ���� From�̶�� �Ϳ� �ָ�
    static From MakeFrom() { return 0; }

public:
    enum
    {
        /* Test�޼��忡 MakeFrom�Լ��� ��ȯ�� FromŸ���� ���ڷ� �������� �� FromŸ���� ToŸ������ ��ȯ�� �����ϴٸ� ù��° Test�Լ��� ȣ��ȴ�.
         ��ȯ�� �� ���� ��� Big�� ��ȯ�ϴ� Test�޼��尡 ȣ�� �� ���̴�.
         ���� �Ʒ��� sizeof(Test(MakeFrom())) == sizeof(Small) ǥ������ From���� To�� ��ȯ�� ������ ��쿡�� ���̵ȴ�.*/
        isConversible = sizeof(Test(MakeFrom())) == sizeof(Small)
    };
};

/*

	��� ����)

	* Knight�� Player�� ��ӹ��� ���� ��ü�̸�, Dog�� ���� ���þ��� ��ü�̴�.

	class Player {}
	class Knight : public Player {}
	class Dog {}

	bool canConvert1 = IsConversible<Player, Knight>::isConversible; // 0
	bool canConvert2 = IsConversible<Knight, Player>::isConversible; // 1
	bool canConvert3 = IsConversible<Knight, Dog>::isConversible; // 0

*/
#pragma endregion


#pragma region TypeCast

// ������ �ϳ��� Ŭ������ �����ϵ��� ����� ��
template <int32 v>
struct Int2Type
{
    enum { value = v };
};

//�ּ��� �����ִ� ��ȣ ������� ������ �����ϱ� ����
template <typename TL>
class TypeConversion
{
public:
    enum
    {
        // 1. ���ø� ���� ���� �߷�
        length = Length<TL>::value
    };


    TypeConversion()
    {
        MakeTable(Int2Type<0>(), Int2Type<0>());
        /*
           3.

            �Ʒ��� �ڵ忡�� i�� j�� ������ Ÿ�ӿ� ���� �� ���� ������ 
            ���ø� Int2Type �̿��Ͽ� MakeTalbe�Լ��� ���� ���̴�.

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

    /*	4. �Ű����� i�� j�� ���� MakeTable �Լ��� �����ȴ�. �������� ���ø� �Լ��� ������ �� �ִٴ� �ǹ�*/
    template <int32 i, int32 j>
    static void MakeTable(Int2Type<i>, Int2Type<j>)
    {
        using FromType = typename TypeAt<TL, i>::Result;
        using ToType = typename TypeAt<TL, j>::Result;

        if (IsConversible<const FromType*, const ToType*>::isConversible)
            s_convert[i][j] = true; // FromType ���� ToType���� ��ȯ ����
        else
            s_convert[i][j] = false; // FromType ���� ToType���� ��ȯ �Ұ���

        // 5. ���⼭ ��������� ���ø� �Լ��� �����Ѵ�
        MakeTable(Int2Type<i>(), Int2Type<j + 1>());
    }

    /*
      6. �ٷ� ���� MakeTable���ø� �Լ��� ��������� MakeTable�Լ��� �����ϴ� �������� j�� length�� �����ϸ�, j�� 0���� �ʱ�ȭ�ϰ� i�� 1 ������Ų��
     */
    template <int32 i>
    static void MakeTable(Int2Type<i>, Int2Type<length>)
    {
        MakeTable(Int2Type<i + 1>(), Int2Type<0>());
    }

    // 7.  i�� length�� �����ϸ� ���ø� ��͸� ���ߴ� ����
    template <int j>
    static void MakeTable(Int2Type<length>, Int2Type<j>)
    {
    }

    // 8.  CanConvert�� ȣ��Ǹ� TypeConversion ��ü�� �����Ǹ鼭 �����ڿ��� s_convert���̺��� �ڵ����� ä������ �ȴ�.
    static inline bool CanConvert(int32 from, int32 to)
    {
        static TypeConversion conversion; // TL�� ���º��� �ϳ��� ���� �ǵ��� �ϱ� ���� static���� ����
        return s_convert[from][to]; //from���� to�� ��ȯ ���� ���θ� ��ȯ
    }

public:
    /* 
      2.
      length�� ������ Ÿ�ӿ� �߷��� �� �ֱ� ������
      s_convert[length][length]�� ����� ������ Ÿ�ӿ� �߷� �����ϴ�.
     s_convert�� ���� ��� �׷���ó�� from���� to�� ��ȯ�� �������� true/false���θ� ��ȯ����*/
    static bool s_convert[length][length];
};

//static ��� �����̱� ������ �ʱ�ȭ�� �ܺο��� ������Ѵ�
template <typename TL>
bool TypeConversion<TL>::s_convert[length][length];


/* =============================================================================
 ���⼭ ���ʹ� TypeConversion ���ø��� ���ϰ� ����� �� �ִ� �������� ��ɵ��̴�.
==============================================================================*/

/*
	���ø� �׽�Ʈ �� �����ڵ� )
	
	#define DECLARE_TL		using TL = TL; int32 typeId;
	#define INIT_TL(Type)	typeId = IndexOf<TL, Type>::value;

	using TL = TypeList<class Player, class Mage, class Knight, class Archer>;

	class Player
	{
	public:
		Player()
		{
			INIT_TL(Player)  // �����ڿ��� ã�Ƽ� �ڽ��� Type�� ã�Ƽ� typeId�� �Է�
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


// ������ Ÿ�� To���� ������ Ÿ�� From���� ��ȯ�� ���������� Ȯ���ϴ� �޼���(dynamic_cast�� ���ø� �����̶�� �����ϸ� �ȴ�)
template <typename To, typename From>
To TypeCast(From* ptr)
{
    if (ptr == nullptr)
        return nullptr;

    // From�� �ش��ϴ� class�� �ݵ�� TL�� ��������� ��
    using TL = typename From::TL; // typename Ű���带 �ٿ������� type�̶�� �����ϰ� �߷��Ѵ�

    // remove_pointer_t�� ���ø� ���� To�� ���޹��� ������ Ÿ�Կ��� �����͸� ���� Ÿ������ ������ش�
    if (TypeConversion<TL>::CanConvert(ptr->typeId, IndexOf<TL, remove_pointer_t<To>>::value))
        return static_cast<To>(ptr);

    return nullptr;
}


//�� �޼����� shared_ptr ����
template <typename To, typename From>
std::shared_ptr<To> TypeCast(std::shared_ptr<From> ptr)
{
    if (ptr == nullptr)
        return nullptr;

    using TL = typename From::TL;

    if (TypeConversion<TL>::CanConvert(ptr->typeId, IndexOf<TL, remove_pointer_t<To>>::value))
        return std::static_pointer_cast<To>(ptr); //static_pointer_cast�� shared_ptr�� ������� ����ϴ� ����ȯ ���

    return nullptr;
}

/*
	��� ���� )

	std::shared_ptr<Knight> knight = std::make_shared<Knight>(); // knight ����
	std::shared_ptr<Player> player = TypeCast<Player>(knight); // 
	bool canCast = CanCast<Player>(knight);

*/

//����ȯ ���� ���θ� ��ȯ
template <typename To, typename From>
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
    return TypeConversion<TL>::CanConvert(ptr->typeId, IndexOf<TL, remove_pointer_t<To>>::value);
}

//shared_ptr�� ����ȯ ���� ���θ� ��ȯ
template <typename To, typename From>
bool CanCast(std::shared_ptr<From> ptr)
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
    return TypeConversion<TL>::CanConvert(ptr->typeId, IndexOf<TL, remove_pointer_t<To>>::value);
}

#pragma endregion

#define DECLARE_TL		using TL = TL; int32 typeId;
#define INIT_TL(Type)	typeId = IndexOf<TL, Type>::value;
}
