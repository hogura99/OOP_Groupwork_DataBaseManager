#pragma once

namespace Client
{

#include <iostream>
#include <cstdlib>
#include <cstdio>

#ifdef WIN32

#include "winsock2.h"

    const int MAXBUF = 65536;

    sockaddr_in sockAddr;
    SOCKET clientSock;

    static void initClient(char *server_ip_address, int port = 1234)
    {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);

        memset(&sockAddr, 0, sizeof(sockAddr));
        sockAddr.sin_family = PF_INET;
        sockAddr.sin_addr.s_addr = inet_addr(server_ip_address);
        sockAddr.sin_port = htons(port);
    }

    static void createClient()
    {
        clientSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    }

    static int createConversation()
    {
        createClient();
        int res = connect(clientSock, (SOCKADDR*)&sockAddr, sizeof (SOCKADDR));
        return res;
    }

    static void sendMsgToServer(char *msg)
    {
        send(clientSock, msg, MAXBUF, NULL);
    }

    static void recvMsgFromServer(char *msg)
    {
        send(clientSock, msg, MAXBUF, NULL);
    }

    static void stopConversation()
    {
        closesocket(clientSock);
    }

#elif linux

#endif
}