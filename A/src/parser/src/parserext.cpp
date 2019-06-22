#include "parserext.h"
#include "../../backend/src/field.h"
#include "../../backend/include/stack.h"
#include <algorithm>
#include <fstream>
#include "fileio.h"

Statement ParserExt::parseStatement()
{
    // TODO: modify the parseWhereClause for other function's using
    switch (_token.type())
    {
        case Token::LOAD:
            return parseLoad();
        case Token::SELECT:
            return parseSelect();
        case Token::CREATE:
            return parseCreate();
        default:
            return Parser::parseStatement();
    }
}

std::vector<Column> ParserExt::parseColumnList()
{
    std::vector<Column> columns;

    auto consumeKeys = [&](Token &_token)
    {
        switch (_token.type())
        {
            case Token::COUNT: case Token::MAX: case Token::MIN: case Token::AVG: case Token::SUM:
            {
                std::string gatherData = "";
                Token::Type gatherType = _token.type();
                consume(_token.type());
                consume(Token::L_PAREN);
                if (_token.type() == Token::MUL)
                {
                    gatherData = "*";
                    consume(Token::MUL);
                }
                else
                {
                    gatherData = _token.toId();
                    consume(Token::ID);
                }
                consume(Token::R_PAREN);
                columns.push_back(Column{"", gatherData, gatherType});
                break;
            }
            case Token::ID:
            {
                columns.push_back(Column{"", _token.toId(), Token::ID});
                consume(Token::ID);
                break;
            }
        }
    };

    consumeKeys(_token);

    while (_token.type() == Token::COMMA)
    {
        consume(Token::COMMA);
        consumeKeys(_token);
    }

    return columns;
}

std::vector<Column> ParserExt::parseSelectList()
{
    std::vector<Column> columns;
    switch (_token.type())
    {
        case Token::MUL:
            //TODO: make sure if it's MUL or other
            columns.emplace_back(Column{"", "*", Token::MUL});
            consume(Token::MUL);
            break;

        case Token::ID: case Token::COUNT: case Token::MAX: case Token::MIN: case Token::AVG: case Token::SUM:
            columns = parseColumnList();
            break;

        default:
            throw ParserError("Syntax error");
    }
    return columns;
}

Statement ParserExt::parseSelect() {
    consume(Token::SELECT);
    std::vector<Column> columns = parseSelectList();
    std::string fileName = "";
    std::string tableId = "";
    std::vector<Column> orderByColumn, groupByColumn;
    Expr where;

    if (_token.type() == Token::INTO)
    {
        consume(Token::INTO);
        consume(Token::OUTFILE);
        fileName = _token.toOperand().toStdString();
        if (_token.type() == Token::OPERAND)
            consume(Token::OPERAND);
        else
            consume(Token::ID);
    }

    consume(Token::FROM);
    tableId = _token.toId();
    consume(Token::ID);

    if (_token.type() == Token::WHERE)
    {
       consume(Token::WHERE);
       where = parseWhereClause();
    }

    if (_token.type() == Token::GROUP)
    {
        consume(Token::GROUP);
        consume(Token::BY);
        groupByColumn = parseColumnList();
    }

    if (_token.type() == Token::ORDER)
    {
        consume(Token::ORDER);
        consume(Token::BY);
        orderByColumn = parseColumnList();
    }

    consume(Token::SEMICOLON);

    return Statement(new StatementSelectInto(tableId, fileName, groupByColumn, orderByColumn, columns, where));
}

Statement ParserExt::parseLoad()
{
    consume(Token::LOAD);
    consume(Token::DATA);
    consume(Token::INFILE);
    std::vector<std::vector<Variant> > values;
    parseValueListFromFile(values);
    consume(Token::OPERAND);
    consume(Token::INTO);
    consume(Token::TABLE);
    std::string tableName = _token.toId();
    consume(Token::ID);
    consume(Token::L_PAREN);
    std::vector<Column> columns = parseColumnList();
    consume(Token::R_PAREN);
    consume(Token::SEMICOLON);

    if (values.front().size() != columns.size())
        throw ParserError("Syntax error");

    std::vector<std::map<std::string, Variant> > entries;
    for (size_t i = 0; i < values.size(); i++)
    {
        std::map<std::string, Variant> entry;
        for (size_t j = 0; j < columns.size(); j++)
            entry[columns[j].name] = values[i][j];
        entries.push_back(entry);
    }
    return Statement(new StatementLoad(tableName, entries));
}

void ParserExt::parseValueListFromFile(std::vector<std::vector<Variant> > &values)
{
    std::ifstream infile;
    std::string fileName = _token.toOperand().toStdString();
    if (!isFile(fileName)) // not absolute directory
        fileName = __ProgramPath + "\\" + fileName;
    infile.clear();
    infile.open(fileName);
    if (!infile.is_open())
        throw ParserError("File " + fileName + " not found.");   //这里的报错信息先随便写了一个
    while(!infile.eof())
    {
        // TODO: bugs here !!!
        std::string data;
        getline(infile, data);
        ParserExt parserInFile(data);
        std::vector<Variant> value = parserInFile.parseValueListInFile();
        if (!value.empty())
            values.emplace_back(value);
        /*std::vector<Variant> value;
        std::stringstream in(data);
        while (!in.eof())
        {
            std::string val;
            in >> val;
            value.push_back(val);
        }
        if (!value.empty())
            values.push_back(value);*/
    }
    infile.close();
}

std::vector<Variant> ParserExt::parseValueListInFile()
{
    std::vector<Variant> value;
    
    while (!isEnd())
    {
        // TODO: bugs here! could not recognize string properly
        if (_token.type() == Token::OPERAND)
        {
            value.emplace_back(_token.toOperand());
            consume(Token::OPERAND);
        }
        else
        {
            value.emplace_back(_token.toId());
            consume(Token::ID);
        }
    }

    return value;
}

Statement ParserExt::parseCreate()
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

void ParserExt::parseFieldList(std::vector<Field> &fields, std::string &priKey)
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

void ParserExt::parseField(std::vector<Field> &fields, std::string &priKey)
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

                case Token::TEXT:
                    fields.push_back(Field(name, Variant::STRING));
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