#pragma once
#include "../Memory/Container.h"
#include "../Core/CoreMacro.h"
#include "../DB/DBConnection.h"


namespace ServerDb
{

using namespace ServerCore;


class DBConnectionPool
{
public:
    DBConnectionPool();
    ~DBConnectionPool();

    // Create the DB Connection as many as connectionCount
    bool Connect(int32 connectionCount,
                 const WCHAR* connectionString);
    void Clear();

    // get remaining DB connection in 'connection pool'
    DbConnection* Pop();
    // return the DB connection to the 'connection pool'
    void Push(DbConnection* connection);

private:
    USE_LOCK;
    // SQL handle for environment
    SQLHENV _environment;
    Vector<DbConnection*> _connectionPool;
};
}
