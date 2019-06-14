#include "server.h"

#ifdef WIN32

#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")

void Server::init(char *ipAddress, int port)
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(ipAddress);
    sockAddr.sin_port = htons(port);
    int bindRes = bind(serverSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    listen(serverSock, 20);
    if (bindRes != 0)
        throw ServerError(std::to_string(bindRes));
}

void Server::close()
{
    closesocket(serverSock);
    WSACleanup();
}

std::string Server::connectClient()
{
    auto client = accept(serverSock, NULL, NULL);

    std::string clientName = std::to_string((unsigned int)client);
    if (!clients.count(clientName))
        clients[clientName] = client;

    return clientName;
}

void Server::disconnectClient(const std::string &client)
{
    if (!clients.count(client))
        throw ServerError("client " + client + " not found.");
    closesocket(clients[client]);
}

void Server::recvMsgFrom(const std::string &client, std::string &msg)
{
    auto clientSock = clients[client];
    static char recvMsg[MAX_BUF];

    int res = recv(clientSock, recvMsg, sizeof recvMsg, NULL);
    //if (res != 0)
    //    throw ServerError("receiving message error code " + std::to_string(res));

    msg = recvMsg;
}

void Server::sendMsgTo(const std::string &client, const std::string &msg)
{
    auto clientSock = clients[client];
    static char sendMsg[MAX_BUF];
    if (msg.length() > MAX_BUF)
        throw ServerError("send too many messages.");

    send(clientSock, sendMsg, strlen(sendMsg) * sizeof(char), NULL);
}

#else

void Server::init(char *ipAddress, int port)
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(ipAddress);
    sockAddr.sin_port = htons(port);
    int bindRes = bind(serverSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    listen(serverSock, 20);
    if (bindRes != 0)
        throw ServerError(std::to_string(bindRes));
}

void Server::close()
{
    closesocket(serverSock);
    WSACleanup();
}

std::string Server::connectClient()
{
    auto client = accept(serverSock, NULL, NULL);

    std::string clientName = std::to_string((unsigned int)client);
    if (!clients.count(clientName))
        clients[clientName] = client;

    return clientName;
}

void Server::disconnectClient(const std::string &client)
{
    if (!clients.count(client))
        throw ServerError("client " + client + " not found.");
    closesocket(clients[client]);
}

void Server::recvMsgFrom(const std::string &client, std::string &msg)
{
    auto clientSock = clients[client];
    static char recvMsg[MAX_BUF];

    int res = recv(clientSock, recvMsg, sizeof recvMsg, NULL);
    //if (res != 0)
    //    throw ServerError("receiving message error code " + std::to_string(res));

    msg = recvMsg;
}

void Server::sendMsgTo(const std::string &client, const std::string &msg)
{
    auto clientSock = clients[client];
    static char sendMsg[MAX_BUF];
    if (msg.length() > MAX_BUF)
        throw ServerError("send too many messages.");

    send(clientSock, sendMsg, strlen(sendMsg) * sizeof(char), NULL);
}

#endif