#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "token.h"

/**
 * Expression class with a tree-like structure.
 *
 * Member `_left` and `_right` are pointers to the left and right sub expressions.
 * Use `hasLeft()` or `hasRight()` to judge whether an expression has sub expressions
 * Use `left()` or `right()` to get sub expressions.
 * Member _token is the type indicator of the expression.
 * There are 3 types of expressions:
 * 1. Binary expression. `_left` and `_right` are pointers to
 *    left and right expressions, `_token.type()` is the binary operator.
 * 2. Unary expression. `_left` is nullptr, `_right` is the right operand,
 *    `_token.type()` is the unary operator.
 * 3. Operand. `_token.type()` is ID or OPERAND, `_left` and `_right` are nullptr.
 */
class Expr
{
  public:
    Expr() = default;
    Expr(const Token &t) : _token(t) {}
    Expr(const Token &self, const Expr &left, const Expr &right);
    Expr(const Expr &other);
    Expr(Expr &&other);

    virtual ~Expr();

    Expr &operator=(const Expr &rhs);

    //Set left expressions with other Expr's left expressions.
    void setLeft(const Expr &other);
    //Set right expressions with other Expr's right expressions.
    void setRight(const Expr &other);

    void setToken(const Token &t) { _token = t; }
    bool isNull() const { return _token.type() == Token::NONE; }

    //Return true if the expression is an id or a number.
    bool isValue() const { return _token.type() == Token::ID || _token.type() == Token::OPERAND; }

    bool hasLeft() const { return _left != nullptr; }
    bool hasRight() const { return _right != nullptr; }
    const Expr &left() const { return *_left; }
    const Expr &right() const { return *_right; }

    const Token &token() const { return _token; }
    Token::Type tokenType() const { return _token.type(); }

    //Return the value of the expressions.
    Variant eval(const std::map<std::string, Variant> &varMap) const;

    friend std::ostream &operator<<(std::ostream &s, const Expr &expr);

  protected:
    Expr *_left = nullptr;
    Expr *_right = nullptr;
    Token _token;
};
