#include "udpServer.h"


int main(void)
{
    udpServer server;
    int ret;

    udpServer_init_I1(&server, 9002, 2);

    if(udpServer_create(&server) != CSOCKET_SUCCESS)
    {
        printf("%s\n", udpServer_getErrMsg(&server));
        return 0;
    }
    int i= 0;
    while(1)
    {
        char buf[128];
        udpConnect connect;
        udpServer_waitDataConnect_timeout(&server, &connect, buf, 128, 5, 0);
        OnSyncEvent(&server)
        {
            EventHandler

            OnFail()
            {
                printf("%s\n", udpServer_getErrMsg(&server));
                break;
            }

            OnTimeout()
            {
                printf("timeout\n");
                continue;
            }

            OnSuccess()
            {
                printf("get %s from %s:%d\n", buf, udpConnect_getIp(&connect), udpConnect_getPort(&connect));
            }
        }
        
        sprintf(buf, "udpServer test%d\n", i++);
        udpConnect_write(&connect, buf, strlen(buf));
        OnSyncEvent(&connect)
        {
            EventHandler
            OnFail()
            {
                printf("%s\n", udpServer_getErrMsg(&server));
                break;
            }
            OnSuccess()
            {
                printf("send %s\n", buf);
            }
        }
        udpConnect_close(&connect); 
    }
    udpServer_close(&server);
    return 0;
}