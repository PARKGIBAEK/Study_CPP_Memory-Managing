#pragma once
#include "DBConnection.h"

/*-------------------
	DBConnectionPool
--------------------*/

class DBConnectionPool
{
public:
	DBConnectionPool();
	~DBConnectionPool();

	// Create the DB Connection as many as connectionCount
	bool					Connect(int32 connectionCount, 
									const WCHAR* connectionString);
	void					Clear();

	// get remaining DB connection in 'connection pool'
	DBConnection*			Pop();
	// return the DB connection to the 'connection pool'
	void					Push(DBConnection* connection);

private:
	USE_LOCK;
	// SQL handle for environment
	SQLHENV					_environment = SQL_NULL_HANDLE;
	Vector<DBConnection*>	_connectionPool;
};

