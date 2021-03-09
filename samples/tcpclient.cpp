#include <stdio.h>
#include <string>
#include "socketbase.h"


int main()
{
    TCPSocketClientBase client;
    client.init("127.0.0.1", 6743);

    int cnt = 0;
    char* pbuffer = (char*)malloc(100);

    while (1)
    {
        memset(pbuffer, 0, 100);
        memcpy(pbuffer, std::to_string(cnt).c_str(), std::to_string(cnt).size());
        if (client.sendData((void*)pbuffer, 100) != 0)
            break;
        if (cnt++ >= 20)
            break;
        getchar();
    }

    free(pbuffer);
    pbuffer = nullptr;
    client.release();
    getchar();
    return 0;
}

