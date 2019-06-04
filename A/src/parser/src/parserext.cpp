#include "parserext.h"
#include "../../backend/src/field.h"
#include "../../backend/include/stack.h"
#include <algorithm>

Statement ParserExt::parseSelect()
{
    consume(Token::SELECT);
    std::vector<std::string> columns = parseSelectList();
    std::string file_name = "";
    std::string table_id = "";
    std::vector<std::string> group_by_column;
    Expr where;

    while (_token.type() != Token::WHERE || _token.type() != Token::SEMICOLON){
        switch (_token.type())
        {
            case Token::COUNT:{
                std::string count_data = "";
                count_data = _token.toId();
                for(char& ch : count_data)
                    if (ch <= 90 && ch >= 65)
                        ch += 32;
                    else if (ch == '(')
                        ch = ' ';
                    else if (ch == ')')
                        ch = 0;
                count_data = "\\\\" + count_data;
                consume(Token::COUNT);
                columns.push_back(count_data);
                break;
            }
            case Token::INTO:
                consume(Token::INTO);
                consume(Token::OUTFILE);
                file_name = _token.toId();
                consume(Token::OPERAND);
                break;
            case Token::FROM:
                consume(Token::FROM);
                table_id = _token.toId();
                consume(Token::ID);
                break;
            case Token::GROUP: {
                consume(Token::GROUP);
                consume(Token::BY);
                group_by_column.insert(group_by_column.end(), parseColumnList().begin(), parseColumnList().end());
                break;
            }
            case Token::ORDER:{
                consume(Token::ORDER);
                consume(Token::BY);
                break;
            }
        }
    }

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

    return Statement(new StatementSelectInto(table_id, file_name, group_by_column, columns, where));
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

}