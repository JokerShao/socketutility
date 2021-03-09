#include <stdio.h>
#include <stdlib.h>
#include "socketbase.h"


int main()
{
    TCPSocketServerBase server;
    server.init("127.0.0.1", 6743);
    char* recvbuf = (char*)malloc(100);

    while (1)
    {
        memset(recvbuf, 0, sizeof(recvbuf));
        if (server.recvData(recvbuf, 100) != 0) { // 接收状态不正确
            break;
        }
        if (strcmp(recvbuf, "20") == 0)
            break;
        printf("Server receive##   %s   ##\n", recvbuf);
    }

    free(recvbuf);
    recvbuf = nullptr;
    server.release();
    getchar();
    return 0;
}

