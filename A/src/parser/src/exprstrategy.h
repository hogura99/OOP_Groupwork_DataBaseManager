#pragma once

#include <map>

#include "expr.h"

class ExprPrint: public Expr
{
public:
    ExprPrint(const Expr &other): Expr(other) {}
    friend std::ostream &operator<<(std::ostream &out, const ExprPrint &expr);
    static std::map<Token::Type, std::string> whereOps;
};