#include "pch.h"
#include "DBConnectionPool.h"

/*-------------------
	DBConnectionPool
--------------------*/

DBConnectionPool::DBConnectionPool()
{

}

DBConnectionPool::~DBConnectionPool()
{
	Clear();
}

bool DBConnectionPool::Connect(int32 connectionCount, const WCHAR* connectionString)
{
	WRITE_LOCK;

	// SQLHENV 할당 초기화
	if (SQL_SUCCESS != ::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_environment))
	{
		return false;
	}

	// SQL 버전 설정
	if (SQL_SUCCESS != ::SQLSetEnvAttr(_environment, SQL_ATTR_ODBC_VERSION,
		reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0))
	{
		return false;
	}

	// connectionCount만큼 Connection 생성
	for (int32 i = 0; i < connectionCount; i++)
	{
		DBConnection* connection = XNew<DBConnection>(); // XNew는 Pooling을 위한 할당 함수
		if (connection->Connect(_environment, connectionString) == false)
			return false;

		_connectionPool.push_back(connection);
	}

	return true;
}

void DBConnectionPool::Clear()
{
	WRITE_LOCK;

	if (_environment != SQL_NULL_HANDLE)
	{	// DB Environmenet 핸들 닫기(해제)
		::SQLFreeHandle(SQL_HANDLE_ENV, _environment);
		_environment = SQL_NULL_HANDLE;
	}

	// connection 해제
	for (DBConnection* connection : _connectionPool)
		XDelete(connection);

	_connectionPool.clear();
}

DBConnection* DBConnectionPool::Pop()
{
	WRITE_LOCK;

	if (_connectionPool.empty())
		return nullptr;

	DBConnection* connection = _connectionPool.back();
	_connectionPool.pop_back();
	return connection;
}

void DBConnectionPool::Push(DBConnection* connection)
{
	WRITE_LOCK;

	_connectionPool.push_back(connection);
}