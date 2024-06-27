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

	/* 쿼리역할을 할 문자열의 ?부분을 대체할 인자를 바인딩 */
	template<typename T>
	void BindParam(int32 idx, T& value)
	{
		// SQL API는 idx를 1부터 받지만, C++코딩에 익숙한 0부터 시작하도록 사용하기 위해 + 1을 해줌
		_dbConnection.BindParam(idx + 1, &value, &_paramIndex[idx]);
		_paramFlag |= (1LL << idx); // idx번째 인자를 채워주면 비트 마스크를 1로 세팅
	}

	void BindParam(int32 idx, const WCHAR* value)
	{
		_dbConnection.BindParam(idx + 1, value, &_paramIndex[idx]);
		_paramFlag |= (1LL << idx);
	}

	template<typename T, int32 N>
	void BindParam(int32 idx, T(&value)[N])
	{
		// 가변 데이터를 인자로 바인드
		_dbConnection.BindParam(idx + 1, (const BYTE*)value, size32(T) * N, &_paramIndex[idx]);
		_paramFlag |= (1LL << idx);
	}

	template<typename T>
	void BindParam(int32 idx, T* value, int32 N)
	{
		_dbConnection.BindParam(idx + 1, (const BYTE*)value, size32(T) * N, &_paramIndex[idx]);
		_paramFlag |= (1LL << idx);
	}

	/* 쿼리 결과를 받아올 변수를 바인딩*/
	template<typename T>
	void BindCol(int32 idx, T& value)
	{
		_dbConnection.BindCol(idx + 1, &value, &_columnIndex[idx]);
		_columnFlag |= (1LL << idx);
	}

	template<int32 N>
	void BindCol(int32 idx, WCHAR(&value)[N])
	{
		// null에 해당하는 바이트는 제외시키기 위해 size는 N - 1로 설정
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
	uint64			_paramFlag; // 모든 인자가 다 채워져있는지 체크하기 위한 비트 마스크
	uint64			_columnFlag; // 모든 인자가 다 채워져있는지 체크하기 위한 비트 마스크
};

// 사용 예시
/*
int main()
{
	for (int32 i = 0; i < 3; i++)
	{
		DBConnection* dbConn = GDBConnectionPool->Pop();

		// INSERT 하기
		// 쿼리에 전달할 인자 3개, 받아올 인자 0개
		DBBind<3, 0> dbBind(*dbConn, L"INSERT INTO [dbo].[gold]([gold], [name], [createDate]) VALUES(?, ?, ?)");

		int32 gold = 200;
		dbBind.BindParam(0, gold);
		WCHAR name[100] = L"MyName";
		dbBind.BindParam(1, name);
		TIMESTAMP_STRUCT ts = { 2022,6,13 };
		dbBind.BindParam(2, ts);

		dbBind.Execute();

		// SELECT 하기
		// 쿼리에 전달할 인자 1개, 받아올 인자 4개
		DBBind<1, 4> dbBind2(*dbConn, L"SELECT id, gold, name, createDate FROM [dbo].[Gold] WHERE gold = (?)");

		// 쿼리에 전달할 인자 1개 바인딩
		int32 gold = 200;
		dbBind2.BindParam(0, gold);

		// 쿼리 결과를 받아올 변수 4개 바인딩
		int32 outId = 0;
		dbBind2.BindCol(0, outId);

		int32 outGold = 0;
		dbBind2.BindCol(1, outGold);

		WCHAR outName[100];
		dbBind2.BindCol(2, outName);

		TIMESTAMP_STRUCT outDate = {};
		dbBind2.BindCol(3, outDate);

		// SELECT 쿼리 실행
		dbBind2.Execute();

		// wide string 한글 지원 옵션 설정
		wcout.imbue(locale("kor"));

		// 쿼리 실행 결과 가져오기
		while (dbBind2->Fetch())
		{
			wcout << "Id : " << outId << ", Gold : " << outGold << ", Name : " << outName << '\n';
			wcout << "Date : " << outDate.year << "/" << outDate.month << "/" << outDate.day << '\n';
		}


	}
}*/