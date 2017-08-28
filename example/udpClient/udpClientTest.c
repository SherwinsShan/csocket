#include "udpClient.h"

int main(void)
{
    udpClient client;
    int ret;
    udpClient_init(&client);

    if(udpClient_create(&client) != CSOCKET_SUCCESS)
    {
        printf("%s\n", udpClient_getErrMsg(&client));
        return 0;
    }

    int i = 0;
    while(1)
    {
        char buf[128];
        sprintf(buf, "udpClient test%d",i++);
        udpClient_sendDataTo_I2(&client, "127.0.0.1", 9002, buf, strlen(buf));
        OnSyncEvent(&client)
        {
            EventHandler
            OnFail()
            {
                printf("%s\n", udpClient_getErrMsg(&client));
                sleep(1);
                continue;
            }

            OnSuccess()
            {
                printf("send %s\n", buf);
            }
        }

        udpClient_getDataFrom_I2_timeout(&client, "127.0.0.1", 9002, buf, 128, 5, 0);
        OnSyncEvent(&client)
        {
            EventHandler
            OnFail()
            {
                printf("%s\n", udpClient_getErrMsg(&client));
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
                printf("get %s from %s:%d\n", buf, udpConnect_getIp(&client), udpConnect_getPort(&client));
            }
        }
        sleep(1);
    }
    udpClient_close(&client);
    return 0;
}