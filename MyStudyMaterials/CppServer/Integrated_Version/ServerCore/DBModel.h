#pragma once
#include <memory>
#include "Container.h"
#include "Types.h"
#include "CoreMacro.h"


namespace DBModel
{
/*-------------
	DataType
--------------*/

enum class DataType
{
    None = 0,
    TinyInt = 48,
    SmallInt = 52,
    Int = 56,
    Real = 59,
    DateTime = 61,
    Float = 62,
    Bit = 104,
    Numeric = 108,
    BigInt = 127,
    VarBinary = 165,
    Varchar = 167,
    Binary = 173,
    NVarChar = 231,
};

/*-------------
	Column
--------------*/

class Column
{
public:
    String CreateText();

public:
    String _name;
    int32 _columnId = 0; // DB
    DataType _type = DataType::None;
    String _typeText;
    int32 _maxLength = 0;
    bool _nullable = false;
    bool _identity = false;
    int64 _seedValue = 0;
    int64 _incrementValue = 0;
    String _default;
    String _defaultConstraintName; // DB
};

/*-----------
	Index
------------*/

enum class IndexType
{
    Clustered = 1,
    NonClustered = 2
};

class Index
{
public:
    String GetUniqueName();
    String CreateName(const String& tableName);
    String GetTypeText();
    String GetKeyText();
    String CreateColumnsText();
    bool DependsOn(const String& columnName);

public:
    String _name; // DB
    int32 _indexId = 0; // DB
    IndexType _type = IndexType::NonClustered;
    bool _primaryKey = false;
    bool _uniqueConstraint = false;
    Vector<std::shared_ptr<Column>> _columns;
};

/*-----------
	Table
------------*/

class Table
{
public:
    std::shared_ptr<Column> FindColumn(const String& columnName);

public:
    int32 _objectId = 0; // DB
    String _name;
    Vector<std::shared_ptr<Column>> _columns;
    Vector<std::shared_ptr<Index>> _indexes;
};

/*----------------
	Procedures
-----------------*/

struct Param
{
    String _name;
    String _type;
};

class Procedure
{
public:
    String GenerateCreateQuery();
    String GenerateAlterQuery();
    String GenerateParamString();

public:
    String _name;
    String _fullBody; // DB
    String _body; // XML
    Vector<Param> _parameters; // XML
};

/*-------------
	Helpers
--------------*/

class Helpers
{
public:
    static String Format(const WCHAR* format, ...);
    static String DataType2String(DataType type);
    static String RemoveWhiteSpace(const String& str);
    static DataType String2DataType(const WCHAR* str, OUT int32& maxLen);
};
}
