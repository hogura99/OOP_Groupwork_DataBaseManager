#pragma once

#include "token.h"
#include <sstream>
#include <cstdio>
#include <cctype>

#define BUF_SIZE 256
#define MAX_OP_SIZE 2

class LexerError : std::exception
{
  public:
    LexerError(const std::string &msg) : _msg("Lexer error: " + msg) {}
    virtual ~LexerError() noexcept override = default;
    virtual const char *what() const noexcept override { return _msg.c_str(); }

  protected:
    std::string _msg;
};

/**
* @brief A lexer for the statements.
*
* The lexer records the current scanning status. 
* Use `Token next()` to get the next token to be scanned.
*/
class Lexer
{
  public:
    Lexer(const std::string &cmd) : _stream(cmd) {}

    // Return true if ch is an operator.
    bool isOp(char ch) const;
    // Return true if the stream is empty.
    bool isEnd() { return (_peek = _stream.peek()) == EOF; }
    int columnNo() const { return _col; }
    int lineNo() const { return _line; }

    // Return the next _token to be scanned.
    Token next();

    // Advance in the stream, return the read character.
    char advance();

    // Put a char back in the stream.
    void retreat(char ch);

  public:
    static std::map<std::string, Token::Type> keywords; // map for keywords
    static std::map<char, Token::Type> singleOp;        // map for operators that can be determined by one character
    static std::map<std::string, Token::Type> ops;      // map for other operators

  protected:
    char _peek;             // next character to read by advance()
    char _buffer[BUF_SIZE]; // a buffer for keywords, identifiers, etc.
    // The line number and Column number of the current char under processing. Use `int columnNo()` and `int lineNo()` to get them.
    int _line = 1;
    int _col = 1;
    std::istringstream _stream; // the input stream
};
