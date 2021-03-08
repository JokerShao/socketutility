#include <iostream>

#include "socketbase.h"


int main()
{
    UDPSocketServerBase server;
    server.init("127.0.0.1", 9800);
    char* recvbuf = new char[100];

    while (1)
    {
        memset(recvbuf, 0, sizeof(recvbuf));
        if (server.recvData(recvbuf, 100) != 0) { // 接收状态不正确
            auto stat = WSAGetLastError();
            std::cout << "stat: " << stat << "\n";
            continue;//break;
        }
        if (strcmp(recvbuf, "20") == 0)
           break;
        std::cout << "Server receive##   " << recvbuf << "   ##\n";
    }

    delete[] recvbuf;
    recvbuf = nullptr;
    server.close();
    system("pause");
    return 0;
}

