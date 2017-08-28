#include "udpConnect.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//************************* udpConnect Funciton **************************//
static const char* const udpConnectErrMsg[] = {
        "No error",
        "udpConnect write error",
        "udpConnect write select error",
        "udpConnect read error",
        "udpConnect read select error"
        
};

const char* udpConnect_getErrMsg(udpConnect* server)
{
    static char buf[64];

    csocket_error_lock(server); 
    sprintf(buf, "%s", udpConnectErrMsg[server->userErrno]);
    if(server->sysErrno == 0)
        sprintf(buf + strlen(buf), "\n");
    else
        sprintf(buf + strlen(buf),": %s\n",strerror(server->sysErrno));
    csocket_error_unlock(server);

    return buf;
}

int udpConnect_getErrno(udpConnect* server)
{
    return server->userErrno;
}
//*********************************************************************//

int udpConnect_init(udpConnect* socketConnect)
{
    socketConnect->sysErrno = 0;
    socketConnect->userErrno = 0;
    socketConnect->readFlags = 0;
    socketConnect->writeFlags = 0;
    socketConnect->type = CSOCKET_TYPE_UDPCONNECT;

    memset(&socketConnect->address, 0, sizeof(struct sockaddr_in));

    socketConnect->ret = csocket_locks_init(socketConnect);
    return socketConnect->ret;
}

int udpConnect_close(udpConnect* socketConnect)
{
    //close(socketConnect->fd);
    socketConnect->ret = csocket_locks_destroy(socketConnect);
    return socketConnect->ret;
}

int udpConnect_read(udpConnect* socketConnect, char* buf, int bufLen)
{
    return csocket_udp_read(socketConnect, &socketConnect->address, buf, bufLen);
}

int udpConnect_read_timeout(udpConnect* socketConnect, char* buf, int bufLen, time_t s, suseconds_t us)
{
    return csocket_udp_read_timeout(socketConnect, &socketConnect->address, buf, bufLen, s, us);
}

int udpConnect_write(udpConnect* socketConnect, char* buf, int bufLen)
{
    return csocket_udp_write(socketConnect, &socketConnect->address, buf, bufLen);
}

int udpConnect_write_timeout(udpConnect* socketConnect, char* buf, int bufLen, time_t s, suseconds_t us)
{
    return csocket_udp_write_timeout(socketConnect, &socketConnect->address, buf, bufLen, s, us);
}

void udpConnect_set_writeFlag(udpConnect* socketConnect, int flags)
{
    socketConnect->writeFlags = flags;
}

void udpConnect_reset_writeFlag(udpConnect* socketConnect)
{
    socketConnect->writeFlags = 0;
}

void udpConnect_set_readFlag(udpConnect* socketConnect, int flags)
{
    socketConnect->readFlags = flags;
}

void udpConnect_reset_readFlag(udpConnect* socketConnect)
{
    socketConnect->readFlags = 0;
}

const char* udpConnect_getIp(udpConnect* socketConnect)
{
    static char ip[16];
    csocket_buf_lock(socketConnect);
    memset(ip, 0, 16);
    getIpFromAddress(&(socketConnect->address), ip);
    csocket_buf_unlock(socketConnect);

    return ip;
}


unsigned short udpConnect_getPort(udpConnect* socketConnect)
{
    return getPortFromAddress(&socketConnect->address);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */