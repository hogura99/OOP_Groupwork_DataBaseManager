#include "parserext.h"
#include "../../backend/src/field.h"
#include "../../backend/include/stack.h"
#include <algorithm>
#include <fstream>
#include "fileio.h"

Statement ParserExt::parseStatement()
{
    switch (_token.type())
    {
        case Token::LOAD:
            return parseLoad();
        case Token::SELECT:
            return parseSelect();
        default:
            return Parser::parseStatement();
    }
}

std::vector<std::string> ParserExt::parseColumnList()
{
    std::vector<std::string> columns;

    auto consumeKeys = [&](Token _token)
    {
        switch (_token.type())
        {
            case Token::COUNT:
            {
                consume(Token::COUNT);
                consume(Token::L_PAREN);
                std::string count_data = "";
                if (_token.type() == Token::MUL)
                {
                    count_data = "*";
                    consume(Token::MUL);
                }
                else
                {
                    count_data = _token.toId();
                    consume(Token::ID);
                }
                for (char& ch : count_data)
                    if (ch <= 'Z' && ch >= 'A')
                        ch += 'z' - 'A';
                    else if (ch == '(')
                        ch = ' ';
                    else if (ch == ')')
                        ch = 0;
                count_data = "\\COUNT " + count_data;
                consume(Token::R_PAREN);
                columns.push_back(count_data);
                break;
            }
            case Token::ID:
            {
                columns.push_back(_token.toId());
                consume(Token::ID);
                break;
            }
        }
    };

    {
        switch (_token.type())
        {
            case Token::COUNT:
            {
                consume(Token::COUNT);
                consume(Token::L_PAREN);
                std::string count_data = "";
                if (_token.type() == Token::MUL)
                {
                    count_data = "*";
                    consume(Token::MUL);
                }
                else
                {
                    count_data = _token.toId();
                    consume(Token::ID);
                }
                for (char& ch : count_data)
                    if (ch <= 'Z' && ch >= 'A')
                        ch += 'z' - 'A';
                    else if (ch == '(')
                        ch = ' ';
                    else if (ch == ')')
                        ch = 0;
                count_data = "\\COUNT " + count_data;
                consume(Token::R_PAREN);
                columns.push_back(count_data);
                break;
            }
            case Token::ID:
            {
                columns.push_back(_token.toId());
                consume(Token::ID);
                break;
            }
        }
    }

    while (_token.type() == Token::COMMA)
    {
        consume(Token::COMMA);
        {
            switch (_token.type())
            {
                case Token::COUNT:
                {
                    consume(Token::COUNT);
                    consume(Token::L_PAREN);
                    std::string count_data = "";
                    if (_token.type() == Token::MUL)
                    {
                        count_data = "*";
                        consume(Token::MUL);
                    }
                    else
                    {
                        count_data = _token.toId();
                        consume(Token::ID);
                    }
                    for (char& ch : count_data)
                        if (ch <= 'Z' && ch >= 'A')
                            ch += 'z' - 'A';
                    count_data = "\\COUNT " + count_data;
                    consume(Token::R_PAREN);
                    columns.push_back(count_data);
                    break;
                }
                case Token::ID:
                {
                    columns.push_back(_token.toId());
                    consume(Token::ID);
                    break;
                }
            }
        }
    }

    return columns;
}

std::vector<std::string> ParserExt::parseSelectList()
{
    std::vector<std::string> columns;
    switch (_token.type())
    {
        case Token::MUL:
            columns.emplace_back("*");
            consume(Token::MUL);
            break;

        case Token::ID: case Token::COUNT:
            columns = parseColumnList();
            break;

        default:
            throw ParserError("Syntax error");
    }
    return columns;
}

Statement ParserExt::parseSelect() {
    consume(Token::SELECT);
    std::vector<std::string> columns = parseSelectList();
    std::string file_name = "";
    std::string table_id = "";
    std::vector<std::string> groupByColumn, orderByColumn;
    Expr where;

    if (_token.type() == Token::INTO)
    {
        consume(Token::INTO);
        consume(Token::OUTFILE);
        file_name = _token.toId();
        consume(Token::OPERAND);
    }

    consume(Token::FROM);
    table_id = _token.toId();
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

    return Statement(new StatementSelectInto(table_id, file_name, groupByColumn, columns, where));
}

Statement ParserExt::parseLoad()
{
    consume(Token::LOAD);
    consume(Token::DATA);
    consume(Token::INFILE);
    std::vector<std::vector<Variant> > values;
    parseValueListFromFile(values);
    consume(Token::ID);
    consume(Token::INTO);
    consume(Token::TABLE);
    std::string tableName = _token.toId();
    consume(Token::ID);
    consume(Token::L_PAREN);
    std::vector<std::string> columns = parseColumnList();
    consume(Token::R_PAREN);
    consume(Token::SEMICOLON);

    if (values.front().size() != columns.size())
        throw ParserError("Syntax error");

    std::vector<std::map<std::string, Variant> > entries;
    for (size_t i = 0; i < values.size(); i++)
    {
        std::map<std::string, Variant> entry;
        for (size_t j = 0; j < columns.size(); j++)
            entry[columns[j]] = values[i][j];
    }
    return Statement(new StatementLoad(tableName, entries));
}

void ParserExt::parseValueListFromFile(std::vector<std::vector<Variant> > values)
{
    std::ifstream infile;
    std::string fileName = _token.toId();
    if (!isFile(fileName)) // not absolute directory
        fileName = getCwd() + "/../" + fileName;
    infile.clear();
    infile.open(fileName);
    if (!infile.is_open())
        throw ParserError("File " + _token.toId() + " not found.");   //这里的报错信息先随便写了一个
    while(!infile.eof())
    {
        std::string data;
        getline(infile, data);
        ParserExt parserInFile(data);
        std::vector<Variant> value = parserInFile.parseValueListInFile();
        values.emplace_back(value);
    }
}

std::vector<Variant> ParserExt::parseValueListInFile()
{
    std::vector<Variant> value;
    
    while (!isEnd())
    {
        value.emplace_back(_token.toOperand());
        consume(Token::OPERAND);
    }
}