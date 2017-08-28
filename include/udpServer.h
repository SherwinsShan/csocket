#ifndef _UDPSERVERNORMAL_H
#define _UDPSERVERNORMAL_H

#include "udpConnect.h"

#define UDP_SERVER_ERR_NO               0
#define UDP_SERVER_ERR_CREATE           1
#define UDP_SERVER_ERR_BIND             2
#define UCP_SERVER_ERR_RECV             3
#define UDP_SERVER_ERR_SELECT_READ      4
#define UDP_SERVER_ERR_WRITE            5
#define UDP_SERVER_ERR_CREATE_CONNECT   6

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _udpServerNormal
{
    BASE_CSOCKET_STRUCT

    int maxListenCount;
}udpServer;


int udpServer_init   (udpServer* server,     struct sockaddr_in* address    , int maxListen);
int udpServer_init_I1(udpServer* server,                 unsigned short port, int maxListen);
int udpServer_init_I2(udpServer* server, const char* ip, unsigned short port, int maxListen);


int udpServer_create(udpServer* server);
int udpServer_close (udpServer* server);

int udpServer_broadcast_setEnable(udpServer* server, bool enable);

int udpServer_waitDataConnect        (udpServer* server, udpConnect* connect, char* buf, int bufLen);
int udpServer_waitDataConnect_timeout(udpServer* server, udpConnect* connect, char* buf, int bufLen, time_t s, suseconds_t us);


int udpServer_sendTo   (udpServer* server,  struct sockaddr_in *clientAddr, char* buf, int bufLen);
int udpServer_sendTo_I2(udpServer* server, char* addr, unsigned short port, char* buf, int bufLen);


const char* udpServer_getErrMsg(udpServer* server);
int         udpServer_getErrno (udpServer* server);

const char*    udpServer_getServerIp  (udpServer* server);
unsigned short udpServer_getServerPort(udpServer* server);




#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
