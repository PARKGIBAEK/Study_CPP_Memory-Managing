#pragma once
#include <sql.h>
#include <sqlext.h>

#include "../Memory/Container.h"


namespace ServerDb
{

using namespace ServerCore;



enum
{
    WVARCHAR_MAX = 4000,
    BINARY_MAX = 8000,
};

class DbConnection
{
public:
    // connect to db with handle
    bool Connect(SQLHENV henv, const WCHAR* connectionString);
    // disconnect
    void Clear();
    // execute query
    bool Execute(const WCHAR* query);
    // fetch the query execution results( used in "SELECT ~ " )
    bool Fetch();
    // Get row count
    int32 GetRowCount();

    void Unbind();

public:
    // Query������ �� ���ڿ��� ?�ڸ��� ��ü�� ���ڸ� ���ε�
    bool BindParam(int32 paramIndex, bool* value, SQLLEN* index);
    bool BindParam(int32 paramIndex, float* value, SQLLEN* index);
    bool BindParam(int32 paramIndex, double* value, SQLLEN* index);
    bool BindParam(int32 paramIndex, int8* value, SQLLEN* index);
    bool BindParam(int32 paramIndex, int16* value, SQLLEN* index);
    bool BindParam(int32 paramIndex, int32* value, SQLLEN* index);
    bool BindParam(int32 paramIndex, int64* value, SQLLEN* index);
    bool BindParam(int32 paramIndex, TIMESTAMP_STRUCT* value, SQLLEN* index);
    bool BindParam(int32 paramIndex, const WCHAR* str, SQLLEN* index);
    bool BindParam(int32 paramIndex, const BYTE* bin, int32 size, SQLLEN* index);

    // Query ���� ����� �޾ƿ� �ܺ� ������ ���ε�
    bool BindCol(int32 columnIndex, bool* value, SQLLEN* index);
    bool BindCol(int32 columnIndex, float* value, SQLLEN* index);
    bool BindCol(int32 columnIndex, double* value, SQLLEN* index);
    bool BindCol(int32 columnIndex, int8* value, SQLLEN* index);
    bool BindCol(int32 columnIndex, int16* value, SQLLEN* index);
    bool BindCol(int32 columnIndex, int32* value, SQLLEN* index);
    bool BindCol(int32 columnIndex, int64* value, SQLLEN* index);
    bool BindCol(int32 columnIndex, TIMESTAMP_STRUCT* value, SQLLEN* index);
    bool BindCol(int32 columnIndex, WCHAR* str, int32 size, SQLLEN* index);
    bool BindCol(int32 columnIndex, BYTE* bin, int32 size, SQLLEN* index);

private:
    /* cType : SQL�� Ư�� �ڷ����� ��Ī�Ǵ� C����� Ÿ��
        sqlType : cType�� ��Ī�Ǵ� SQL�� �ڷ���		*/
    bool BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr,
                   SQLLEN* index);
    /* ���� ���� ����� �������� ��û�� �޸𸮿� ���ε�*/
    bool BindCol(SQLUSMALLINT columnIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index);
    /* ���� ��ȣ�� ���� ���� ���*/
    void HandleError(SQLRETURN ret);

private:
    // SQL connection ��� �ڵ�
    SQLHDBC _connection = SQL_NULL_HANDLE; // = SQL_NULL_HANDLE;
    // DB�� connection�� ���� �� �������� ���ڸ� �����ϰų� ������ �� ���(�� : ���� ����, ���� ��� �������� )
    SQLHSTMT _statement = SQL_NULL_HANDLE; // = SQL_NULL_HANDLE;
};


// DBBind Ŭ���� ��� ��õ( ��� ���� 3 ����)
// ��õ���� : ������ ���ڵ�� ���� ������ ������ �ÿ� �߰��� �� �ִ�.

