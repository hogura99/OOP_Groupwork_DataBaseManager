#include <iostream>
#include <string>
#include <ctime>

#include "databaseExt.h"
#include "entry.h"
#include "datastream.h"
#include "parserext.h"
#include "token.h"
#include "server.h"

//#define SERVER_ON

/**
 * Database shell program.
 * This program accepts SQL commands from keyboard, calls parser to parse the commands into SQL statements,
 * then calls backend to execute the statements, and print out the query results.
 */
int main()
{
    DatabaseExt db;
    std::string cmd;
    static char recvBuf[Server::MAXBUF];
    int InitServerStatus = Server::initServer("127.0.0.1");
    if (InitServerStatus != 0)
    {
        std::cerr << "init server failed" << std::endl;
        return InitServerStatus;
    }
    while (true)
    {
#ifdef SERVER_ON
        memset(recvBuf, 0, sizeof recvBuf);
        Server::SOCKET client = Server::connectClient();
        int ConnectStatus = Server::recvMsgFromClient(client, recvBuf, Server::MAXBUF);
        cmd = recvBuf;
#else
        if (!std::getline(std::cin, cmd, ';'))
            break;
#endif
        cmd += ';';

        Parser parser(cmd);
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
                db.dropDatabase(statement->id());
                break;
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
                if (s->getColumns().front() == "*")
                    db.selectAllFrom(s->id(), s->getWhere(), s->getFilename() ,s->getGroupByColumn()).result()->print();
                else
                    db.selectFrom(s->id(), s->getColumns(), s->getWhere(), s->getFilename(), s->getGroupByColumn()).result()->print();
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
        Server::disconnectClient(client);
#endif
    }

    Server::closeServer();
    return 0;
}
