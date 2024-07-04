#pragma once
#include <memory>
#include "../DB/DBConnection.h"
#include "../DB/DBModel.h"

/*--------------------
	DBSynchronizer
---------------------*/
namespace ServerDb
{
using namespace ServerCore;

class DBSynchronizer
{
	enum
	{
		PROCEDURE_MAX_LEN = 10000
	};

	enum UpdateStep : uint8
	{
		DropIndex,
		AlterColumn,
		AddColumn,
		CreateTable,
		DefaultConstraint,
		CreateIndex,
		DropColumn,
		DropTable,
		StoredProcecure,

		Max
	};

	enum ColumnFlag : uint8
	{
		Type = 1 << 0,
		Nullable = 1 << 1,
		Identity = 1 << 2,
		Default = 1 << 3,
		Length = 1 << 4,
	};

	

public:
	DBSynchronizer(DbConnection& conn) : _dbConn(conn) { }
	~DBSynchronizer();

	bool		Synchronize(const WCHAR* path);

private:
	void		ParseXmlDB(const WCHAR* path);
	bool		GatherDBTables();
	bool		GatherDBIndexes();
	bool		GatherDBStoredProcedures();

	void		CompareDBModel();
	void		CompareTables(std::shared_ptr<Table> dbTable, std::shared_ptr<Table> xmlTable);
	void		CompareColumns(std::shared_ptr<Table> dbTable, std::shared_ptr<Column> dbColumn, std::shared_ptr<Column> xmlColumn);
	void		CompareStoredProcedures();

	void		ExecuteUpdateQueries();

private:
	DbConnection& _dbConn;

	// XML상의 Table & Stored Procedure 정보
	Vector<std::shared_ptr<Table>>			_xmlTables;
	Vector<std::shared_ptr<Procedure>>		_xmlProcedures;
	// 삭제되어야 할 항목
	Set<String>							_xmlRemovedTables;

	// DB의 현재 Table & Stored Procedure 정보
	Vector<std::shared_ptr<Table>>			_dbTables;
	Vector<std::shared_ptr<Procedure>>		_dbProcedures;

private:
	Set<String>							_dependentIndexes;
	Vector<String>						_updateQueries[UpdateStep::Max];
};
}