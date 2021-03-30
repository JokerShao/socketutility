#include <stdio.h>
#include <string>
#include <thread>
#include "socketbase.h"


int main()
{
    UDPSocketClientBase client;
    client.init("127.0.0.1", 9800);

    int cnt = 0;
    char* pbuffer = (char*)malloc(100);

    while (1)
    {
        memset(pbuffer, 0, 100);
        memcpy(pbuffer, std::to_string(cnt).c_str(), std::to_string(cnt).size());
        if (client.sendData((void*)pbuffer, 100) != 0)
            break;
        printf("send %d messages.\n", cnt);
        if (cnt++ >= 20)
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    free(pbuffer);
    pbuffer = nullptr;
    client.release();
    getchar();
    return 0;
}

