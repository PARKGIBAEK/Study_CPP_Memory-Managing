#pragma once
#include "DBConnection.h"

/*-------------------
	DBConnectionPool
--------------------*/
/* DB Connection Pool을 만드는 이유
 - DB와의 연결을 미리 여러개 맺어 놓고, DB관련 요청 시 재사용 하기 위함 */
class DBConnectionPool
{
public:
	DBConnectionPool();
	~DBConnectionPool();
	/* connectionCount: 몇개의 연결을 맺어 놓을 것인지, 
	*	connectionString : DB에 연결할 때의 요건(어떤 DB와 연결할 때 어떤 조건 / 환경으로 연결할지)
	* 최초로 서버 가동 시 한번만 호출하면 됨
	*/
	bool					Connect(int32 connectionCount, const WCHAR* connectionString);
	// 맺어 놓은 연결들을 다 해제하는 함수(사용할 일은 없지만 예비 용)
	void					Clear();
	// Pool에서 미리 맺어놓은 DB Connection을 꺼내는 함수
	DBConnection*		Pop();
	// 꺼내 쓴 DB Connection을 다시 Pool에 반환하는 함수
	void					Push(DBConnection* connection);

private:
	USE_LOCK;
	// SQLHENV : SQL HANDLE for ENVIRONMENT
	SQLHENV						_environment = SQL_NULL_HANDLE;
	Vector<DBConnection*>	_connections;
};

