#pragma once

#include "parser.h"

class ParserExt: public Parser
{
public:
    ParserExt(const std::string &cmd): Parser(cmd) {}
    virtual ~ParserExt() = default;

    std::vector<std::string> parseColumnList();
    std::vector<std::string> parseSelectList();

    Statement parseSelect();
    Statement parseLoad() ;

    Statement parseStatement();
    void parseValueListFromFile(std::vector<std::vector<Variant> > values);
    
};