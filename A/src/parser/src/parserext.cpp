#include "parserext.h"
#include "../../backend/src/field.h"
#include "../../backend/include/stack.h"
#include <algorithm>

Statement ParserExt::parseSelect()
{
    consume(Token::SELECT);
    std::vector<std::string> columns = parseSelectList();
    std::string file_name;
    if(_token.type() == Token::INTO){
        consume(Token::INTO);
        consume(Token::OUTFILE);
        file_name = _token.toId();
    }
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
    return Statement(new StatementSelectInto(table_id, file_name, columns, where));
}