#pragma once

#include <string>
#include <vector>
#include <map>

#include "serverexcept.h"

class Server
{
public:
    void init(char *ipAddress, int port = 1234);
    void close();
    std::string connectClient();
    void disconnectClient(const std::string &client);
    void recvMsgFrom(const std::string &client, std::string &msg);
    void sendMsgTo(const std::string &client, const std::string &msg);

private:
    typedef unsigned int SOCKET;

    SOCKET serverSock;

    std::map<std::string, unsigned int> clients;

    static const int MAX_BUF = 65536;
};