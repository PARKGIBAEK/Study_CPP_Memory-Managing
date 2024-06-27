#include "DBConnection.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <iostream>
#include "CoreMacro.h"
/*----------------
	DBConnection
-----------------*/

bool DbConnection::Connect(SQLHENV henv, const WCHAR* connectionString)
{
    if (::SQLAllocHandle(SQL_HANDLE_DBC, henv, &_connection) != SQL_SUCCESS)
        return false;

    
    
    WCHAR stringBuffer[MAX_PATH] = {0};
    ::wcscpy_s(stringBuffer, connectionString); // connectionString����

    WCHAR resultString[MAX_PATH] = {0}; // ��� ������ ����
    SQLSMALLINT resultStringLen = 0;

    SQLRETURN ret = ::SQLDriverConnectW(
        _connection,
        NULL,
        reinterpret_cast<SQLWCHAR*>(stringBuffer),
        _countof(stringBuffer),
        OUT reinterpret_cast<SQLWCHAR*>(resultString),
        _countof(resultString),
        OUT & resultStringLen,
        SQL_DRIVER_NOPROMPT
    );
    // SQL Statement �ڵ� �Ҵ�
    if (::SQLAllocHandle(SQL_HANDLE_STMT, _connection, &_statement) != SQL_SUCCESS)
        return false;

    return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
}

void DbConnection::Clear()
{
    if (_connection != SQL_NULL_HANDLE)
    {
        // DB Connection �ڵ� �ݱ�
        ::SQLFreeHandle(SQL_HANDLE_DBC, _connection);
        _connection = SQL_NULL_HANDLE;
    }

    if (_statement != SQL_NULL_HANDLE)
    {
        ::SQLFreeHandle(SQL_HANDLE_STMT, _statement);
        _statement = SQL_NULL_HANDLE;
    }
}

bool DbConnection::Execute(const WCHAR* query)
{
    // statement �ڵ��� ���� ������ �Ѱ��ش�
    SQLRETURN ret = ::SQLExecDirectW(_statement, (SQLWCHAR*)query, SQL_NTSL);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return true;

    HandleError(ret);
    return false;
}

bool DbConnection::Fetch()
{
    SQLRETURN ret = ::SQLFetch(_statement);

    switch (ret)
    {
    case SQL_SUCCESS:
    case SQL_SUCCESS_WITH_INFO:
        return true;
    case SQL_NO_DATA:
        return false;
    case SQL_ERROR:
        HandleError(ret);
        return false;
    default:
        return true;
    }
}

int32 DbConnection::GetRowCount()
{
    SQLLEN count = 0;
    SQLRETURN ret = ::SQLRowCount(_statement, OUT & count);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return static_cast<int32>(count);

    return -1;
}

void DbConnection::Unbind()
{
    ::SQLFreeStmt(_statement, SQL_UNBIND);
    ::SQLFreeStmt(_statement, SQL_RESET_PARAMS);
    ::SQLFreeStmt(_statement, SQL_CLOSE);
}

bool DbConnection::BindParam(int32 paramIndex, bool* value, SQLLEN* index)
{
    return BindParam(paramIndex, SQL_C_TINYINT, SQL_TINYINT, size32(bool), value, index);
}

bool DbConnection::BindParam(int32 paramIndex, float* value, SQLLEN* index)
{
    return BindParam(paramIndex, SQL_C_FLOAT, SQL_REAL, 0, value, index);
}

bool DbConnection::BindParam(int32 paramIndex, double* value, SQLLEN* index)
{
    return BindParam(paramIndex, SQL_C_DOUBLE, SQL_DOUBLE, 0, value, index);
}

bool DbConnection::BindParam(int32 paramIndex, int8* value, SQLLEN* index)
{
    return BindParam(paramIndex, SQL_C_TINYINT, SQL_TINYINT, size32(int8), value, index);
}

bool DbConnection::BindParam(int32 paramIndex, int16* value, SQLLEN* index)
{
    return BindParam(paramIndex, SQL_C_SHORT, SQL_SMALLINT, size32(int16), value, index);
}

bool DbConnection::BindParam(int32 paramIndex, int32* value, SQLLEN* index)
{
    return BindParam(paramIndex, SQL_C_LONG, SQL_INTEGER, size32(int32), value, index);
}

bool DbConnection::BindParam(int32 paramIndex, int64* value, SQLLEN* index)
{
    return BindParam(paramIndex, SQL_C_SBIGINT, SQL_BIGINT, size32(int64), value, index);
}

bool DbConnection::BindParam(int32 paramIndex, TIMESTAMP_STRUCT* value, SQLLEN* index)
{
    return BindParam(paramIndex, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, size32(TIMESTAMP_STRUCT), value, index);
}

bool DbConnection::BindParam(int32 paramIndex, const WCHAR* str, SQLLEN* index)
{
    SQLULEN size = static_cast<SQLULEN>((::wcslen(str) + 1) * 2);
    *index = SQL_NTSL;

    if (size > WVARCHAR_MAX)
        return BindParam(paramIndex, SQL_C_WCHAR, SQL_WLONGVARCHAR, size, (SQLPOINTER)str, index);
    else
        return BindParam(paramIndex, SQL_C_WCHAR, SQL_WVARCHAR, size, (SQLPOINTER)str, index);
}

