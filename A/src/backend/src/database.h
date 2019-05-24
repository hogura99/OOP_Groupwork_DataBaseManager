#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <exception>
#include "utils.h"
#include "simsql.h"
#include "table.h"
#include "print.h"
#include "field.h"
#include "datastream.h"
#include "page.h"
#include "fileio.h"
#include "expr.h"
#include "queryresults.h"
#include "databaseexcept.h"

/**
 * Database class provides methods of database and table operation.
 *
 * Support creating / showing / dropping / using database
 *
 * When creating a database, it creates a directory of the database name under ./data/
 * When using a database, it changes current directory to the database directory.
 * When dropping a database, it remove the database directory and all its containing files.
 *
 * Provides API for table operations
 * Support creating / showing / dropping / inserting / updating / deleting / selecting from tables.
 * In terms of table operations, it calls Table class to execute.
 */
class Database
{
  public:
    Database();
    virtual ~Database() = default;

    // database operation
    bool isDatabase(const std::string &dbName) const;
    std::vector<std::string> databaseNames() const;
    void useDatabase(const std::string &dbName);
    void createDatabase(const std::string &dbName);
    bool dropDatabase(const std::string &dbName);
    QueryResult showDatabases() const;

    // table operation
    bool isTable(const std::string &tableName) const { return isFile(tableName + SimSql::suffixIndex) && isFile(tableName + SimSql::suffixDB); }
    std::vector<std::string> tableNames() const;
    void createTable(const std::string &tableName, const std::vector<Field> &fields, const std::string &primaryKey);
    QueryResult showColumnsFrom(const std::string &tableName);
    QueryResult showTables() const;
    void dropTable(const std::string &tableName);
    void insertInto(const std::string &tableName, const std::map<std::string, Variant> &entry);
    QueryResult selectAllFrom(const std::string &tableName, const Expr &expr);
    QueryResult selectFrom(const std::string &tableName, const std::vector<std::string> &fieldNames, const Expr &expr);
    void update(const std::string &tableName, const std::vector<std::string> &keys, const std::vector<Variant> &values, const Expr &expr);
    void deleteFrom(const std::string &tableName, const Expr &expr);

  protected:
    void assertDatabaseExist(const std::string &dbName) const;
    void assertDatabaseNotExist(const std::string &dbName) const;
    void assertTableExist(const std::string &tableName) const;
    void assertTableNotExist(const std::string &tableName) const;
    void assertDatabaseSelected() const;

  protected:
    std::string _dbName;
    std::string _dataDir;
    std::map<std::string, Table> _tables;
};
