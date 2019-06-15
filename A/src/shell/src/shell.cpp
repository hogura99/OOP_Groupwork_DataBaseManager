#include <iostream>
#include <string>
#include <ctime>

#include "databaseExt.h"
#include "entry.h"
#include "datastream.h"
#include "parserext.h"
#include "token.h"
#include "server.h"

#define SERVER_ON

/**
 * Database shell program.
 * This program accepts SQL commands from keyboard, calls parser to parse the commands into SQL statements,
 * then calls backend to execute the statements, and print out the query results.
 */
int main()
{
    DatabaseExt db;
    std::string cmd;
#ifdef SERVER_ON
    Server server;
    server.init("127.0.0.1", 1234);
#endif
    while (true)
    {
#ifdef SERVER_ON
        //std::string client = server.connectClient();
        //server.recvMsgFrom(client, cmd);
        server.recvMsg(cmd);

        auto streamBufCout = std::cout.rdbuf();
        std::stringstream strstream;
        std::cout.rdbuf(strstream.rdbuf());
#else
        if (!std::getline(std::cin, cmd, ';'))
            break;
#endif
        cmd += ';';

        ParserExt parser(cmd);
        try
        {
            auto parseResult = parser.parseStatement();
            if (!parseResult.content())
                break;
            auto statement = parseResult.content();
            switch (statement->type())
            {
            case StatementBase::SHOW_COLUMNS:
                db.showColumnsFrom(statement->id()).result()->print();
                break;
            case StatementBase::SHOW_DATABASES:
                db.showDatabases().result()->print();
                break;
            case StatementBase::SHOW_TABLES:
                db.showTables().result()->print();
                break;
            case StatementBase::DROP_DATABASE:
            {
                auto res = db.dropDatabase(statement->id());
                //std::cout << res << std::endl;
                break;
            }
            case StatementBase::DROP_TABLE:
                db.dropTable(statement->id());
                break;
            case StatementBase::CREATE_DATABASE:
                db.createDatabase(statement->id());
                break;
            case StatementBase::CREATE_TABLE:
            {
                auto s = dynamic_cast<StatementCreateTable *>(statement);
                db.createTable(s->id(), s->fields(), s->primaryKey());
                break;
            }
            case StatementBase::USE_DATABASE:
                db.useDatabase(statement->id());
                break;
            case StatementBase::DELETE:
            {
                auto s = dynamic_cast<StatementDelete *>(statement);
                db.deleteFrom(s->id(), s->getWhere());
                break;
            }
            case StatementBase::UPDATE:
            {
                auto s = dynamic_cast<StatementUpdate *>(statement);
                db.update(s->id(), s->keys(), s->values(), s->whereClause());
                break;
            }
            case StatementBase::INSERT:
            {
                auto s = dynamic_cast<StatementInsert *>(statement);
                s->entry();
                db.insertInto(s->id(), s->entry());
                break;
            }
            case StatementBase::SELECT:
            {
                auto s = dynamic_cast<StatementSelectInto *>(statement);
                if (s->isSelectAll())
                    db.selectAllFrom(s->id(), s->getColumns(), s->getWhere(), s->getFilename(), s->getGroupByColumn(), s->getOrderByColumn()).result()->print();
                else
                    db.selectFrom(s->id(), s->getColumns(), s->getWhere(), s->getFilename(), s->getGroupByColumn(), s->getOrderByColumn()).result()->print();
                break;
            }
            case StatementBase::LOAD:
            {
                auto s = dynamic_cast<StatementLoad *>(statement);
                db.load(s->id(), s->entries());
            }
            default:
                break;
            }
        }
        catch (const TokenError &e)
        {
            std::cout << "line " << parser.lexer().lineNo() << ", ";
            std::cout << "column " << parser.lexer().columnNo() << ": ";
            std::cout << e.what() << std::endl;
        }
        catch (const LexerError &e)
        {
            std::cout << "line " << parser.lexer().lineNo() << ", ";
            std::cout << "column " << parser.lexer().columnNo() << ": ";
            std::cout << e.what() << std::endl;
        }
        catch (const ParserError &e)
        {
            std::cout << "line " << parser.lexer().lineNo() << ", ";
            std::cout << "column " << parser.lexer().columnNo() << ": ";
            std::cout << e.what() << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }

#ifdef SERVER_ON
        std::string result = strstream.str();
        std::cerr << "Command:[ " << cmd << " ]\n"
                  << "Result :[ " << result << " ]" << std::endl;
        server.sendMsg(result);
        std::cout.rdbuf(streamBufCout);
#endif
    }

#ifdef SERVER_ON
    server.close();
#endif
    return 0;
}
