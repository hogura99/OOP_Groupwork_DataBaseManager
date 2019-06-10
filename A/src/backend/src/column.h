#pragma once

#include <string>
#include <vector>

#include "token.h"

struct Column
{
    std::string tableName;
    std::string columnName;
    Token::Type type;
};

/*
class Column
{
public:
    Column(const std::string &tableName, const std::string &columnName, const Token::Type &type):
           _tableName(tableName), _columnName(columnName), _type(type) {}
    virtual ~Column() {}

    Token::Type getType() const ;
    std::string getTableName() const ;
    std::string getColumnName() const ;

private:
    std::string _tableName;
    std::string _columnName;
    Token::Type _type;
};*/
