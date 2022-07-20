#pragma once
#include <sql.h>
#include <sqlext.h>

/*----------------
	DBConnection
-----------------*/

enum
{
	WVARCHAR_MAX = 4000,
	BINARY_MAX = 8000
};

class DBConnection
{
public:
	// connect to db with handle
	bool			Connect(SQLHENV henv, const WCHAR* connectionString);
	// disconnect
	void			Clear();
	// execute query
	bool			Execute(const WCHAR* query);
	// fetch the query execution results( used in "SELECT ~ " )
	bool			Fetch();
	// Get row count
	int32			GetRowCount();

	void			Unbind();

public:
	// Query역할을 할 문자열의 ?자리에 대체될 인자를 바인딩
	bool			BindParam(int32 paramIndex, bool* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, float* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, double* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, int8* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, int16* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, int32* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, int64* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, TIMESTAMP_STRUCT* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, const WCHAR* str, SQLLEN* index);
	bool			BindParam(int32 paramIndex, const BYTE* bin, int32 size, SQLLEN* index);
	
	// Query 수행 결과를 받아올 외부 변수를 바인딩
	bool			BindCol(int32 columnIndex, bool* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, float* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, double* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, int8* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, int16* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, int32* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, int64* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, TIMESTAMP_STRUCT* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, WCHAR* str, int32 size, SQLLEN* index);
	bool			BindCol(int32 columnIndex, BYTE* bin, int32 size, SQLLEN* index);

private:
	/* cType : SQL의 특정 자료형과 매칭되는 C언어의 타입
		sqlType : cType과 매칭되는 SQL의 자료형		*/
	bool			BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr, SQLLEN* index);
	/* 쿼리 실행 결과를 가져오는 요청을 메모리와 바인딩*/
	bool			BindCol(SQLUSMALLINT columnIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index);
	/* 에러 번호에 따른 오류 출력*/
	void			HandleError(SQLRETURN ret);

private:
	// SQL connection 담당 핸들
	SQLHDBC			_connection = SQL_NULL_HANDLE;
	// DB와 connection을 맺은 후 여러가지 인자를 전달하거나 가져올 때 사용(예 : 쿼리 전달, 쿼리 결과 가져오기 )
	SQLHSTMT		_statement = SQL_NULL_HANDLE;
};


// DBBind 클래스 사용 추천( 사용 예시 3 참조)
// 추천이유 : 누락된 인자들로 인한 에러를 컴파일 시에 발견할 수 있다.

/* 사용 예시 1
(기본 API를 거의 가공하지 않은 방식이므로 전달할 인자가 많아져서 비추천) */
/*
int main()
{
	// 1.  DB Connection Pool 객체 생성 
	DBConnectionPool* dbConnectionPool = new DBConnectionPool();

	// 2. SQL에 연결하여 해당 DB에 대한 연결 핸들을 발급 받기
	-	실제 서비스에서는 Connection String을 하드 코딩하지 않고,
		Server 주소, DB명, Account/PW 등은 파일로 분리하여 로드하는 방식으로 해야한다.
	dbConnectionPool->Connect(10,
		L"Driver={SQL Server Native Client 11.0};Server=(localdb)\\MSSQLLocalDB;Database=ServerDb;Trusted_Connection=Yes;");


	//  3. 테이블 생성하기
	//-	dbo라는 DB에 Gold라는 테이블이 있으면 지우고 새로 만들기(column 명과 옵션 입력)
	
	auto query = L"	DROP TABLE IF EXISTS[dbo].[Gold];	\
			CREATE TABLE [dbo].[Gold]					\
			(											\
				[id] INT NOT NULL PRIMARY KEY IDENTITY,	\
				[gold] INT NULL							\
			)";
	//	4. Connection Pool에서 Connection 꺼내오기
	DBConnection* dbConnection = dbConnectionPool->Pop();

	//	5. Connection으로 쿼리 전달하기
	dbConnection->Execute(query);

	//  connection을 connection Pool에 반환하기
	dbConnectionPool->Push(dbConnection);

	//	6. INSERT
	int count;
	for (size_t i = 0; i < count; i++)
	{
		DBConnection* dbConn = dbConnectionPool->Pop();
		// 기존에 바인딩 된 정보 날리기
		dbConn->Unbind();

		// 넘길 인자 정의
		int32 gold = 100; // 100골드
		SQLLEN len = 0; // 가변인자가 아니기 때문에 0으로 설정하면 됨

		// 넘길 인자 바인딩 : Query의 ?부분에 해당하는 자리를 대체할 인자를 바인딩함(바인딩할 인자는 하나 이상일 수 있음)
		//  SQL 쿼리에서 ?로 표시되어 있는 부분에 Bind시키게 된다
		dbConn->BindParam(1, SQL_C_LONG, SQL_INTEGER, sizeof(gold), &gold, &len);

		//-	100골드 넣기
		auto query1 = L"INSERT INTO[dbo].[Gold]([gold]) VALUES(? )";

		// INSERT 쿼리 실행
		dbConn->Execute(L"INSERT INTO [dbo].[Gold]([gold]) VALUES(?)");

		// connection 반납
		dbConnectionPool->Push(dbConn);
	}

	//		7. Read
	{
		DBConnection dbConn = dbConnectionPool->Pop();
		// 기존 바딩인 정보 날리기
		dbConn->Unbind();

		int32 gold = 100;
		SQLLEN len = 0;//가변인자가 아니므로 0으로 설정해도 됨

		// 넘겨줄 인자 바인딩
		dbConn->BindParam(1, SQL_C_LONG, SQL_INTEGER, sizeof(gold), &gold, &len);

		// 쿼리 수행 결과로 받아온 데이터를 꺼내갈 변수 예약하기
		//-	쿼리 수행 결과에서 id와 gold를 outId와 outGold에 받을 예정

		// id에 해당하는 Column의 데이터를 받아올 변수
		int32 outId = 0;
		SQLLEN outIdLen = 0;
		dbConn->BindCol(1, SQL_C_LONG, sizeof(outId), &outId, &outIdLen);

		// gold에 해당하는 Column의 데이터를 받아올 변수
		int32 outGold = 0;
		SQLLEN outGoldLen = 0;
		dbConn->BindCol(2, SQL_C_LONG, sizeof(outGold), &outGold, &outGoldLen);

		
		// SELECT 쿼리 실행( 쿼리를 실행해도 데이터가 outId와 outGold에 들어가지 않는다 )
		dbConn->Execute(L"SELECT id, gold FROM [dbo].[Gold] WHERE gold = (?)");

		// 쿼리 수행 결과를 outId와 outGold에 받아오려면 Fetch를 실행해야 한다
		//	쿼리 수행 결과는 여러개의 Row가 1개 이상이므로 다 가져올 때 까지 반복적으로 Fetch를 수행한다.
		//	한번 Fetch수행할 때마다 하나의 Row에 해당하는 데이터가 outId와 outGold에 전달된다.
		while (dbConn->Fetch())
		{
			cout << "Id : " << outId << ", Gold : " << outGold << '\n';
		}

		// connection 반납
		dbConnectionPool->Push(dbConn);
	}
}
*/


