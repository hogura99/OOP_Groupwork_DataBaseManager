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
        ParserExt parser(cmd);
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