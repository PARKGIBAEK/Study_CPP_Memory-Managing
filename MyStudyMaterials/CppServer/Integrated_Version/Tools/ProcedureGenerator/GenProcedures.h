#pragma once
#include "Types.h"
#include <windows.h>
#include "DBBind.h"

namespace SP
{
	

    class InsertGold : public DBBind < 3, 0 >
    {
    public:
    InsertGold(DBConnection & conn) : DBBind(conn, L"{CALL dbo.spInsertGold(?,?,?)}") { }
    void In_Gold(int32&v) { BindParam(0, v); };
    void In_Gold(int32 && v) { _ g old  = std::move(v); BindParam(0, _ g old ); };
    template<int32 N> void In_Name(WCHAR(&v)[N]) { BindParam(1, v); };
    template<int32 N> void In_Name(const WCHAR(&v)[N]) { BindParam(1, v); };
    void In_Name(WCHAR * v, int32 count) { BindParam(1, v, count); };
    void In_Name(const WCHAR * v, int32 count) { BindParam(1, v, count); };
    void In_CreateDate(TIMESTAMP_STRUCT&v) { BindParam(2, v); };
    void In_CreateDate(TIMESTAMP_STRUCT && v) { _ c reateDate  = std::move(v); BindParam(2, _ c reateDate ); };

    private:
    int32 _ g old  = {};
    TIMESTAMP_STRUCT _ c reateDate  = {};
    };

    class GetGold : public DBBind < 1, 4 >
    {
    public:
    GetGold(DBConnection & conn) : DBBind(conn, L"{CALL dbo.spGetGold(?)}") { }
    void In_Gold(int32&v) { BindParam(0, v); };
    void In_Gold(int32 && v) { _ g old  = std::move(v); BindParam(0, _ g old ); };
    void Out_Id(OUTint32&v) { BindCol(0, v); };
    void Out_Gold(OUTint32&v) { BindCol(1, v); };
    template<int32 N> void Out_Name(OUT WCHAR(&v)[N]) { BindCol(2, v); };
    void Out_CreateDate(OUTTIMESTAMP_STRUCT&v) { BindCol(3, v); };

    private:
    int32 _ g old  = {};
    }; 
};