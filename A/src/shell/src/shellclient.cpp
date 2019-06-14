#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>

#include "client.h"

int main()
{
    Client client;
    client.setServer("127.0.0.1", 1234);
    std::string cmd;
    std::string recvMsg;
    while (std::getline(std::cin, cmd, ';'))
    {
        client.createConversation();
        client.sendMsgToServer(cmd);
        client.recvMsgFromServer(recvMsg);
        std::cout << recvMsg << std::endl;
        client.stopConversation();
    }
    return 0;
}