/* ��� ���� 1
(�⺻ API�� ���� �������� ���� ����̹Ƿ� ������ ���ڰ� �������� ����õ) */
/*
int main()
{
	// 1.  DB Connection Pool ��ü ���� 
	DBConnectionPool* dbConnectionPool = new DBConnectionPool();

	// 2. SQL�� �����Ͽ� �ش� DB�� ���� ���� �ڵ��� �߱� �ޱ�
	-	���� ���񽺿����� Connection String�� �ϵ� �ڵ����� �ʰ�,
		Server �ּ�, DB��, Account/PW ���� ���Ϸ� �и��Ͽ� �ε��ϴ� ������� �ؾ��Ѵ�.
	dbConnectionPool->Connect(10,
		L"Driver={SQL Server Native Client 11.0};Server=(localdb)\\MSSQLLocalDB;Database=ServerDb;Trusted_Connection=Yes;");


	//  3. ���̺� �����ϱ�
	//-	dbo��� DB�� Gold��� ���̺��� ������ ����� ���� �����(column ��� �ɼ� �Է�)
	
	auto query = L"	DROP TABLE IF EXISTS[dbo].[Gold];	\
			CREATE TABLE [dbo].[Gold]					\
			(											\
				[id] INT NOT NULL PRIMARY KEY IDENTITY,	\
				[gold] INT NULL							\
			)";
	//	4. Connection Pool���� Connection ��������
	DBConnection* dbConnection = dbConnectionPool->Pop();

	//	5. Connection���� ���� �����ϱ�
	dbConnection->Execute(query);

	//  connection�� connection Pool�� ��ȯ�ϱ�
	dbConnectionPool->Push(dbConnection);

	//	6. INSERT
	int count;
	for (size_t i = 0; i < count; i++)
	{
		DBConnection* dbConn = dbConnectionPool->Pop();
		// ������ ���ε� �� ���� ������
		dbConn->Unbind();

		// �ѱ� ���� ����
		int32 gold = 100; // 100���
		SQLLEN len = 0; // �������ڰ� �ƴϱ� ������ 0���� �����ϸ� ��

		// �ѱ� ���� ���ε� : Query�� ?�κп� �ش��ϴ� �ڸ��� ��ü�� ���ڸ� ���ε���(���ε��� ���ڴ� �ϳ� �̻��� �� ����)
		//  SQL �������� ?�� ǥ�õǾ� �ִ� �κп� Bind��Ű�� �ȴ�
		dbConn->BindParam(1, SQL_C_LONG, SQL_INTEGER, sizeof(gold), &gold, &len);

		//-	100��� �ֱ�
		auto query1 = L"INSERT INTO[dbo].[Gold]([gold]) VALUES(? )";

		// INSERT ���� ����
		dbConn->Execute(L"INSERT INTO [dbo].[Gold]([gold]) VALUES(?)");

		// connection �ݳ�
		dbConnectionPool->Push(dbConn);
	}

	//		7. Read
	{
		DBConnection dbConn = dbConnectionPool->Pop();
		// ���� �ٵ��� ���� ������
		dbConn->Unbind();

		int32 gold = 100;
		SQLLEN len = 0;//�������ڰ� �ƴϹǷ� 0���� �����ص� ��

		// �Ѱ��� ���� ���ε�
		dbConn->BindParam(1, SQL_C_LONG, SQL_INTEGER, sizeof(gold), &gold, &len);

		// ���� ���� ����� �޾ƿ� �����͸� ������ ���� �����ϱ�
		//-	���� ���� ������� id�� gold�� outId�� outGold�� ���� ����

		// id�� �ش��ϴ� Column�� �����͸� �޾ƿ� ����
		int32 outId = 0;
		SQLLEN outIdLen = 0;
		dbConn->BindCol(1, SQL_C_LONG, sizeof(outId), &outId, &outIdLen);

		// gold�� �ش��ϴ� Column�� �����͸� �޾ƿ� ����
		int32 outGold = 0;
		SQLLEN outGoldLen = 0;
		dbConn->BindCol(2, SQL_C_LONG, sizeof(outGold), &outGold, &outGoldLen);

		
		// SELECT ���� ����( ������ �����ص� �����Ͱ� outId�� outGold�� ���� �ʴ´� )
		dbConn->Execute(L"SELECT id, gold FROM [dbo].[Gold] WHERE gold = (?)");

		// ���� ���� ����� outId�� outGold�� �޾ƿ����� Fetch�� �����ؾ� �Ѵ�
		//	���� ���� ����� �������� Row�� 1�� �̻��̹Ƿ� �� ������ �� ���� �ݺ������� Fetch�� �����Ѵ�.
		//	�ѹ� Fetch������ ������ �ϳ��� Row�� �ش��ϴ� �����Ͱ� outId�� outGold�� ���޵ȴ�.
		while (dbConn->Fetch())
		{
			cout << "Id : " << outId << ", Gold : " << outGold << '\n';
		}

		// connection �ݳ�
		dbConnectionPool->Push(dbConn);
	}
}
*/


