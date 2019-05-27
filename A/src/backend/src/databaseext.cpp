#include "databaseext.h"
QueryResult DatabaseExt::selectAllFromInto(const std::string &tableName, const Expr &expr, const std::string* file_name)
{
	auto ret = QueryResultSelectInto(Database::selectAllFrom(tableName, expr));
	if(!file_name)
		ret.setFileName(*file_name);
    return ret;
}

QueryResult DatabaseExt::selectFromInto(const std::string &tableName, const std::vector<std::string> &keyNames, const Expr &expr, const std::string* file_name)
{
	auto ret = QueryResultSelectInto(Database::selectFrom(tableName, keyNames, expr));
	if(!file_name)
		ret.setFileName(*file_name);
    return ret;
}