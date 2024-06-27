#include "DBConnectionPool.h"
#include "Lock.h"
#include "MemoryManager.h"
/*-------------------
	DBConnectionPool
--------------------*/

DBConnectionPool::DBConnectionPool(): _environment(SQL_NULL_HANDLE)
{
}

DBConnectionPool::~DBConnectionPool()
{
	Clear();
}

bool DBConnectionPool::Connect(int32 connectionCount, const WCHAR* connectionString)
{
	WRITE_LOCK;

	// SQLHENV �Ҵ� �ʱ�ȭ
	if (SQL_SUCCESS != ::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_environment))
	{
		return false;
	}

	// SQL ���� ����
	if (SQL_SUCCESS != ::SQLSetEnvAttr(_environment, SQL_ATTR_ODBC_VERSION,
		reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0))
	{
		return false;
	}

	// connectionCount��ŭ Connection ����
	for (int32 i = 0; i < connectionCount; i++)
	{
		DbConnection* connection = XNew<DbConnection>(); // XNew�� Pooling�� ���� �Ҵ� �Լ�
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
	{	// DB Environmenet �ڵ� �ݱ�(����)
		::SQLFreeHandle(SQL_HANDLE_ENV, _environment);
		_environment = SQL_NULL_HANDLE;
	}

	// connection ����
	for (DbConnection* connection : _connectionPool)
		XDelete(connection);

	_connectionPool.clear();
}

DbConnection* DBConnectionPool::Pop()
{
	WRITE_LOCK;

	if (_connectionPool.empty())
		return nullptr;

	DbConnection* connection = _connectionPool.back();
	_connectionPool.pop_back();
	return connection;
}

void DBConnectionPool::Push(DbConnection* connection)
{
	WRITE_LOCK;

	_connectionPool.push_back(connection);
}