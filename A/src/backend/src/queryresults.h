
#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include "fileio.h"
#include "entry.h"
#include "column.h"

/**
 * Base class of query result
 */
class QueryResultBase
{
  public:
    enum Type
    {
        SELECT,
        SHOW_DATABASES,
        SHOW_COLUMNS,
        SHOW_TABLES
    };

    explicit QueryResultBase(Type type) : _type(type) {}
    virtual ~QueryResultBase() = default;
    virtual void print() const = 0;

  protected:
    Type _type;
};

/**
 * Query result class manage query result and prevent memory leak.
 */
class QueryResult
{
  public:
    explicit QueryResult(QueryResultBase *result) : _result(result) {}
    QueryResultBase *result() const { return _result; }
    virtual ~QueryResult() { delete _result; }

  protected:
    QueryResultBase *_result = nullptr;
};

class QueryResultSelect : public QueryResultBase
{
  public:
    explicit QueryResultSelect(std::vector<std::string> keyNames, std::vector<Entry> entries)
        : QueryResultBase(SELECT), _keyNames(std::move(keyNames)), _entries(std::move(entries)) {}

    virtual void print() const override
    {
        if (_entries.empty())
            return;

        for (auto &k : _keyNames)
            std::cout << k << '\t';
        std::cout << std::endl;
        for (auto &r : _entries)
            std::cout << r << std::endl;
    }
    const std::vector<std::string> &keyNames() const { return _keyNames; }
    const std::vector<Entry> &entries() const { return _entries; }

  protected:
    std::vector<std::string> _keyNames;
    std::vector<Entry> _entries;
};
/**
* ext queryresult for into
*/
class QueryResultSelectInto : public QueryResultBase
{
public:
    explicit QueryResultSelectInto(std::vector<std::string> keyNames,
                                   std::vector<Entry> entries,
                                   const std::string *file_name,
                                   const std::vector<Column>& group_by_column,
                                   const std::vector<Column>& order_by_column)
            : QueryResultBase(SELECT), _keyNames(std::move(keyNames)), _entries(std::move(entries)), _group_by_column(group_by_column), _order_by_column(order_by_column)
            {
                this->setFileName(file_name);
            }

    explicit QueryResultSelectInto(const QueryResultSelect &queryResult,
                                   const std::string *file_name,
                                   const std::vector<Column>& group_by_column,
                                   const std::vector<Column>& order_by_column)
            : QueryResultSelectInto(queryResult.keyNames(), queryResult.entries(), file_name, group_by_column, order_by_column)
            {

            }

    ~QueryResultSelectInto()
    {
        delete _file_name;
        decltype(_entries)().swap(_entries);
        decltype(_keyNames)().swap(_keyNames);
    }

    virtual void print() const override
    {
        if (_entries.empty())
            return;
        if (_file_name == nullptr)
        {
            for (auto &k : _keyNames)
                std::cout << k << '\t';
            std::cout << std::endl;
            for (auto &r : _entries)
                std::cout << r << std::endl;
        }
        else
        {
            std::string file_name = *_file_name;

            if (file_name[0] != '/' && file_name[0] != '\\') // not absolute path
                file_name = "__ProgramPath/../" + *_file_name; // go back to the main directory
            std::ofstream file_stream(file_name);
            if (isFile(file_name)) throw std::runtime_error("ERROR : File already exists.");
            for (auto &k : _keyNames)
                file_stream << k << '\t';
            file_stream << std::endl;
            for (auto &r : _entries)
                file_stream << r << std::endl;
        }
    }
    const std::vector<std::string> &keyNames() const { return _keyNames; }
    const std::vector<Entry> &entries() const { return _entries; }
    void setFileName(const std::string *file_name)
    {
        if (file_name != nullptr)
            _file_name = new std::string(*file_name);
        else
            _file_name = nullptr;
    }
    std::string* getFileName() const { return _file_name; }
    const std::vector<Column>& getGroupByColumn() const { return _group_by_column;}
    const std::vector<Column>& getOrderByColumn() const { return _order_by_column; }
protected:
    std::vector<std::string> _keyNames;
    std::vector<Entry> _entries;
    std::string* _file_name;

    std::vector<Column> _group_by_column;
    std::vector<Column> _order_by_column;
};

class QueryResultShowDatabases : public QueryResultBase
{
  public:
    explicit QueryResultShowDatabases(std::vector<std::string> databaseNames)
        : QueryResultBase(SHOW_DATABASES), _dbNames(std::move(databaseNames)){};
    const std::vector<std::string> &databaseNames() const { return _dbNames; }
    virtual void print() const override
    {
        std::cout << "Database" << std::endl;
        for (auto &name : _dbNames)
            std::cout << name << std::endl;
    }

  protected:
    std::vector<std::string> _dbNames;
};

class QueryResultShowTables : public QueryResultBase
{
  public:
    explicit QueryResultShowTables(std::string databaseName, std::vector<std::string> tableNames)
        : QueryResultBase(SHOW_TABLES), _dbName(std::move(databaseName)), _tableNames(std::move(tableNames)) {}
    const std::string &databaseName() const { return _dbName; }
    const std::vector<std::string> &tableNames() const { return _tableNames; }
    virtual void print() const override
    {
        std::cout << "Tables_in_" << _dbName << std::endl;
        auto tNames = tableNames();
        for (auto &t : _tableNames)
            std::cout << t << std::endl;
    }

  protected:
    std::string _dbName;
    std::vector<std::string> _tableNames;
};

class QueryResultShowColumns : public QueryResultBase
{
  public:
    explicit QueryResultShowColumns(std::vector<Field> fields) : QueryResultBase(SHOW_COLUMNS), _fields(std::move(fields)) {}
    virtual void print() const override
    {
        std::cout << "Field\tType\tNull\tKey\tDefault\tExtra" << std::endl;
        for (auto &f : _fields)
            std::cout << f.key() << '\t' << f.type() << '\t' << (f.isNull() ? "YES" : "NO") << '\t'
                      << (f.isPrimary() ? "PRI" : "") << "\tNULL" << std::endl;
    }

  protected:
    std::vector<Field> _fields;
};
