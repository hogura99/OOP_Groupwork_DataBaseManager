
#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include "entry.h"

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
    explicit QueryResultSelectInto(std::vector<std::string> keyNames, std::vector<Entry> entries)
            : QueryResultBase(SELECT), _keyNames(std::move(keyNames)), _entries(std::move(entries)) {_file_name = NULL;}

    virtual void print() const override
    {
        std::ofstream file_stream(_file_name);
        if (_entries.empty())
            return;

        for (auto &k : _keyNames)
            file_stream << k << '\t';
        file_stream << std::endl;
        for (auto &r : _entries)
            file_stream << r << std::endl;
    }
    const std::vector<std::string> &keyNames() const { return _keyNames; }
    const std::vector<Entry> &entries() const { return _entries; }
    void setFileName(const std::string file_name){_file_name = file_name;}
protected:
    std::vector<std::string> _keyNames;
    std::vector<Entry> _entries;
    std::string _file_name;
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
