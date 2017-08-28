#include "udpServer.h"

void main(void)
{
    udpServer server;
    udpServer_init_I2(&server, "172.18.22.61", 6345, 1);
    if(udpServer_create(&server) != CSOCKET_SUCCESS)
    {
        printf("%s\n", udpServer_getErrMsg(&server));
        return ;
    }
    int count = 0;
    while(1)
    {
        
        char buf[128] = {0};
        udpConnect connect;
        udpServer_waitDataConnect(&server, &connect, buf, 128);
        OnSyncEvent(&server)
        {
            EventHandler
            OnFail()
            {
                printf("%s\n", udpServer_getErrMsg(&server));
                continue ;
            }
            OnSuccess()
            {
                printf("get msg:%s from %s:%d\n", buf, udpConnect_getIp(&connect), udpConnect_getPort(&connect));
            }
        }



        sprintf(buf, "broadcast ack%d", count++);
        udpConnect_write(&connect, buf, strlen(buf));
        OnSyncEvent(&connect)
        {
            EventHandler
            OnFail()
            {
                printf("%s\n", udpConnect_getErrMsg(&connect));
                continue ;
            }
            OnSuccess()
            {
                printf("send %s\n", buf);
            }
        }
        


        
    }

}