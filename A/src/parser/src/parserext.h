#pragma once

#include "parser.h"

class ParserExt: public Parser
{
public:
    ParserExt(const std::string &cmd): Parser(cmd) {}
    virtual ~ParserExt() = default;
    Statement parseStatement();
    Statement parseSelect() ;
    Statement parseLoad() ;
    void parseValueListFromFile(std::vector<std::vector<Variant> > values);
    std::vector<Variant> parseValueListInFile();
};