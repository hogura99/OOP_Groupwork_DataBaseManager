#include <iostream>
#include <string>
#include <ctime>

#include "database.h"
#include "entry.h"
#include "datastream.h"
#include "parserext.h"
#include "token.h"
#include "server.h"

/**
 * Database shell program.
 * This program accepts SQL commands from keyboard, calls parser to parse the commands into SQL statements,
 * then calls backend to execute the statements, and print out the query results.
 */
int main()
{
    Database db;
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
        memset(recvBuf, 0, sizeof recvBuf);
        Server::SOCKET client = Server::connectClient();
        int ConnectStatus = Server::recvMsgFromClient(client, recvBuf, Server::MAXBUF);

        cmd = recvBuf;
        cmd += ';';

        Parser parser(cmd);
        try
        {
            auto parseResult = parser.parseStatement();
            if (!parseResult.content())
                return 0;
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
                auto s = dynamic_cast<StatementSelect *>(statement);
                if (s->getColumns().front() == "*")
                    db.selectAllFrom(s->id(), s->getWhere()).result()->print();
                else
                    db.selectFrom(s->id(), s->getColumns(), s->getWhere()).result()->print();
                break;
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

        Server::disconnectClient(client);
    }

    Server::closeServer();
    return 0;
}
