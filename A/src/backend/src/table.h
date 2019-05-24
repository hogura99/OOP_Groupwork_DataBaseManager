#pragma once
#include <utility>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <cassert>
#include "entry.h"
#include "field.h"
#include "print.h"
#include "simsql.h"
#include "datastream.h"
#include "page.h"
#include "utils.h"
#include "fileio.h"
#include "databaseexcept.h"
#include "expr.h"
#include "queryresults.h"

/**
 * Table class provides methods for table management and operation
 *
 * Support creating / showing / dropping / inserting / updating / deleting / selecting from tables.
 * When creating a table, it creates 3 files under the current database directory
 * 1. info file  (.info) : it stores the basic framework of the table, including fields info, primary key name, etc.
 * 2. index file (.idx)  : it stores the all entries' primary keys and the corresponding positions in the data file.
 * 3. data file  (.data) : it stores the data of all entries.
 * When insert / selecting / updating / deleting from a table, it loads the index file,
 * and searches for all the entries in the index file that meet the condition of the where clause.
 * Then search for the entire entry in the data file and do specific operations.
 * Return QueryResult if needed.
 *
 * Input & output methods are implemented. Support std::cout and DataStream I/O.
 */
class Table
{
  public:
    Table() = default;
    Table(std::string tableName) : _name(std::move(tableName)) {}
    const std::string &name() const { return _name; }
    std::map<Variant, EntryPos> &entryPos() { return _entryPos; }
    const std::vector<Field> &fields() const { return _fields; }
    std::map<std::string, Variant> entryToMap(const Entry &entry);

    void loadIndex();
    void loadInfo();
    void insertInto(const Entry &entry);
    void insertInto(const std::map<std::string, Variant> &entry);
    void createTable(const std::vector<Field> &fields, const std::string &primaryKey);
    QueryResult selectAllFrom(const Expr &expr);
    QueryResult selectFrom(const std::vector<std::string> &keyNames, const Expr &expr);
    std::vector<Field> columns() { return _fields; }
    QueryResult showColumns();
    void update(const std::vector<std::string> &keys, const std::vector<Variant> &values, const Expr &expr);
    void deleteWhere(const Expr &expr);

    friend DataStream &operator<<(DataStream &out, const Table &table)
    {
        return out << table._name << table._fields << table._priKeyName << table._entryPos << table._keyToRank;
    }
    friend DataStream &operator>>(DataStream &in, Table &table)
    {
        return in >> table._name >> table._fields >> table._priKeyName >> table._entryPos >> table._keyToRank;
    }
    friend std::ostream &operator<<(std::ostream &out, const Table &table)
    {
        return out << "Table(" << table._name << ',' << table._fields << ',' << table._priKeyName << ',' << table._entryPos << ')';
    }

  protected:
    std::string _name;
    std::vector<Field> _fields;
    std::string _priKeyName;
    std::map<Variant, EntryPos> _entryPos;
    std::map<std::string, size_t> _keyToRank;
};
