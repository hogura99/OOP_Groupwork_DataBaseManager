#pragma once

namespace Server
{

    const size_t MAXBUF = 65536;

#ifdef WIN32

#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")

    SOCKET serverSock;

    static int initServer(char *ip_address, int port = 1234)
    {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
        serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        sockaddr_in sockAddr;
        memset(&sockAddr, 0, sizeof(sockAddr));
        sockAddr.sin_family = PF_INET;
        sockAddr.sin_addr.s_addr = inet_addr(ip_address);
        sockAddr.sin_port = htons(port);
        int bindRes = bind(serverSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
        listen(serverSock, 20);
        return bindRes;
    }

    static void closeServer()
    {
        closesocket(serverSock);
        WSACleanup();
    }

    static SOCKET connectClient()
    {
        /*SOCKADDR_IN clientAddr;
        size_t clientAddrSize = sizeof(clientAddr);
        SOCKET client = accept(serverSock, (SOCKADDR*)&clientAddr, &clientAddrSize);*/
        SOCKET client = accept(serverSock, NULL, NULL);
        return client;
    }

    static SOCKET connectClient(SOCKADDR_IN &clientAddr, int &clientAddrSize)
    {
        SOCKET client = accept(serverSock, (SOCKADDR*)&clientAddr, &clientAddrSize);
        return client;
    }

    static void disconnectClient(SOCKET clientSock)
    {
        closesocket(clientSock);
    }

    static int recvMsgFromClient(SOCKET &clientSock, char *msg, int msgLength)
    {
        int res = recv(clientSock, msg, msgLength * sizeof(char), NULL);
        return res;
    }

    static void sendMsgToClient(const SOCKET &clientSock, const char *msg, int msgLength)
    {
        send(clientSock, msg, msgLength * sizeof(char), NULL);
    }

#undef DELETE

#elif linux

    static int initServer(char *ip_address, int port = 1234)
    {
        return 0;
    }

    static void closeServer()
    {

    }

#endif

}