/*	사용 예시 2(기본 API를 2차적으로 래핑하여 전달할 인자 개수를 조금 줄였다)	*/
/*
int main()
{

	// 1.  DB Connection Pool 객체 생성 
	DBConnectionPool* dbConn = new DBConnectionPool();

	// 2. SQL에 연결하여 해당 DB에 대한 연결 핸들을 발급 받기
	// -실제 서비스에서는 Connection String을 하드 코딩하지 않고,
	//	Server 주소, DB명, Account / PW 등은 파일로 분리하여 로드하는 방식으로 해야한다.
	dbConn->Connect(10,
			L"Driver={SQL Server Native Client 11.0};Server=(localdb)\\MSSQLLocalDB;Database=ServerDb;Trusted_Connection=Yes;");
	dbConn->Unbind();

	// 3. INSERT
	// 넘길 인자 정의
	int32 gold = 100; // 100골드
	SQLLEN len = 0; // 가변인자가 아니기 때문에 0으로 설정하면 됨

	WCHAR name[100] = L"MyName";
	SQLLEN nameLen = 0;

	TIMESTAMP_STRUCT ts{};
	ts.year = 2022;
	ts.month = 6;
	ts.day = 13;
	SQLLEN tsLen = 0;

	// 넘길 인자 바인딩
	dbConn->BindParam(1, &gold, &len);
	dbConn->BindParam(2, &name, &len);
	dbConn->BindParam(3, &ts, &tsLen);

	// INSERT 쿼리 실행
	dbConn->Execute(L"INSERT INTO [dbo].[Gold]([gold],[name],[createDate]) VALUES(?, ?, ?)");
	
	// 4. SELECT
	// id에 해당하는 Column의 데이터를 받아올 변수
	int32 outId = 0;
	SQLLEN outIdLen = 0;
	dbConn->BindCol(1, &outId, &outIdLen);

	// gold에 해당하는 Column의 데이터를 받아올 변수
	int32 outGold = 0;
	SQLLEN outGoldLen = 0;
	dbConn->BindCol(2, &outGold, &outGoldLen);

	// name에 해당하는 Column의 데이터를 받아올 변수
	WCHAR outName[100];
	SQLLEN outNameLen = 0;
	dbConn->BindCol(3, &outName, &outNameLen);

	// date에 해당하는 Column의 데이터를 받아올 변수
	TIMESTAMP_STRUCT outDate = {};
	SQLLEN outDateLen = 0;
	dbConn->BindCol(4, &outDate, &outDateLen);
	
	// SELECT 쿼리 실행
	dbConn->Execute(L"SELECT id, gold, name, createDate FROM [dbo].[Gold] WHERE gold = (?)");
	
	// wide string 한글 지원 옵션 설정
	wcout.imbue(locale("kore"));

	// 쿼리 실행 결과 가져오기
	while (dbConn->Fetch())
	{
		wcout << "Id : " << outId << ", Gold : " << outGold << ", Name : " << outName<<'\n';
		wcout << "Date : " << outDate.year << "/" << outDate.month << "/" << outDate.day << '\n';
	}

	// connection 반납
	GDBConnectionPool->Push(dbConn);
}
*/