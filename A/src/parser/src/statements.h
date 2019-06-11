#pragma once

#include "expr.h"
#include <set>
#include <iostream>
#include <string>
#include <map>
#include "../../backend/src/print.h"
#include "../../backend/src/field.h"
#include "../../backend/src/column.h"

/**
* @brief A basic container for all kinds of statements.  
*
* This is a basic class for statements, which consists of the type and identifier of the statement.
* Create the class for a specific kind of statement by inheriting from this basic class.
*/
class StatementBase
{
  public:
    enum StatementType
    {
        NONE = 0,
        CREATE_TABLE,
        CREATE_DATABASE,
        SHOW_TABLES,
        SHOW_DATABASES,
        SHOW_COLUMNS,
        USE_DATABASE,
        DROP_TABLE,
        DROP_DATABASE,
        SELECT,
        INSERT,
        UPDATE,
        DELETE,
        INTO_OUTFILE,
        LOAD,
        COUNT
    };
    StatementBase(const std::string &id, StatementType type) : _id(id), _type(type) {}
    virtual const std::string &id() const { return _id; }
    virtual StatementType type() const { return _type; }

    // Return the type name as a string.
    virtual std::string typeString() const { return typeStringMap[_type]; }
    // Output the statement info.
    virtual void print() const
    {
        std::cout << "StatementBase " << typeString() << " (id:" << _id << " type:" << _type << ')' << std::endl;
    }
    friend std::ostream &operator<<(std::ostream &out, const StatementBase &s)
    {
        return out << "StatementBase " << s.typeString() << " (id:" << s._id << " type:" << s._type << ')';
    }

  public:
    static std::map<StatementType, std::string> typeStringMap;

  protected:
    std::string _id;            // Identifier of the corresponding table or database. Use `std::string & id()` to get it.
    StatementType _type = NONE; // Type of the statement. Use `StatementType type()` to get it.
};

class Statement
{
  public:
    Statement(StatementBase *content) : _content(content) {}
    virtual ~Statement() { delete _content; }
    StatementBase *content() const { return _content; }

  protected:
    StatementBase *_content = nullptr;
};

/**
* Create table statement. Consists of id, fields and a primary key.
*/
class StatementCreateTable : public StatementBase
{
  public:
    StatementCreateTable(const std::string id, const std::vector<Field> &fields, const std::string &priKey)
        : StatementBase(id, CREATE_TABLE), _fields(fields), _priKey(priKey) {}

    const std::vector<Field> &fields() const { return _fields; }
    const std::string &primaryKey() const { return _priKey; }

    virtual void print() const override
    {
        StatementBase::print();
        std::cout << "Primary Key " << _priKey << "\nFields " << _fields << std::endl;
    }
    friend std::ostream &operator<<(std::ostream &out, const StatementCreateTable &s)
    {
        return out << (StatementBase)(s) << "\nPrimary Key " << s._priKey << "\nFields " << s._fields;
    }

  protected:
    const std::vector<Field> _fields;
    std::string _priKey;
};

/**
* Create database statement. Consists of id.
*/
class StatementCreateDatabase : public StatementBase
{
  public:
    StatementCreateDatabase(const std::string &id_) : StatementBase(id_, CREATE_DATABASE) {}
};

/**
 * Insert statement. Consists of id, columns and values(paired with index).
 */
class StatementInsert : public StatementBase
{
  public:
    StatementInsert(const std::string id, const std::map<std::string, Variant> &entry) : StatementBase(id, INSERT), _entry(entry) {}

    const std::map<std::string, Variant> &entry() const { return _entry; }

    virtual void print() const override
    {
        StatementBase::print();
        std::cout << "Entry:" << _entry << std::endl;
    }
    friend std::ostream &operator<<(std::ostream &out, const StatementInsert &s)
    {
        return out << (StatementBase)(s) << std::endl
                   << "Entry:" << s._entry;
    }

  protected:
    std::map<std::string, Variant> _entry;
};

/**
 * Delete statement. Consists of id and a where expression.
 */
class StatementDelete : public StatementBase
{
  public:
    StatementDelete(const std::string id, const Expr &where) : StatementBase(id, DELETE), _where(where) {}
    const Expr &getWhere() const { return _where; }
    virtual void print() const override
    {
        StatementBase::print();
        std::cout << "where clause:" << _where << std::endl;
    }
    friend std::ostream &operator<<(std::ostream &out, const StatementDelete &s)
    {
        return out << (StatementBase)s << std::endl
                   << "where clause:" << s._where;
    }

  protected:
    Expr _where;
};

/**
 * Select statement. Consists of id, columns, and a where expression.
 */
class StatementSelect : public StatementBase
{
  public:
    StatementSelect(const std::string id, const std::vector<std::string> columns, const Expr &where)
        : StatementBase(id, SELECT), _columns(columns), _where(where) {}

