#include "tcpServer.h"


bool isStop = false;

void sigpipe_handler(int signo)
{
    printf("sigpipe\n");
}

void *newConectionThread(void* arg)
{
    pthread_detach(pthread_self());
    signal(SIGPIPE, sigpipe_handler);


    tcpConnect* connect = (tcpConnect*)arg;
    printf("thread %ld: %s:%d connect in\n", (unsigned long)pthread_self(), tcpConnect_getIp(connect), tcpConnect_getPort(connect));
    while(1)
    {
        char buf[128] = {0};
        tcpConnect_read_timeout(connect, buf, 128, 10, 0);
        OnSyncEvent(connect)
        {
            EventHandler
            OnFail()
            {
                printf("thread %ld: %s\n", (unsigned long)pthread_self(), tcpConnect_getErrMsg(connect));
                break;
            }
            OnEnd()
            {
                printf("thread %ld: end\n" , (unsigned long)pthread_self());
                break;
            }

            OnTimeout()
            {
                printf("thread %ld: tcpConnect_read_timeout\n", (unsigned long)pthread_self());
                break;
            }

            OnSuccess()
            {
                printf("thread %ld: getMsg %s\n", (unsigned long)pthread_self(), buf);
                if(strstr(buf, "quit") != NULL)
                {
                    break;
                }
            }
        }

        

        char buf2[128] = {0};
        sprintf(buf2, "thread %ld: ack: %s\n", (unsigned long)pthread_self(), buf);
        tcpConnect_write(connect, buf2, strlen(buf2));
        OnSyncEvent(connect)
        {
            EventHandler
            OnFail()
            {
                printf("thread %ld: %s\n", (unsigned long)pthread_self(), tcpConnect_getErrMsg(connect));
                break;
            }
            OnEnd()
            {
                printf("thread %ld: end\n" , (unsigned long)pthread_self());
                break;
            }
            OnSuccess()
            {
                printf("thread %ld: %s\n", (unsigned long)pthread_self(), buf2);
            }
        }

    }
    tcpConnect_close(connect);
    free(connect);
    printf("thread %ld quit\n", (unsigned long)pthread_self());
}

int main(void)
{
    int ret;
    tcpServer server;
    tcpServer_init_I1(&server, 9001, 10);
    //tcpServer_init_I2(&server,"172.18.21.196", 9001, 10);
    if( tcpServer_create(&server) != CSOCKET_SUCCESS)
    {
        printf("%s\n",tcpServer_getErrMsg(&server));
        return 0;
    }
    printf("createTcpServer successful\n");
    tcpConnect* connect = (tcpConnect*) malloc(sizeof(tcpConnect));
    
    while(!isStop)
    {
        printf("wait for connect\n");
        tcpServer_waitConnect_timeout(&server, connect, 5, 0);

        OnSyncEvent(&server)
        {
            EventHandler
            OnFail()
            {
                printf("%s\n", tcpServer_getErrMsg(&server));
                continue;
            }

            OnTimeout()
            {
                printf("tcpServer_waitConnect_timeout\n");
                continue;
            }

            OnSuccess()
            {
                pthread_t tid;
                pthread_create(&tid, NULL, newConectionThread, connect);
                printf("create new thread: %ld\n",(unsigned long)tid);
                connect = (tcpConnect*) malloc(sizeof(tcpConnect));
            }
        }
    }
    tcpServer_close(&server);
    return 0;
}