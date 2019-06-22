#pragma once

#include <cstdlib>
#include <cstdio>
#include <string>

#include "clientexcept.h"

class Client
{
public:
    typedef unsigned int SOCKET;

    static const int MAX_BUF = 65536;

    void setServer(const char *serverIpAddress = "127.0.0.1", int port = 1234);
    void buildClient();

    void sendMsgToServer(const std::string &msg);
    void recvMsgFromServer(std::string &msg);

    void createConversation();
    void stopConversation();

private:
    void _sendMsgToServer(const std::string &msg);
    void _recvMsgFromServer(std::string &msg);

    std::string _serverIpAddr;
    int _serverPort;

    SOCKET _socket;
};