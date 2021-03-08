#include <iostream>
#include <string>

#include "socketbase.h"


int main()
{
    UDPSocketClientBase client;
    client.init("127.0.0.1", 9800);

    int cnt = 0;
    char* pbuffer = new char[100];

    while (1)
    {
        memset(pbuffer, 0, 100);
        memcpy(pbuffer, std::to_string(cnt).c_str(), std::to_string(cnt).size());
        if (client.sendData((void*)pbuffer, 100) != 0)
            break;
        std::cout << cnt << "\n";

        if (cnt++ >= 20)
            break;
        system("pause");
    }

    delete[] pbuffer;
    client.close();
    system("pause");
    return 0;
}

