#ifndef _TCPSERVERNORMAL_H
#define _TCPSERVERNORMAL_H

#include "tcpConnect.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define TCP_SERVER_ERR_NO               0
#define TCP_SERVER_ERR_CREATE           1
#define TCP_SERVER_ERR_BIND             2
#define TCP_SERVER_ERR_LISTEN           3
#define TCP_SERVER_ERR_ACCEPT           4
#define TCP_SERVER_ERR_CREATE_CONNECT   5
#define TCP_SERVER_ERR_SELECT_READ      6




typedef struct _tcpServerNormal
{
    BASE_CSOCKET_STRUCT
    
    int                 maxListenCount;   
}tcpServer;


int tcpServer_init   (tcpServer* server,  struct sockaddr_in* address , int maxListen);
int tcpServer_init_I1(tcpServer* server,           unsigned short port, int maxListen);
int tcpServer_init_I2(tcpServer* server, char* ip, unsigned short port, int maxListen);
int tcpServer_create (tcpServer* server);
int tcpServer_close  (tcpServer* server);


int tcpServer_waitConnect        (tcpServer* server, tcpConnect* socketConnect);
int tcpServer_waitConnect_timeout(tcpServer* server, tcpConnect* socketConnect, time_t s, suseconds_t us);

const char*    tcpServer_getServerIp  (tcpServer* server);
unsigned short tcpServer_getServerPort(tcpServer* server);
int            tcpServer_getMaxListen (tcpServer* server);

const char* tcpServer_getErrMsg(tcpServer* server);
int         tcpServer_getErrno (tcpServer* server);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
