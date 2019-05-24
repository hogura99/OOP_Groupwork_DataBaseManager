#pragma once

#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <map>
#include "../../backend/src/variant.h"

class TokenError : std::exception
{
  public:
    TokenError(const std::string &msg) : _msg("Token error: " + msg) {}
    ~TokenError() throw() {}
    const char *what() const throw() { return _msg.c_str(); }

  protected:
    std::string _msg;
};

/**
 * @brief A container for every single word(divided by space) in the statements.
 *
 * There are Four _type of tokens:
 *     1. Keyword. `_type` is its _type (e.g. CREATE), `data`=NULL.
 *         Use `Type toKeyword()` to get it.
 *     2. Operator. `_type` is its _type (e.g. EQ), `data`=NULL.
 *         Use `Type toKeyword()` to get it.
 *     3. Identifier. `_type`is ID, `data` contains the raw string.
 *         Use `string& toId()` to get it.
 *     4. Operand. `_type` is OPERAND, `data` contains an int, double or char,
 *         Use `Variant toOperand()` to get it.
 */
class Token
{
  public:
    enum Type
    {
        NONE,
        ID,
        OPERAND,
        CHAR,
        DOUBLE,
        INT,
        DATABASE,
        DATABASES,
        TABLE,
        TABLES,
        CREATE,
        INSERT,
        SELECT,
        DELETE,
        SHOW,
        DROP,
        UPDATE,
        USE,
        DEFAULT,
        PRIMARY,
        KEY,
        FROM,
        INTO,
        SET,
        VALUES,
        WHERE,
        COLUMNS,
        LT,
        GT,
        NEQ,
        EQ,
        GEQ,
        LEQ,
        NOT,
        NULL_SQL,
        PLUS,
        MINUS,
        MUL,
        DIV,
        AND,
        OR,
        L_PAREN,
        R_PAREN,
        COMMA,
        SEMICOLON,
        END,
    };
    Token(Type type = NONE, const Variant &data = Variant());
    Token(const Token &other) : _type(other._type), _data(other._data) {}
    Token(Token &&other) : _type(std::move(other._type)), _data(std::move(other._data)) {}
    virtual ~Token() {}

    // Return the type of the _token.
    Token::Type type() const { return _type; }

    // For identifiers.
    const std::string &toId() const;
    // For keywords and operators.
    Type toKeyword() const;
    // For operands.
    Variant toOperand() const;

    Token &operator=(const Token &rhs);
    Token &operator=(Token &&rhs);
    friend std::ostream &operator<<(std::ostream &s, const Token &token);

  public:
    static std::map<Type, std::string> name; // uppercase name of _types

  protected:
    Type _type;
    Variant _data;
};

static std::ostream &operator<<(std::ostream &out, const Token::Type &t)
{
    if (Token::name.count(t))
        return out << Token::name[t];
    else
        return out << (int)t;
}
