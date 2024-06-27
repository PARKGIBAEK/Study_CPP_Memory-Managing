#pragma once
#include "CoreMacro.h"
#include "DBConnection.h"

template<int32 C>
struct FullBits {
	enum { value = (1 << (C - 1)) | FullBits<C - 1>::value };
};

template<>
struct FullBits<1> { enum { value = 1 }; };

template<>
struct FullBits<0> { enum { value = 0 }; };


template<int32 ParamCount, int32 ColumnCount>
class DBBind
{
public:
	DBBind(DbConnection& dbConnection, const WCHAR* query)
		: _dbConnection(dbConnection), _query(query)
	{
		::memset(_paramIndex, 0, sizeof(_paramIndex));
		::memset(_columnIndex, 0, sizeof(_columnIndex));
		_paramFlag = 0;
		_columnFlag = 0;
		dbConnection.Unbind();
	}

	bool Validate()
	{
		return (_paramFlag == FullBits<ParamCount>::value) &&
				(_columnFlag == FullBits<ColumnCount>::value);
	}

	bool Execute()
	{
		ASSERT_CRASH(Validate());
		return _dbConnection.Execute(_query);
	}

	bool Fetch()
	{
		return _dbConnection.Fetch();
	}

public:

	/* ���������� �� ���ڿ��� ?�κ��� ��ü�� ���ڸ� ���ε� */
	template<typename T>
	void BindParam(int32 idx, T& value)
	{
		// SQL API�� idx�� 1���� ������, C++�ڵ��� �ͼ��� 0���� �����ϵ��� ����ϱ� ���� + 1�� ����
		_dbConnection.BindParam(idx + 1, &value, &_paramIndex[idx]);
		_paramFlag |= (1LL << idx); // idx��° ���ڸ� ä���ָ� ��Ʈ ����ũ�� 1�� ����
	}

	void BindParam(int32 idx, const WCHAR* value)
	{
		_dbConnection.BindParam(idx + 1, value, &_paramIndex[idx]);
		_paramFlag |= (1LL << idx);
	}

	template<typename T, int32 N>
	void BindParam(int32 idx, T(&value)[N])
	{
		// ���� �����͸� ���ڷ� ���ε�
		_dbConnection.BindParam(idx + 1, (const BYTE*)value, size32(T) * N, &_paramIndex[idx]);
		_paramFlag |= (1LL << idx);
	}

	template<typename T>
	void BindParam(int32 idx, T* value, int32 N)
	{
		_dbConnection.BindParam(idx + 1, (const BYTE*)value, size32(T) * N, &_paramIndex[idx]);
		_paramFlag |= (1LL << idx);
	}

	/* ���� ����� �޾ƿ� ������ ���ε�*/
	template<typename T>
	void BindCol(int32 idx, T& value)
	{
		_dbConnection.BindCol(idx + 1, &value, &_columnIndex[idx]);
		_columnFlag |= (1LL << idx);
	}

	template<int32 N>
	void BindCol(int32 idx, WCHAR(&value)[N])
	{
		// null�� �ش��ϴ� ����Ʈ�� ���ܽ�Ű�� ���� size�� N - 1�� ����
		_dbConnection.BindCol(idx + 1, value, N - 1, &_columnIndex[idx]);
		_columnFlag |= (1LL << idx);
	}

	void BindCol(int32 idx, WCHAR* value, int32 len)
	{
		_dbConnection.BindCol(idx + 1, value, len - 1, &_columnIndex[idx]);
		_columnFlag |= (1LL << idx);
	}

	template<typename T, int32 N>
	void BindCol(int32 idx, T(&value)[N])
	{
		_dbConnection.BindCol(idx + 1, value, size32(T) * N, &_columnIndex[idx]);
		_columnFlag |= (1LL << idx);
	}

protected:
	DbConnection& _dbConnection;
	const WCHAR* _query;
	SQLLEN			_paramIndex[ParamCount > 0 ? ParamCount : 1];
	SQLLEN			_columnIndex[ColumnCount > 0 ? ColumnCount : 1];
	uint64			_paramFlag; // ��� ���ڰ� �� ä�����ִ��� üũ�ϱ� ���� ��Ʈ ����ũ
	uint64			_columnFlag; // ��� ���ڰ� �� ä�����ִ��� üũ�ϱ� ���� ��Ʈ ����ũ
};

// ��� ����
/*
int main()
{
	for (int32 i = 0; i < 3; i++)
	{
		DBConnection* dbConn = GDBConnectionPool->Pop();

		// INSERT �ϱ�
		// ������ ������ ���� 3��, �޾ƿ� ���� 0��
		DBBind<3, 0> dbBind(*dbConn, L"INSERT INTO [dbo].[gold]([gold], [name], [createDate]) VALUES(?, ?, ?)");

		int32 gold = 200;
		dbBind.BindParam(0, gold);
		WCHAR name[100] = L"MyName";
		dbBind.BindParam(1, name);
		TIMESTAMP_STRUCT ts = { 2022,6,13 };
		dbBind.BindParam(2, ts);

		dbBind.Execute();

		// SELECT �ϱ�
		// ������ ������ ���� 1��, �޾ƿ� ���� 4��
		DBBind<1, 4> dbBind2(*dbConn, L"SELECT id, gold, name, createDate FROM [dbo].[Gold] WHERE gold = (?)");

		// ������ ������ ���� 1�� ���ε�
		int32 gold = 200;
		dbBind2.BindParam(0, gold);

		// ���� ����� �޾ƿ� ���� 4�� ���ε�
		int32 outId = 0;
		dbBind2.BindCol(0, outId);

		int32 outGold = 0;
		dbBind2.BindCol(1, outGold);

		WCHAR outName[100];
		dbBind2.BindCol(2, outName);

		TIMESTAMP_STRUCT outDate = {};
		dbBind2.BindCol(3, outDate);

		// SELECT ���� ����
		dbBind2.Execute();

		// wide string �ѱ� ���� �ɼ� ����
		wcout.imbue(locale("kor"));

		// ���� ���� ��� ��������
		while (dbBind2->Fetch())
		{
			wcout << "Id : " << outId << ", Gold : " << outGold << ", Name : " << outName << '\n';
			wcout << "Date : " << outDate.year << "/" << outDate.month << "/" << outDate.day << '\n';
		}


	}
}*/