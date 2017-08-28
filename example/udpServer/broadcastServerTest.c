#include "udpServer.h"

udpServer server;

void *listenerThread(void *arg)
{
    while(1)
    {
        char buf[1024] = {0};
        printf("waiting\n");
        udpConnect connect;
        udpServer_waitDataConnect_timeout(&server, &connect, buf, 1024, 3, 0);
        OnSyncEvent(&server)
        {
            EventHandler
            OnFail()
            {
                printf("%s\n", udpServer_getErrMsg(&server));
                sleep(1);
                continue;
            }
            OnTimeout()
            {
                printf("timeout\n");
                sleep(1);
                continue;
            }

            OnSuccess()
            {
                printf("receive from: %s:%d   : %s\n", \
                    udpConnect_getIp(&connect), \
                    udpConnect_getPort(&connect), \
                    buf);
                udpConnect_close(&connect);
            }
        }
    }

}
void main(void)
{
    
    udpServer_init_I2(&server, "172.18.22.61", 8000, 10);
    if(udpServer_create(&server) != CSOCKET_SUCCESS)
    {
        printf("%s\n", udpServer_getErrMsg(&server));
        return ;
    }
    pthread_t tid;
    if(pthread_create(&tid, NULL, listenerThread, NULL) != 0)
    {
        printf("create thread fail\n");
        udpServer_close(&server);
        return ;
    }

    udpServer_broadcast_setEnable(&server, true);
    int i = 0;
    while(1)
    {
        char p[64];
        sprintf(p, "broadcast test%d", i++);
        udpServer_sendTo_I2(&server,"172.18.255.255", 6345, p, strlen(p));
        OnSyncEvent(&server)
        {
            EventHandler
            OnFail()
            {
                printf("%s\n", udpServer_getErrMsg(&server));
            }

            OnSuccess()
            {
                printf("send %s\n",p);
            }
        }
       sleep(2);

    }
    
}