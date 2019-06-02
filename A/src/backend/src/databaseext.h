#pragma once

#include "database.h"

class DatabaseExt: public Database
{
public:
    DatabaseExt(): Database() {}
    virtual ~DatabaseExt() = default;
    QueryResult selectAllFrom(const std::string &tableName, const Expr &expr, const std::string* file_name);
    QueryResult selectFrom(const std::string &tableName, const std::vector<std::string> &fieldNames, const Expr &expr, const std::string* file_name);
    void load(const std::string &tableName, const std::vector< std::map<std::string, Variant> > &entries);
};