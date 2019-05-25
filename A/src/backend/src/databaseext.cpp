#include "databaseext.h"
QueryResult DatabaseExt::selectAllFromInto(const std::string &tableName, const Expr &expr)
{
    return QueryResultSelectInto(Database::selectAllFrom(const std::string &tableName, const Expr &expr))
}

QueryResult DatabaseExt::selectFromInto(const std::string &tableName, const std::vector<std::string> &keyNames, const Expr &expr)
{
    return QueryResultSelectInto(Database::selectFrom(const std::string &tableName, const std::vector<std::string> &keyNames, const Expr &expr))
}