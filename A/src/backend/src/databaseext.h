#pragma once

#include "database.h"
#include "column.h"

typedef std::vector<Entry> Group;

class DatabaseExt: public Database
{
public:
    DatabaseExt(): Database() {}
    virtual ~DatabaseExt() = default;

    QueryResult selectAllFrom(const std::string &tableName, const std::vector<Column> &columns,
                              const Expr &expr, const std::string* fileName,
                              const std::vector<Column>& groupByColumns,
                              const std::vector<Column> &orderByColumns);

    QueryResult selectFrom(const std::string &tableName,
                           const std::vector<Column> &keyNames, const Expr &expr, const std::string* fileName,
                           const std::vector<Column> &groupByColumn, const std::vector<Column>& orderByColumn);

    QueryResult selectFromMultTables(const std::vector<std::string> &tableNames,
                           const std::vector<Column> &keyNames, const Expr &expr, const std::string* fileName,
                           const std::vector<Column> &groupByColumn, const std::vector<Column>& orderByColumn);

    void InsertEntry(Table& currentTable, const std::vector<std::string>& tableNames, int tableNum, std::vector<Entry>& entries);

    void gatherEntries(std::vector<Column> columns,
                       std::vector<std::string> keyNames,
                       const std::vector<Entry> &entries,
                       Entry &resultEntries);
    void gatherEntriesInGroup(const std::vector<Column> &columns,
                              const std::vector<std::string> &keyNames,
                              const std::vector<Group> &groups,
                              std::vector<Entry> &resultEntries,
                              bool isGatherSelect);
    void orderEntriesBy(std::vector<Entry> &entries, const std::vector<Column> &columns, const std::vector<Column> &orderByColumns);

    void makeGroupBy(const std::vector<Column> &groupByKey,
                     const std::vector<std::string> &keyNames,
                     const Group &entries, std::vector<Group> &groups);

    //bool dropDatabase(const std::string &dbName);

    void load(const std::string &tableName, const std::vector< std::map<std::string, Variant> > &entries);

protected:

    virtual void gatherFunction(const Token::Type &gatherType,
                                const std::vector<Entry> &entries,
                                int columnPos, Variant &gatherResult);
};