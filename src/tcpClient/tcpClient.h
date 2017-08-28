#ifndef _TCPCLIENTNORMAL_H
#define  _TCPCLIENTNORMAL_H

#include "tcpConnect.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define TCP_CLIENT_ERR_NO           0
#define TCP_CLIENT_ERR_CREATE       1
#define TCP_CLIENT_ERR_CONNECT      2

typedef tcpConnect  tcpClient;


const char* tcpClient_getErrMsg(tcpClient* client);
int         tcpClient_getErrno(tcpClient* client);


int tcpClient_init(tcpClient* client);
int tcpClient_create(tcpClient* client);
int tcpClient_close(tcpClient* client);

int tcpClient_connect   (tcpClient* client, struct sockaddr_in* addr_ser);
int tcpClient_connect_I2(tcpClient* client, const char *addr, unsigned short port);

tcpConnect* tcpClient_getConnect(tcpClient* client);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
