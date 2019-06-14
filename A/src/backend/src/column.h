#pragma once

#include <string>
#include <vector>

#include "token.h"

class Column
{
public:
    std::string table;
    std::string name;
    Token::Type type;

    Column(std::string table, std::string name, Token::Type type):
        table(table), name(name), type(type) {}
    Column(): table(""), name(""), type(Token::NONE) {}

    bool operator==(const Column &x) const
    {
        return table == x.table && name == x.name && type == x.type;
    }

    bool operator<(const Column &x) const
    {
        return table == x.table ? name < x.name : table < x.table;
    }

    friend std::string trans2Str(Column x)
    {
        std::string s;
        if (Token::name.count(x.type) && x.type != Token::ID)
            s += Token::name[x.type] + "(";

        if (x.table != "")
            s += x.table + ".";
        s += x.name;

        if (Token::name.count(x.type) && x.type != Token::ID)
            s += ")";
        return s;
    }
};