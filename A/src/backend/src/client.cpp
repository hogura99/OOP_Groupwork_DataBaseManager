#include "client.h"

#include <cstring>

void Client::sendMsgToServer(const std::string &msg)
{
    createConversation();
    _sendMsgToServer(msg);
    stopConversation();
}

void Client::recvMsgFromServer(std::string &msg)
{
    createConversation();
    _recvMsgFromServer(msg);
    stopConversation();
}

#ifdef WIN32

#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")

void Client::setServer(char *serverIpAddress, int port)
{
    _serverIpAddr = serverIpAddress;
    _serverPort = port;
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

void Client::buildClient()
{
    _socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
}

void Client::createConversation()
{
    buildClient();
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(_serverIpAddr.c_str());
    sockAddr.sin_port = htons(_serverPort);
    int res = connect(_socket, (SOCKADDR*)&sockAddr, sizeof (SOCKADDR));
    if (res != 0)
        throw ClientError("Connecting server error code: " + std::to_string(res));
}

void Client::_sendMsgToServer(const std::string &msg)
{
    static char sendMsg[MAX_BUF];

    if (msg.length() > MAX_BUF)
        throw ClientError("Client sending too many messages.");
    strcat(sendMsg, msg.c_str());

    send(_socket, sendMsg, MAX_BUF, NULL);

    memset(sendMsg, 0, strlen(sendMsg) * sizeof(char));
}

void Client::_recvMsgFromServer(std::string &msg)
{
    static char recvMsg[MAX_BUF];
    memset(recvMsg, 0, strlen(recvMsg) * sizeof(char));
    recv(_socket, recvMsg, MAX_BUF * sizeof(char), NULL);
    msg = recvMsg;
}

void Client::stopConversation()
{
    closesocket(_socket);
}

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

void Client::setServer(char *serverIpAddress, int port)
{
    _serverIpAddr = serverIpAddress;
    _serverPort = port;
}

void Client::buildClient()
{
    _socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
}

void Client::createConversation()
{
    buildClient();
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(_serverIpAddr.c_str());
    sockAddr.sin_port = htons(_serverPort);
    int res = connect(_socket, (sockaddr*)&sockAddr, sizeof (sockaddr));
    if (res != 0)
        throw ClientError("Create conversation error code: " + std::to_string(res));
}

void Client::_sendMsgToServer(const std::string &msg)
{
    if (msg.length() > MAX_BUF)
        throw ClientError("Client sending too many messages.");

    send(_socket, msg.c_str(), msg.length() * sizeof(char), NULL);
}

void Client::_recvMsgFromServer(std::string &msg)
{
    static char recvMsg[MAX_BUF];
    memset(recvMsg, 0, strlen(recvMsg) * sizeof(char));
    recv(_socket, recvMsg, MAX_BUF * sizeof(char), NULL);
    msg = recvMsg;
}

void Client::stopConversation()
{
    close(_socket);
}

#endif