#include "parser.h"
#include "../../backend/src/field.h"
#include "../../backend/include/stack.h"
#include <algorithm>

std::map<Token::Type, int> Parser::opPrioMap{
    {Token::MUL, 10},
    {Token::DIV, 10},

    {Token::PLUS, 9},
    {Token::MINUS, 9},

    {Token::EQ, 5},
    {Token::NEQ, 5},
    {Token::LEQ, 5},
    {Token::GEQ, 5},
    {Token::GT, 5},
    {Token::LT, 5},

    {Token::NOT, 3},

    {Token::AND, 2},

    {Token::OR, 1},

    {Token::SEMICOLON, 0},
    {Token::R_PAREN, 0}};

Statement Parser::parseCreate()
{
    consume(Token::CREATE);

    switch (_token.type())
    {
    case Token::TABLE:
    {
        consume(Token::TABLE);
        std::string table_id = _token.toId();
        consume(Token::ID);
        consume(Token::L_PAREN);
        std::vector<Field> fields;
        std::string priKey;
        parseFieldList(fields, priKey);
        consume(Token::R_PAREN);
        consume(Token::SEMICOLON);
        return Statement(new StatementCreateTable(table_id, fields, priKey));
    }

    case Token::DATABASE:
    {
        consume(Token::DATABASE);
        std::string databaseId = _token.toId();
        consume(Token::ID);
        consume(Token::SEMICOLON);
        return Statement(new StatementCreateDatabase(databaseId));
    }

    default:
        throw ParserError("Syntax error");
    }
}

Statement Parser::parseStatement()
{
    switch (_token.type())
    {
    case Token::CREATE:
        return parseCreate();

    case Token::INSERT:
        return parseInsert();

    case Token::DELETE:
        return parseDelete();

    case Token::UPDATE:
        return parseUpdate();

    case Token::USE:
        return parseUse();

    case Token::SELECT:
        return parseSelect();

    case Token::DROP:
        return parseDrop();

    case Token::SHOW:
        return parseShow();

    case Token::END:
    case Token::SEMICOLON:
        return nullptr;

    default:
        throw ParserError("Syntax error");
    }
}
void Parser::parseFieldList(std::vector<Field> &fields, std::string &priKey)
{
    switch (_token.type())
    {
    case Token::ID:
    case Token::PRIMARY:
        parseField(fields, priKey);

        while (_token.type() != Token::R_PAREN)
        {
            consume(Token::COMMA);
            parseField(fields, priKey);
        }

        break;

    default:
        throw ParserError("Syntax error");
    }
}

void Parser::parseField(std::vector<Field> &fields, std::string &priKey)
{
    switch (_token.type())
    {
    case Token::ID:
    {
        std::string name = _token.toId();
        consume(Token::ID);

        switch (_token.type())
        {
        case Token::INT:
            fields.push_back(Field(name, Variant::INT));
            break;

        case Token::CHAR:
            fields.push_back(Field(name, Variant::CHAR));
            break;

        case Token::DOUBLE:
            fields.push_back(Field(name, Variant::DOUBLE));
            break;
        }

        consume(_token.type());

        if (_token.type() == Token::NOT)
        {
            if (fields.empty())
                throw ParserError("Not Null error");

            consume(Token::NOT);
            consume(Token::NULL_SQL);
            fields.back().setNull(false);
        }

        break;
    }

    case Token::PRIMARY:
    {
        consume(Token::PRIMARY);
        consume(Token::KEY);
        consume(Token::L_PAREN);
        priKey = _token.toId();
        consume(Token::ID);
        auto it = std::find_if(fields.begin(), fields.end(), [=](const Field &f) {
            return f.key() == priKey;
        });

        if (it == fields.end())
            throw ParserError("Primary key not found");

        it->setPrimary(true);
        consume(Token::R_PAREN);
        break;
    }

    default:
        throw ParserError("Syntax error");
    }
}

std::vector<std::string> Parser::parseColumnList()
{
    std::vector<std::string> colNames;
    colNames.push_back(_token.toId());
    consume(Token::ID);

    while (_token.type() == Token::COMMA)
    {
        consume(Token::COMMA);
        colNames.push_back(_token.toId());
        consume(Token::ID);
    }

    return colNames;
}

Statement Parser::parseInsert()
{
    if (_token.type() != Token::INSERT)
        throw ParserError("Syntax error");

    consume(Token::INSERT);
    consume(Token::INTO);
    std::string table_id = _token.toId();
    consume(Token::ID);
    consume(Token::L_PAREN);
    std::vector<std::string> columns = parseColumnList();
    consume(Token::R_PAREN);
    consume(Token::VALUES);
    consume(Token::L_PAREN);
    std::vector<Variant> values = parseValueList();

    if (values.size() != columns.size())
        throw ParserError("Syntax error");

    std::map<std::string, Variant> entry;

    for (size_t i = 0; i < values.size(); i++)
        entry[columns[i]] = values[i];

    consume(Token::R_PAREN);
    consume(Token::SEMICOLON);
    return Statement(new StatementInsert(table_id, entry));
}

std::vector<Variant> Parser::parseValueList()
{
    std::vector<Variant> values;
    values.emplace_back(_token.toOperand());
    consume(Token::OPERAND);

    while (_token.type() != Token::R_PAREN)
    {
        consume(Token::COMMA);
        values.emplace_back(_token.toOperand());
        consume(Token::OPERAND);
    }

    return values;
}

