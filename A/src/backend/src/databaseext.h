#pragma once

#include "database.h"

class DatabaseExt: public Database
{
public:
    DatabaseExt(): Database() {}
    virtual ~DatabaseExt() = default;

    QueryResult selectAllFrom(const std::string &tableName, const std::vector<std::string> &fileNames, const Expr &expr, const std::string* file_name);
    QueryResult selectFrom(const std::string &tableName, const std::vector<std::string> &fieldNames, const Expr &expr, const std::string* file_name);

    void getCountNames(const std::vector<std::string> &keyNames,
                      std::vector<std::string> &_keyNames,
                      std::vector<std::string> &_cntNames,
                      std::vector<size_t> &cntPos);
    void countEntries(const std::vector<std::string> &keyNames,
                      const std::vector<std::string> &cntNames,
                      const std::vector<size_t> &cntPos,
                      const std::vector<Entry> &entries,
                      std::vector<Entry> &resultEntries);

    void load(const std::string &tableName, const std::vector< std::map<std::string, Variant> > &entries);
};