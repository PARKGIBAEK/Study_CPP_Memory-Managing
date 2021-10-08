#pragma once
#include <sql.h>
#include <sqlext.h>

/*----------------
	DBConnection
-----------------*/

class DBConnection
{
public:
	// ODBC를 사용하는 환경에 맞는 연결을 생성하는 함수
	bool			Connect(SQLHENV henv, const WCHAR* connectionString);
	void			Clear();
	// DB Query를 실행하는 함수
	bool			Execute(const WCHAR* query);
	// 실행 결과를 받아오는 함수
	bool			Fetch();
	// 데이터가 몇개인지 확인하는 함수
	int32			GetRowCount();
	// 기존에 들고있던 데이터를 비우는 함수
	void			Unbind();

public:
	/* Query를 실행할 때 인자를 넘겨주는 함수
	  paramIndex : 넘겨주는 인자의 갯수
	  cType : SQL에서 사용할 C언어 데이터 타입
	  sqlType : SQL의 타입
	  len :
	  ptr :
	  index  :
	*/
	bool			BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr, SQLLEN* index);
	/* Query 실행 결과를 받아올 때 사용하는 함수
	  columnIndex : 컬럼 갯수
	  cType : SQL에서 사용할 C언어 데이터 타입
	  len : 
	  value : 
	  index : 
	 */
	bool			BindCol(SQLUSMALLINT columnIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index);
	/* Error코드에 따른 Error를 처리하기 위한 함수*/
	void			HandleError(SQLRETURN ret);
	
private:
	// SQLHDBC : SQL Handle for Database Connection(DB와의 연결을 담당하는 핸들)
	SQLHDBC			_connection = SQL_NULL_HANDLE;
	// SQLHSTMT : SQL Handle for Statement(연결된 핸들을 통해서 인자(Query/Query결과)들을 전달/수신)
	SQLHSTMT			_statement = SQL_NULL_HANDLE;
};