Statement Parser::parseDelete()
{
    consume(Token::DELETE);
    consume(Token::FROM);
    std::string table_id = _token.toId();
    consume(Token::ID);
    Expr where;

    switch (_token.type())
    {
    case Token::WHERE:
        consume(Token::WHERE);
        where = parseWhereClause();
        break;

    case Token::SEMICOLON:
        break;

    default:
        throw ParserError("Syntax error");
    }

    consume(Token::SEMICOLON);
    return Statement(new StatementDelete(table_id, where));
}

void Parser::parseSetList(std::vector<std::string> &keys, std::vector<Variant> &values)
{
    keys.push_back(_token.toId());
    consume(Token::ID);
    consume(Token::EQ);
    values.push_back(_token.toOperand());
    consume(Token::OPERAND);
    while (_token.type() == Token::COMMA)
    {
        consume(Token::COMMA);
        keys.push_back(_token.toId());
        consume(Token::ID);
        consume(Token::EQ);
        values.push_back(_token.toOperand());
        consume(Token::OPERAND);
    }
}

Expr Parser::parseWhereClause()
{
    Stack<Expr> vals;       // a stack of operands
    Stack<Token::Type> ops; // a stack of operators

    for (; !isEnd(); advance())
    {
        if (_token.type() == Token::L_PAREN)
        {
            consume(Token::L_PAREN);
            vals.push(parseWhereClause());
            continue;
        }
        else if (opPrioMap.count(_token.type())) // operator
        {
            while (!ops.empty())
            {
                if (opPrioMap[ops.back()] >= opPrioMap[_token.type()])
                {
                    if (ops.top() == Token::NOT) // unary operator
                    {
                        Expr expr(ops.pop());
                        expr.setRight(vals.pop());
                        vals.push(expr);
                    }
                    else // binary operator
                    {
                        Expr b(vals.pop());
                        Expr a(vals.pop());
                        Expr expr(ops.pop(), a, b);
                        vals.push(expr);
                    }
                }
                else
                    break;
            }

            ops.push(_token.type());
        }
        else // operand
        {
            switch (_token.type())
            {
            case Token::ID:
                vals.push(Expr(_token));
                break;

            case Token::OPERAND:
                vals.push(Expr(_token));
                break;

            default:
                throw ParserError("Syntax error");
            }
        }

        if (_token.type() == Token::SEMICOLON || _token.type() == Token::R_PAREN)
            break;
    }

    if (vals.size() != 1)
        throw ParserError("Syntax error");

    return vals.back();
}

Statement Parser::parseSelect()
{
    consume(Token::SELECT);
    std::vector<std::string> columns = parseSelectList();
    consume(Token::FROM);
    std::string table_id = _token.toId();
    consume(Token::ID);
    Expr where;

    switch (_token.type())
    {
    case Token::WHERE:
        consume(Token::WHERE);
        where = parseWhereClause();
        break;

    case Token::SEMICOLON:
        break;

    default:
        break;
    }

    consume(Token::SEMICOLON);
    return Statement(new StatementSelect(table_id, columns, where));
}

Statement Parser::parseDrop()
{
    consume(Token::DROP);

    switch (_token.type())
    {
    case Token::TABLE:
    {
        consume(Token::TABLE);
        std::string tableName = _token.toId();
        consume(Token::ID);
        consume(Token::SEMICOLON);
        return Statement(new StatementDropTable(tableName));
    }

    case Token::DATABASE:
    {
        consume(Token::DATABASE);
        std::string dbName = _token.toId();
        consume(Token::ID);
        consume(Token::SEMICOLON);
        return Statement(new StatementDropDatabase(dbName));
    }

    default:
        throw ParserError("Syntax error");
    }
}

Statement Parser::parseUse()
{
    consume(Token::USE);
    std::string dbName = _token.toId();
    consume(Token::ID);
    consume(Token::SEMICOLON);
    return Statement(new StatementUseDatabase(dbName));
}

Statement Parser::parseShow()
{
    consume(Token::SHOW);

    switch (_token.type())
    {
    case Token::TABLES:
    {
        consume(Token::TABLES);
        consume(Token::SEMICOLON);
        return Statement(new StatementShowTables());
    }

    case Token::DATABASES:
    {
        consume(Token::DATABASES);
        consume(Token::SEMICOLON);
        return Statement(new StatementShowDatabases());
    }

    case Token::COLUMNS:
    {
        consume(Token::COLUMNS);
        consume(Token::FROM);
        std::string tableName = _token.toId();
        consume(Token::ID);
        consume(Token::SEMICOLON);
        return Statement(new StatementShowColumns(tableName));
    }

    default:
        throw ParserError("Syntax error");
    }
}

Statement Parser::parseUpdate()
{
    consume(Token::UPDATE);
    std::string tableName = _token.toId();
    consume(Token::ID);
    consume(Token::SET);

    std::vector<std::string> keys;
    std::vector<Variant> values;
    parseSetList(keys, values);

    Expr expr;

    switch (_token.type())
    {
    case Token::WHERE:
        consume(Token::WHERE);
        expr = parseWhereClause();
        break;

    case Token::SEMICOLON:
        break;

    default:
        throw ParserError("Syntax error");
    }

    return Statement(new StatementUpdate(tableName, keys, values, expr));
}

std::vector<std::string> Parser::parseSelectList()
{
    std::vector<std::string> columns;
    switch (_token.type())
    {
    case Token::MUL:
        columns.emplace_back("*");
        consume(Token::MUL);
        break;

    case Token::ID:
        columns = parseColumnList();
        break;

    default:
        throw ParserError("Syntax error");
    }
    return columns;
}

void Parser::consume(Token::Type t)
{
    if (_token.type() != t)
        throw ParserError("Syntax error");

    advance();
}

void Parser::advance()
{
    _token = _lexer.next(); // get the next token to be scanned.
}