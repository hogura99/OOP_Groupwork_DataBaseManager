#pragma once

#include <string>
#include <vector>
#include <map>

#include "serverexcept.h"

class Server
{
public:
    void init(const char *ipAddress = "127.0.0.1", int port = 1234);
    void close();
    std::string connectClient();
    void disconnectClient(const std::string &client);
    void recvMsgFrom(const std::string &client, std::string &msg);
    void sendMsgTo(const std::string &client, const std::string &msg);

    void recvMsg(std::string &msg);
    void sendMsg(const std::string &msg);

private:

#ifdef WIN32
    typedef unsigned int SOCKET;
#else
    typedef int SOCKET;
#endif

    SOCKET serverSock;

    std::map<std::string, SOCKET> clients;

    SOCKET currentClient;

    static const int MAX_BUF = 65536;
};