#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>

#include "client.h"

int main()
{
    /*
    Client::initClient("127.0.0.1");
    std::string cmd;
    static char sendBuf[Client::MAXBUF];
    static char recvBuf[Client::MAXBUF];
    while (std::getline(std::cin, cmd, ';'))
    {
        memset(sendBuf, 0, sizeof sendBuf);
        strcpy(sendBuf, cmd.c_str());
        int createConvStatus = Client::createConversation();
        if (createConvStatus != 0)
        {
            std::cerr << "creating conversation fail!" << std::endl;
            Client::stopConversation();
            continue;
        }
        Client::sendMsgToServer(sendBuf);
        Client::recvMsgFromServer(recvBuf);
        std::cout << recvBuf << std::endl;
        Client::stopConversation();
    }
    */
    return 0;
}