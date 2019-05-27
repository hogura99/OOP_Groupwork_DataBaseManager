#pragma once

#include "database.h"

class DatabaseExt: public Database
{
public:
    DatabaseExt();
    virtual ~DatabaseExt() = default;
    QueryResult selectAllFromInto(const std::string &tableName, const Expr &expr, const std::string* file_name);
    QueryResult selectFromInto(const std::string &tableName, const std::vector<std::string> &fieldNames, const Expr &expr, const std::string* file_name);
};