#pragma once

#include "parser.h"

class ParserExt: public Parser
{
public:
    ParserExt(const std::string &cmd): Parser(cmd) {}
    virtual ~ParserExt() = default;

    std::vector<Column> parseColumnList();
    std::vector<Column> parseSelectList();

    Statement parseSelect();

    Statement parseLoad() ;
    void parseFieldList(std::vector<Field> &fields, std::string &keys);
    void parseField(std::vector<Field> &fields, std::string &keys);
    Statement parseCreate() ;
    Statement parseStatement();
    void parseValueListFromFile(std::vector<std::vector<Variant> > &values);
    std::vector<Variant> parseValueListInFile();
};