    const std::vector<std::string> &getColumns() const { return _columns; }
    const Expr &getWhere() const { return _where; }
    virtual void print() const override
    {
        StatementBase::print();
        std::cout << "Columns:" << _columns << std::endl
                  << "where clause:" << _where << std::endl;
    }
    friend std::ostream &operator<<(std::ostream &out, const StatementSelect &s)
    {
        return out << (StatementBase)s << std::endl
                   << "Columns:" << s._columns << std::endl
                   << "where clause:" << s._where << std::endl;
    }

  protected:
    std::vector<std::string> _columns; // can contain '*'
    Expr _where;
};
/**
* ext StatementSelect new
*/
class StatementSelectInto : public StatementBase
{
public:
    StatementSelectInto(const std::string& id ,const std::string& file_name, const std::vector<Column>& group_by_column, const std::vector<Column>& order_by_column,const std::vector<Column>& columns, const Expr &where)
            : StatementBase(id, SELECT), _columns(columns), _where(where), _file_name(file_name), _group_by_column(group_by_column), _order_by_column(order_by_column)
            {

            }

    const std::vector<Column> &getColumns() const { return _columns; }
    const Expr &getWhere() const { return _where; }
    const std::string* getFilename() const
    {
        if (_file_name == "")
            return nullptr;
        else
            return &_file_name;
    }
    virtual void print() const override
    {
        StatementBase::print();
        std::cout << "Columns:" << _columns << std::endl
                  << "where clause:" << _where << std::endl;
    }
    friend std::ostream &operator<<(std::ostream &out, const StatementSelectInto &s)
    {
        return out << (StatementBase)s << std::endl
                   << "Columns:" << s._columns << std::endl
                   << "where clause:" << s._where << std::endl;
    }
    const std::vector<Column>& getGroupByColumn() const { return _group_by_column; }
    const std::vector<Column>& getOrderByColumn() const { return _order_by_column; }

    bool isSelectAll() const
    {
        if (_columns.front().columnName == "*")
            return true;
        // TODO: change the column names of StatementSelectInto.

        for (auto column: _columns)
            if (column.columnName[0] != '\\')
                return false;
        return true;
    }
protected:
    std::vector<Column> _columns; // can contain '*'
    std::string _file_name;
    std::vector<Column> _group_by_column, _order_by_column;
    Expr _where;
    bool _select_all;
};
/**
* Update statement. Consists of id, keys, values and a where expression.
*/
class StatementUpdate : public StatementBase
{
  public:
    StatementUpdate(const std::string &id, const std::vector<std::string> &keys, const std::vector<Variant> &values, const Expr &whereClause)
        : StatementBase(id, UPDATE), _keys(keys), _values(values), _where(whereClause) {}

    const std::vector<std::string> &keys() const { return _keys; }
    const std::vector<Variant> &values() const { return _values; }
    const Expr &whereClause() const { return _where; }

    virtual void print() const override
    {
        StatementBase::print();
        std::cout << "set keys:" << _keys << " values:" << _values << "\nwhere clause" << _where << std::endl;
    }
    friend std::ostream &operator<<(std::ostream &out, const StatementUpdate &s)
    {
        return out << "set keys:" << s._keys << " values:" << s._values << "\nwhere clause" << s._where;
    }

  protected:
    std::vector<std::string> _keys;
    std::vector<Variant> _values;
    Expr _where;
};

class StatementLoad : public StatementBase
{
  public:
    StatementLoad(std::string tableName, std::vector< std::map<std::string, Variant> > entries) 
    : _entries(entries), StatementBase(tableName, LOAD) {}
    const std::vector< std::map<std::string, Variant> > entries() { return _entries; }

  protected:
    
    std::vector< std::map<std::string, Variant> > _entries;
};

/**
* Drop, show and use statements for table, database or column. Consists of id.
*/

class StatementDropTable : public StatementBase
{
  public:
    StatementDropTable(const std::string &id) : StatementBase(id, DROP_TABLE) {}
};

class StatementDropDatabase : public StatementBase
{
  public:
    StatementDropDatabase(const std::string &id) : StatementBase(id, DROP_DATABASE) {}
};

class StatementShowDatabases : public StatementBase
{
  public:
    StatementShowDatabases() : StatementBase(std::string(), SHOW_DATABASES) {}
};

class StatementShowTables : public StatementBase
{
  public:
    StatementShowTables() : StatementBase(std::string(), SHOW_TABLES) {}
};

class StatementShowColumns : public StatementBase
{
  public:
    StatementShowColumns(const std::string &id) : StatementBase(id, SHOW_COLUMNS) {}
};

class StatementUseDatabase : public StatementBase
{
  public:
    StatementUseDatabase(const std::string &id) : StatementBase(id, USE_DATABASE) {}
};
