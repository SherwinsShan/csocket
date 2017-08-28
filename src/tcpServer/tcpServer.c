#include "tcpServer.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//****************************** ERROR LOG ***************************//
static const char* tcpServerErrMsg[] = {
        "No error",
        "tcpServer create error",
        "tcpServer bind error",
        "tcpServer listen error",
        "tcpServer accept error",
        "tcpServer create connect error",
        "tcpServer select read error"
};

const char* tcpServer_getErrMsg(tcpServer* server)
{
    static char buf[64];

    csocket_error_lock(server); 
    sprintf(buf, "%s", tcpServerErrMsg[server->userErrno]);
    if(server->sysErrno == 0)
        sprintf(buf + strlen(buf), "\n");
    else
        sprintf(buf + strlen(buf),": %s\n",strerror(server->sysErrno));
    csocket_error_unlock(server);

    return buf;
}

int tcpServer_getErrno(tcpServer* server)
{
    return server->userErrno;
}

//********************** tcpServer Function *****************************/
int tcpServer_init_I1(tcpServer* server, unsigned short port ,int maxListen)
{
    server->sysErrno = 0;
    server->userErrno = 0;
    server->readFlags = 0;
    server->writeFlags = 0;
    server->maxListenCount = maxListen;
    server->type = CSOCKET_TYPE_TCPSERVER;

    memset(&server->address, 0, sizeof(struct sockaddr_in));
    server->address.sin_family = AF_INET;  
    server->address.sin_addr.s_addr = htonl(INADDR_ANY);  
    server->address.sin_port = htons(port); 
    server->ret = csocket_locks_init(server);
    return server->ret;
}
int tcpServer_init_I2(tcpServer* server, char* ip, unsigned short port, int maxListen)
{
    server->sysErrno = 0;
    server->userErrno = 0;
    server->readFlags = 0;
    server->writeFlags = 0;
    server->maxListenCount = maxListen;
    server->type = CSOCKET_TYPE_TCPSERVER;

    memset(&server->address, 0, sizeof(struct sockaddr_in));
    server->address.sin_family = AF_INET;  
    server->address.sin_addr.s_addr = inet_addr(ip);  
    server->address.sin_port = htons(port); 
    server->ret = csocket_locks_init(server);
    return server->ret;
}

int tcpServer_init(tcpServer* server, struct sockaddr_in* address , int maxListen)
{
    server->sysErrno = 0;
    server->userErrno = 0;
    server->readFlags = 0;
    server->writeFlags = 0;
    server->maxListenCount = maxListen;
    server->type = CSOCKET_TYPE_TCPSERVER;

    memcpy(&server->address, address, sizeof(struct sockaddr_in));
    server->ret = csocket_locks_init(server);
    return server->ret;
}


int tcpServer_create(tcpServer* server)
{
    int ret = socket(AF_INET, SOCK_STREAM, 0);
    if(ret == -1)
    {
        csocket_setErrno(server, TCP_SERVER_ERR_CREATE);
        server->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }
    server->fd = ret;

    ret = bind(server->fd, (struct sockaddr *)&server->address, sizeof(struct sockaddr_in));  
    if(ret == -1)
    {
        csocket_setErrno(server, TCP_SERVER_ERR_BIND);
        server->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }

    ret = listen(server->fd, server->maxListenCount);  
    if(ret == -1)  
    {  
        csocket_setErrno(server, TCP_SERVER_ERR_LISTEN);
        server->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    } 

    server->ret = CSOCKET_SUCCESS;
    return CSOCKET_SUCCESS;
}


int tcpServer_close(tcpServer* server)
{
    close(server->fd);
    server->ret = csocket_locks_destroy(server);
    return server->ret;
}

int tcpServer_waitConnect(tcpServer* server, tcpConnect* socketConnect)
{
    socklen_t addr_len = sizeof(struct sockaddr_in);
    struct sockaddr_in addr = {0};
    int ret = accept(server->fd, (struct sockaddr *)&addr, &addr_len);  
    if(ret == -1)
    {
        csocket_setErrno(server, TCP_SERVER_ERR_ACCEPT);
        server->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }

    if(tcpConnect_init(socketConnect) != CSOCKET_SUCCESS)
    {
        csocket_setErrno(server, TCP_SERVER_ERR_CREATE_CONNECT);
        server->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }

    socketConnect->fd = ret;
    memcpy(&(socketConnect->address), &addr, sizeof(struct sockaddr_in));
    socketConnect->server = server;

    server->ret = CSOCKET_SUCCESS;
    return CSOCKET_SUCCESS;

}


int tcpServer_waitConnect_timeout(tcpServer* server, tcpConnect* socketConnect, time_t s, suseconds_t us)
{

    int ret = select_read(server->fd, s, us);
    if(ret < 0)//error
    {
        csocket_setErrno(server, TCP_SERVER_ERR_SELECT_READ);
        server->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }
    else if(ret == 0)//timeout
    {  
        server->ret = CSOCKET_TIMEOUT;
        return CSOCKET_TIMEOUT;   
    }
    else//server readable
    {
        return tcpServer_waitConnect(server, socketConnect);
    }

}

const char* tcpServer_getServerIp(tcpServer* server)
{
    static char ip[16];
    csocket_buf_lock(server);
    memset(ip, 0, 16);
    getIpFromAddress(&(server->address), ip);
    csocket_buf_unlock(server);

    return ip;
}

unsigned short tcpServer_getServerPort(tcpServer* server)
{
    return getPortFromAddress(&server->address);
}

int tcpServer_getMaxListen(tcpServer* server)
{
    return server->maxListenCount;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */



