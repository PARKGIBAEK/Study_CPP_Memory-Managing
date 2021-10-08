#pragma once
#include <sql.h>
#include <sqlext.h>

/*----------------
	DBConnection
-----------------*/

class DBConnection
{
public:
	// ODBC�� ����ϴ� ȯ�濡 �´� ������ �����ϴ� �Լ�
	bool			Connect(SQLHENV henv, const WCHAR* connectionString);
	void			Clear();
	// DB Query�� �����ϴ� �Լ�
	bool			Execute(const WCHAR* query);
	// ���� ����� �޾ƿ��� �Լ�
	bool			Fetch();
	// �����Ͱ� ����� Ȯ���ϴ� �Լ�
	int32			GetRowCount();
	// ������ ����ִ� �����͸� ���� �Լ�
	void			Unbind();

public:
	/* Query�� ������ �� ���ڸ� �Ѱ��ִ� �Լ�
	  paramIndex : �Ѱ��ִ� ������ ����
	  cType : SQL���� ����� C��� ������ Ÿ��
	  sqlType : SQL�� Ÿ��
	  len :
	  ptr :
	  index  :
	*/
	bool			BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr, SQLLEN* index);
	/* Query ���� ����� �޾ƿ� �� ����ϴ� �Լ�
	  columnIndex : �÷� ����
	  cType : SQL���� ����� C��� ������ Ÿ��
	  len : 
	  value : 
	  index : 
	 */
	bool			BindCol(SQLUSMALLINT columnIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index);
	/* Error�ڵ忡 ���� Error�� ó���ϱ� ���� �Լ�*/
	void			HandleError(SQLRETURN ret);
	
private:
	// SQLHDBC : SQL Handle for Database Connection(DB���� ������ ����ϴ� �ڵ�)
	SQLHDBC			_connection = SQL_NULL_HANDLE;
	// SQLHSTMT : SQL Handle for Statement(����� �ڵ��� ���ؼ� ����(Query/Query���)���� ����/����)
	SQLHSTMT			_statement = SQL_NULL_HANDLE;
};

