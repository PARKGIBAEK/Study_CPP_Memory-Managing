#pragma once
#include "DBConnection.h"

/*-------------------
	DBConnectionPool
--------------------*/
/* DB Connection Pool�� ����� ����
 - DB���� ������ �̸� ������ �ξ� ����, DB���� ��û �� ���� �ϱ� ���� */
class DBConnectionPool
{
public:
	DBConnectionPool();
	~DBConnectionPool();
	/* connectionCount: ��� ������ �ξ� ���� ������, 
	*	connectionString : DB�� ������ ���� ���(� DB�� ������ �� � ���� / ȯ������ ��������)
	* ���ʷ� ���� ���� �� �ѹ��� ȣ���ϸ� ��
	*/
	bool					Connect(int32 connectionCount, const WCHAR* connectionString);
	// �ξ� ���� ������� �� �����ϴ� �Լ�(����� ���� ������ ���� ��)
	void					Clear();
	// Pool���� �̸� �ξ���� DB Connection�� ������ �Լ�
	DBConnection*		Pop();
	// ���� �� DB Connection�� �ٽ� Pool�� ��ȯ�ϴ� �Լ�
	void					Push(DBConnection* connection);

private:
	USE_LOCK;
	// SQLHENV : SQL HANDLE for ENVIRONMENT
	SQLHENV						_environment = SQL_NULL_HANDLE;
	Vector<DBConnection*>	_connections;
};

