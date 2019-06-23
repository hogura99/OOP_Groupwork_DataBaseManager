#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <cstdio>

#include "databaseext.h"
#include "entry.h"
#include "datastream.h"
#include "parserext.h"
#include "token.h"
#include "client.h"
#include "clientexcept.h"

int main(int argc, char **argv)
{
    std::string SERVER_IP = "127.0.0.1";
    int SERVER_PORT = 1234;
    if (argc == 3)
    {
        SERVER_IP = argv[1];
        SERVER_PORT = std::atoi(argv[2]);
    }
    Client client;
    client.setServer(SERVER_IP.c_str(), SERVER_PORT);
    std::string cmd;
    std::string sendMsg;
    std::string recvMsg;
    std::string selectIntoFile;
    std::stringstream sendMsgStream;
    while (std::getline(std::cin, cmd, ';'))
    {
        cmd += ';';
        sendMsg = "";
        selectIntoFile = "";
        sendMsgStream.clear();
        ParserExt parser(cmd);
        try
        {
            auto parserResult = parser.parseStatement();
            auto statement = parserResult.content();
            switch (statement->type())
            {
                case StatementBase::SELECT:
                {
                    auto statementSelect = (dynamic_cast<StatementSelectInto *> (statement));
                    if (statementSelect->getFilename() != nullptr)
                    {
                        selectIntoFile = *statementSelect->getFilename();
                        statementSelect->setFileName("");
                    }
                    sendMsgStream << *statementSelect;
                    sendMsg = sendMsgStream.str();
                    break;
                }
                case StatementBase::LOAD:
                {
                    auto statementLoad = dynamic_cast<StatementLoad *> (statement);
                    sendMsgStream << *statementLoad;
                    sendMsg = sendMsgStream.str();
                    break;
                }
                default:
                {
                    sendMsg = cmd;
                    sendMsg.pop_back(); // erase the last ';' token
                    break;
                }
            }
            client.sendMsgToServer(sendMsg);
            client.recvMsgFromServer(recvMsg);
            if (selectIntoFile != "")
            {
                std::ofstream out(selectIntoFile);
                if (!out.is_open())
                    throw ClientError("could not open file " + selectIntoFile);
                out << recvMsg << std::endl;
                out.close();
            }
            else
            {
                std::cout << recvMsg << std::endl;
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
        catch (const ClientError &err)
        {
            std::cout << err.what() << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}