/*	��� ���� 2(�⺻ API�� 2�������� �����Ͽ� ������ ���� ������ ���� �ٿ���)	*/
/*
int main()
{

	// 1.  DB Connection Pool ��ü ���� 
	DBConnectionPool* dbConn = new DBConnectionPool();

	// 2. SQL�� �����Ͽ� �ش� DB�� ���� ���� �ڵ��� �߱� �ޱ�
	// -���� ���񽺿����� Connection String�� �ϵ� �ڵ����� �ʰ�,
	//	Server �ּ�, DB��, Account / PW ���� ���Ϸ� �и��Ͽ� �ε��ϴ� ������� �ؾ��Ѵ�.
	dbConn->Connect(10,
			L"Driver={SQL Server Native Client 11.0};Server=(localdb)\\MSSQLLocalDB;Database=ServerDb;Trusted_Connection=Yes;");
	dbConn->Unbind();

	// 3. INSERT
	// �ѱ� ���� ����
	int32 gold = 100; // 100���
	SQLLEN len = 0; // �������ڰ� �ƴϱ� ������ 0���� �����ϸ� ��

	WCHAR name[100] = L"MyName";
	SQLLEN nameLen = 0;

	TIMESTAMP_STRUCT ts{};
	ts.year = 2022;
	ts.month = 6;
	ts.day = 13;
	SQLLEN tsLen = 0;

	// �ѱ� ���� ���ε�
	dbConn->BindParam(1, &gold, &len);
	dbConn->BindParam(2, &name, &len);
	dbConn->BindParam(3, &ts, &tsLen);

	// INSERT ���� ����
	dbConn->Execute(L"INSERT INTO [dbo].[Gold]([gold],[name],[createDate]) VALUES(?, ?, ?)");
	
	// 4. SELECT
	// id�� �ش��ϴ� Column�� �����͸� �޾ƿ� ����
	int32 outId = 0;
	SQLLEN outIdLen = 0;
	dbConn->BindCol(1, &outId, &outIdLen);

	// gold�� �ش��ϴ� Column�� �����͸� �޾ƿ� ����
	int32 outGold = 0;
	SQLLEN outGoldLen = 0;
	dbConn->BindCol(2, &outGold, &outGoldLen);

	// name�� �ش��ϴ� Column�� �����͸� �޾ƿ� ����
	WCHAR outName[100];
	SQLLEN outNameLen = 0;
	dbConn->BindCol(3, &outName, &outNameLen);

	// date�� �ش��ϴ� Column�� �����͸� �޾ƿ� ����
	TIMESTAMP_STRUCT outDate = {};
	SQLLEN outDateLen = 0;
	dbConn->BindCol(4, &outDate, &outDateLen);
	
	// SELECT ���� ����
	dbConn->Execute(L"SELECT id, gold, name, createDate FROM [dbo].[Gold] WHERE gold = (?)");
	
	// wide string �ѱ� ���� �ɼ� ����
	wcout.imbue(locale("kore"));

	// ���� ���� ��� ��������
	while (dbConn->Fetch())
	{
		wcout << "Id : " << outId << ", Gold : " << outGold << ", Name : " << outName<<'\n';
		wcout << "Date : " << outDate.year << "/" << outDate.month << "/" << outDate.day << '\n';
	}

	// connection �ݳ�
	GDBConnectionPool->Push(dbConn);
}
*/
}
