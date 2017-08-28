#include "tcpConnect.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//************************* tcpConnect Funciton **************************//
static const char* const tcpConnectErrMsg[] = {
        "No error",
        "tcpConnect write error",
        "tcpConnect write select error",
        "tcpConnect read error",
        "tcpConnect read select error"
};

const char* tcpConnect_getErrMsg(tcpConnect* server)
{
    static char buf[128];

    csocket_error_lock(server); 
    sprintf(buf, "%s", tcpConnectErrMsg[server->userErrno]);
    if(server->sysErrno == 0)
        sprintf(buf + strlen(buf), "\n");
    else
        sprintf(buf + strlen(buf),": %s\n",strerror(server->sysErrno));
    csocket_error_unlock(server);

    return buf;
}

int tcpConnect_getErrno(tcpConnect* server)
{
    return server->userErrno;
}
//*********************************************************************//

int tcpConnect_init(tcpConnect* socketConnect)
{
    socketConnect->sysErrno = 0;
    socketConnect->userErrno = 0;
    socketConnect->readFlags = 0;
    socketConnect->writeFlags = 0;
    socketConnect->type = CSOCKET_TYPE_TCPCONNECT;
    memset(&socketConnect->address, 0, sizeof(struct sockaddr_in));

    socketConnect->ret = csocket_locks_init(socketConnect);
    return socketConnect->ret;
}

int tcpConnect_close(tcpConnect* socketConnect)
{
    close(socketConnect->fd);
    socketConnect->ret = csocket_locks_destroy(socketConnect);
    return socketConnect->ret;
}

int tcpConnect_read(tcpConnect* socketConnect, char* buffer, int bufferLen)
{
    return csocket_tcp_read(socketConnect, buffer, bufferLen);
}

int tcpConnect_read_timeout(tcpConnect* socketConnect, char* buffer, int bufferLen, time_t s, suseconds_t us)
{
    return csocket_tcp_read_timeout(socketConnect, buffer, bufferLen, s, us);
    
}

int tcpConnect_write(tcpConnect* socketConnect, char* buffer, int bufferLen)
{
    return csocket_tcp_write(socketConnect, buffer, bufferLen);
}

int tcpConnect_write_timeout(tcpConnect* socketConnect, char* buffer, int bufferLen, time_t s, suseconds_t us)
{
    return csocket_tcp_write_timeout(socketConnect, buffer, bufferLen, s, us);
    
}

void tcpConnect_set_writeFlag(tcpConnect* socketConnect, int flags)
{
    socketConnect->writeFlags = flags;
}

void tcpConnect_reset_writeFlag(tcpConnect* socketConnect)
{
    socketConnect->writeFlags = 0;
}

void tcpConnect_set_readFlag(tcpConnect* socketConnect, int flags)
{
    socketConnect->readFlags = flags;
}

void tcpConnect_reset_readFlag(tcpConnect* socketConnect)
{
    socketConnect->readFlags = 0;
}

const char* tcpConnect_getIp(tcpConnect* socketConnect)
{
    static char ip[16];
    csocket_buf_lock(socketConnect);
    memset(ip, 0, 16);
    getIpFromAddress(&(socketConnect->address), ip);
    csocket_buf_unlock(socketConnect);

    return ip;
}

unsigned short tcpConnect_getPort(tcpConnect* socketConnect)
{
    return getPortFromAddress(&socketConnect->address);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */