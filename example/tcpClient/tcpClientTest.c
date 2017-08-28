#include "tcpClient.h"

tcpClient client;
void exit_Handler(void)
{
    printf("Close TCPClient\n");
    tcpClient_close(&client);
}

int main(void)
{
    int ret; 
    atexit(exit_Handler);

    tcpClient_init(&client);

    if(tcpClient_create(&client) != CSOCKET_SUCCESS)
    {
        printf("%s\n", tcpClient_getErrMsg(&client));
        return 0;
    }

    if(tcpClient_connect_I2(&client, "127.0.0.1", 9001) != CSOCKET_SUCCESS)
    {
        printf("%s\n", tcpClient_getErrMsg(&client));
        return 0;
    }

    tcpConnect *connect = tcpClient_getConnect(&client);
    int i = 0;
    while(1)
    {
        
        char buf[128] = {0};
        sprintf(buf, "tcpClient test%d\n", i++);
        tcpConnect_write(connect, buf, strlen(buf));
        OnSyncEvent(connect)
        {
            EventHandler

            OnFail()
            {
                printf("client: %s\n", tcpConnect_getErrMsg(connect));
                break;
            }

           OnEnd()
           {
                printf("client: end\n");
                break;
           }

           OnSuccess()
           {
               printf("client send %s\n", buf);
           } 
        }


        tcpConnect_read_timeout(connect, buf, 128, 5, 0);
        OnSyncEvent(connect)
        {
            EventHandler

            OnFail()
            {
                printf("client: %s\n", tcpConnect_getErrMsg(connect));
                break;
            }

           OnEnd()
           {
                printf("client: end\n");
                break;
           }

           OnTimeout()
           {
                printf("client: timeout\n");
                break;
           }

           OnSuccess()
           {
               printf("client get %s\n", buf);
           } 
        }

        usleep(1000);

    }
    return 0;

}