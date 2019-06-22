#include "exprstrategy.h"
#include "lexer.h"

std::map<Token::Type, std::string> ExprPrint::whereOps =
{
    {Token::NOT, "!"},
    {Token::LT, "<"},
    {Token::GT, ">"},
    {Token::NEQ, "!="},
    {Token::EQ, "="},
    {Token::GEQ, ">="},
    {Token::LEQ, "<="},
    {Token::PLUS, "+"},
    {Token::MINUS, "-"},
    {Token::MUL, "*"},
    {Token::DIV, "/"},
    {Token::MOD, "%"},
    {Token::L_PAREN, "("},
    {Token::R_PAREN, ")"},
    {Token::COMMA, ","},
    {Token::SEMICOLON, ";"},
    {Token::LIKE, "like"},
    {Token::AND, "and"},
    {Token::OR, "or"},
    {Token::XOR, "xor"},
};

std::ostream &operator<<(std::ostream &out, const ExprPrint &expr)
{
    if (expr.isValue())
    { // id or number
        if (expr.tokenType() == Token::ID)
            out << expr.token().toId();
        else
            out << expr.token().toOperand();
        return out;
    }

    // real expression
    out << "(";
    if (expr.hasLeft())
        out << ExprPrint(expr.left());

    if (!expr.isNull()) // operator
    {
        out << " " << ExprPrint::whereOps[expr.tokenType()] << " ";
    }

    if (expr.hasRight())
        out << ExprPrint(expr.right());

    out << ")";
    return out;
}