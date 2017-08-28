#include "udpServer.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//************************************* ERROR LOG ***************************//

static const char* udpServerErrMsg[] = {
        "No error",
        "udpServer create error",
        "udpServer bind error",
        "udpServer recv error",
        "udpServer selete read error",
        "udpServer write error",
        "udpServer create connect err"
    };

const char* udpServer_getErrMsg(udpServer* server)
{
    static char buf[64];

    csocket_error_lock(server); 
    sprintf(buf, "%s", udpServerErrMsg[server->userErrno]);
    if(server->sysErrno == 0)
        sprintf(buf + strlen(buf), "\n");
    else
        sprintf(buf + strlen(buf),": %s\n",strerror(server->sysErrno));
    csocket_error_unlock(server);

    return buf;
}

int udpServer_getErrno(udpServer* server)
{
    return server->userErrno;
}

//*************************** SetUp Function *****************************/

int udpServer_init_I1(udpServer* server, unsigned short port, int maxListen)
{
    server->sysErrno = 0;
    server->userErrno = 0;
    server->readFlags = 0;
    server->writeFlags = 0;
    server->type = CSOCKET_TYPE_UDPSERVER;
    server->maxListenCount = maxListen;

    memset(&server->address, 0, sizeof(struct sockaddr_in));
    server->address.sin_family = AF_INET;  
    server->address.sin_addr.s_addr = htonl(INADDR_ANY);  
    server->address.sin_port = htons(port); 
    server->ret = csocket_locks_init(server);;
    return server->ret;
}
int udpServer_init_I2(udpServer* server, const char* ip, unsigned short port, int maxListen)
{
    server->sysErrno = 0;
    server->userErrno = 0;
    server->readFlags = 0;
    server->writeFlags = 0;
    server->type = CSOCKET_TYPE_UDPSERVER;
    server->maxListenCount = maxListen;

    memset(&server->address, 0, sizeof(struct sockaddr_in));
    server->address.sin_family = AF_INET;  
    server->address.sin_addr.s_addr = inet_addr(ip);  
    server->address.sin_port = htons(port); 
    server->ret = csocket_locks_init(server);;
    return server->ret;
}

int udpServer_init(udpServer* server, struct sockaddr_in* address ,int maxListen)
{
    server->sysErrno = 0;
    server->userErrno = 0;
    server->readFlags = 0;
    server->writeFlags = 0;
    server->type = CSOCKET_TYPE_UDPSERVER;
    server->maxListenCount = maxListen;

    memcpy(&server->address, address, sizeof(struct sockaddr_in));
    server->ret = csocket_locks_init(server);;
    return server->ret;
}


int udpServer_create(udpServer* server)
{
    int ret = socket(AF_INET, SOCK_DGRAM, 0);
    if(ret == -1)
    {
        csocket_setErrno(server, UDP_SERVER_ERR_CREATE);
        server->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }
    server->fd = ret;

    ret = bind(server->fd, (struct sockaddr *)&server->address, sizeof(struct sockaddr_in));  
    if(ret == -1)
    {
        csocket_setErrno(server, UDP_SERVER_ERR_BIND);
        server->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }
    server->ret = CSOCKET_SUCCESS;
    return CSOCKET_SUCCESS;
}


int udpServer_close(udpServer* server)
{
    close(server->fd);
    server->ret = csocket_locks_destroy(server);;
    return server->ret;
}

//*************************** RW Function *****************************/

int udpServer_broadcast_setEnable(udpServer* server, bool enable)
{
    int flag = enable?1:0;
    setsockopt(server->fd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag)); 
    server->ret = CSOCKET_SUCCESS;
    return CSOCKET_SUCCESS;
}


int udpServer_waitDataConnect(udpServer* server, udpConnect* connect, char* buf, int bufLen)
{
    socklen_t len = sizeof(struct sockaddr_in);
    struct sockaddr_in addr = {0};
    int ret = recvfrom(server->fd, buf, bufLen, 0, (struct sockaddr*)&addr, &len);
    if(ret == -1)
    {
        csocket_setErrno(server, UCP_SERVER_ERR_RECV);
        server->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }

    if(udpConnect_init(connect) != CSOCKET_SUCCESS)
    {
        csocket_setErrno(server, UDP_SERVER_ERR_CREATE_CONNECT);
        server->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }

    connect->fd = server->fd;
    memcpy(&(connect->address), &addr, sizeof(struct sockaddr_in));
    connect->server = server;
    server->ret = ret;
    return ret;
}

int udpServer_waitDataConnect_timeout(udpServer* server, udpConnect* connect, char* buf, int bufLen, time_t s, suseconds_t us)
{
    int ret = select_read(server->fd, s, us);
    if(ret < 0)//error
    {
        csocket_setErrno(server, UDP_SERVER_ERR_SELECT_READ);
        server->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }
    else if(ret == 0)//timeout
    {
        server->ret = CSOCKET_TIMEOUT;  
        return CSOCKET_TIMEOUT;   
    }
    else//readable
    {
        return udpServer_waitDataConnect(server, connect, buf, bufLen); 
    }
}


int udpServer_sendTo(udpServer* server, struct sockaddr_in *clientAddr, char* buf, int bufLen)
{
    return csocket_udp_write(server, clientAddr, buf, bufLen);
}

int udpServer_sendTo_I2(udpServer* server, char* addr, unsigned short port, char* buf, int bufLen )
{
    struct sockaddr_in clientAddr;
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(port);
    clientAddr.sin_addr.s_addr = inet_addr(addr);
    return csocket_udp_write(server, &clientAddr, buf, bufLen);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */
