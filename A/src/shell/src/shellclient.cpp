#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>

#include "client.h"
#include "clientexcept.h"

#include "parserext.h"

int main()
{
    Client client;
    client.setServer("127.0.0.1", 1234);
    std::string cmd;
    std::string recvMsg;
    while (std::getline(std::cin, cmd, ';'))
    {
        try
        {
            cmd += ';';
            ParserExt parser(cmd);
            auto parserResult = parser.parseStatement();
            auto statement = parserResult.content();
            switch (statement->type())
            {
                case StatementBase::SELECT:
                    std::cout << *(dynamic_cast<StatementSelectInto *> (statement)) << std::endl;
                    break;
            }
        }
        catch (const ParserError &e)
        {
            std::cout << e.what() << std::endl;
        }
        continue;
        try
        {
            client.sendMsgToServer(cmd);
            client.recvMsgFromServer(recvMsg);
            std::cout << recvMsg << std::endl;
        }
        catch (const ClientError &err)
        {
            std::cout << err.what() << std::endl;
        }
    }
    return 0;
}