bool DbConnection::BindParam(int32 paramIndex, const BYTE* bin, int32 size, SQLLEN* index)
{
    if (bin == nullptr)
    {
        *index = SQL_NULL_DATA;
        size = 1;
    }
    else
        *index = size;

    if (size > BINARY_MAX)
        return BindParam(paramIndex, SQL_C_BINARY, SQL_LONGVARBINARY, size, (BYTE*)bin, index);
    else
        return BindParam(paramIndex, SQL_C_BINARY, SQL_BINARY, size, (BYTE*)bin, index);
}

bool DbConnection::BindCol(int32 columnIndex, bool* value, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_C_TINYINT, size32(bool), value, index);
}

bool DbConnection::BindCol(int32 columnIndex, float* value, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_C_FLOAT, size32(float), value, index);
}

bool DbConnection::BindCol(int32 columnIndex, double* value, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_C_DOUBLE, size32(double), value, index);
}

bool DbConnection::BindCol(int32 columnIndex, int8* value, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_C_TINYINT, size32(int8), value, index);
}

bool DbConnection::BindCol(int32 columnIndex, int16* value, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_C_SHORT, size32(int16), value, index);
}

bool DbConnection::BindCol(int32 columnIndex, int32* value, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_C_LONG, size32(int32), value, index);
}

bool DbConnection::BindCol(int32 columnIndex, int64* value, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_C_SBIGINT, size32(int64), value, index);
}

bool DbConnection::BindCol(int32 columnIndex, TIMESTAMP_STRUCT* value, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_C_TYPE_TIMESTAMP, size32(TIMESTAMP_STRUCT), value, index);
}

bool DbConnection::BindCol(int32 columnIndex, WCHAR* str, int32 size, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_C_WCHAR, size, str, index);
}

bool DbConnection::BindCol(int32 columnIndex, BYTE* bin, int32 size, SQLLEN* index)
{
    return BindCol(columnIndex, SQL_BINARY, size, bin, index);
}

bool DbConnection::BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len,
                             SQLPOINTER ptr, SQLLEN* index)
{
    /*
    * SQLHSTMT        StatementHandle : statement�ڵ�,  
      SQLUSMALLINT    ParameterNumber : ���� ����(1������ ����),  
      SQLSMALLINT     InputOutputType : ������ ���� ,  
      SQLSMALLINT     ValueType : C������ Ÿ��,  
      SQLSMALLINT     ParameterType : SQL ������ Ÿ��,  
      SQLULEN         ColumnSize : �÷� or ǥ���� ������,  
      SQLSMALLINT     DecimalDigits : ,  
      SQLPOINTER      ParameterValuePtr : ������ ������ ���ۿ� ���� ������,  
      SQLLEN          BufferLength : �� ���� ����,  
      SQLLEN *        StrLen_or_IndPtr) : ;  
    * paramIndex : ���° ���ڸ� ������ ������
    * SQL_PARAM_INPUT : ���� �Է� �ɼ� ����
    * cType : SQL�� Ư�� �ڷ����� ��Ī�Ǵ� C����� Ÿ��
    * sqlType : cType�� ��Ī�Ǵ� SQL�� �ڷ���
    * len : sqlType�� ũ��
    * ptr : �����Ͱ� �ִ� ���� �޸� �ּ�
    * index : ���������� ��� ������ ������ �Ѱ� ��(�������̰� �ƴ� ��쿡�� 0�� ���� ���� ������ ����)
    */
    // statement�ڵ鿡 ���ڸ� ���ε� ��Ű��
    SQLRETURN ret = ::SQLBindParameter(
        _statement, paramIndex, SQL_PARAM_INPUT,
        cType, sqlType, len, 0, ptr, 0, index);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        HandleError(ret);
        return false;
    }

    return true;
}

bool DbConnection::BindCol(SQLUSMALLINT columnIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index)
{
    SQLRETURN ret = ::SQLBindCol(_statement, columnIndex, cType,
                                 value, len, index);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        HandleError(ret);
        return false;
    }

    return true;
}

void DbConnection::HandleError(SQLRETURN ret)
{
    if (ret == SQL_SUCCESS)
        return;

    SQLSMALLINT index = 1;
    SQLWCHAR sqlState[MAX_PATH] = {0};
    SQLINTEGER nativeErr = 0;
    SQLWCHAR errMsg[MAX_PATH] = {0};
    SQLSMALLINT msgLen = 0;
    SQLRETURN errorRet = 0;

    while (true)
    {
        // errorRet = ::SQLGetDiagRecW(
        //     SQL_HANDLE_STMT,
        //     _statement,
        //     index,
        //     sqlState,
        //     OUT &nativeErr,
        //     errMsg,
        //     _countof(errMsg),
        //     &msgLen
        // );

        if (errorRet == SQL_NO_DATA)
            break;

        if (errorRet != SQL_SUCCESS && errorRet != SQL_SUCCESS_WITH_INFO)
            break;

        // TODO : Log
        std::wcout.imbue(std::locale("kor"));
        std::wcout << errMsg << std::endl;

        index++;
    }
}
