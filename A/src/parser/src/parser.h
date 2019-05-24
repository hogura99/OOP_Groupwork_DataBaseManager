#pragma once

#include "lexer.h"
#include "expr.h"
#include "statements.h"
#include "../../backend/src/print.h"

class ParserError : public std::exception
{
  public:
    ParserError(const std::string &msg) : _msg("Parser Error: " + msg) {}
    virtual ~ParserError() noexcept override = default;
    virtual const char *what() const noexcept override { return _msg.c_str(); }

  protected:
    std::string _msg;
};

/**
 * A parser for the statements.
 *
 * Member opPrioMap specifies the priority between different operators.
 * Member _token is the token to be processed. Use 'const Token &token()' to get it.
 * Member _lexer is the lexer for the statement. Use 'const Lexer &lexer()' to get it.
 *
 * The parser parses a statement word by word.
 * The list of attribute name and the list of primary key in the statement are treated as a whole by using the STL vector.
 * The where clause is treated as a whole by using the class Expr.
 * Use 'void consume()' to move to the next token. Use 'void Advance()' to get the next token in the statement.
 */
class Parser
{
  public:
    Parser(const std::string &cmd) : _lexer(cmd) { _token = _lexer.next(); }
    virtual ~Parser() = default;

    const Token &token() const { return _token; }
    const Lexer &lexer() const { return _lexer; }

    Statement parseStatement(); // Return the parsed statement.

    // Parse different types of statements.
    Statement parseCreate();
    Statement parseDelete();
    Statement parseDrop();
    Statement parseShow();
    Statement parseUse();
    Statement parseInsert();
    Statement parseUpdate();
    Statement parseSelect();

    bool isEnd() const { return _token.type() == Token::END; }

    void consume(Token::Type type); // Consume a keyword/operator.

    void advance(); // Advance in the stream.

    // Return a vector of attribute name mentioned in a insert or SELECT statement.
    std::vector<std::string> parseColumnList();

    // Return a vector of value mentioned in an INSERT or a SELECT statement.
    std::vector<Variant> parseValueList();

    // Parse the list of attribute name and the primary key mentioned in a CREATE statement.
    void parseFieldList(std::vector<Field> &fields, std::string &keys);
    void parseField(std::vector<Field> &fields, std::string &keys);

    Expr parseWhereClause();

    // Parse the set list mentioned in an UPDATE statement.
    void parseSetList(std::vector<std::string> &keys, std::vector<Variant> &values);

    // Return a vector of the attribute name mentioned in an INSERT or a SELECT statement.
    std::vector<std::string> parseSelectList();

  public:
    static std::map<Token::Type, int> opPrioMap;

  protected:
    Token _token;
    Lexer _lexer;
};
