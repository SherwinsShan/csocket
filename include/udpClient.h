#ifndef _UDPCLIENTNORMAL_H
#define _UDPCLIENTNORMAL_H

#include "udpConnect.h"


#define UDP_CLIENT_ERR_NO           0
#define UDP_CLIENT_ERR_CREATE       1
#define UDP_CLIENT_ERR_RECV         2
#define UDP_CLIENT_ERR_WRITE        3
#define UDP_CLIENT_ERR_SELECT_READ  4


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef udpConnect udpClient;

int udpClient_init  (udpClient* client);
int udpClient_close (udpClient* client);
int udpClient_create(udpClient* client);



int udpClient_getDataFrom   (udpClient *client,           struct sockaddr_in *addr, char *data, int dataLength);
int udpClient_getDataFrom_I2(udpClient *client, char* addrStr, unsigned short port, char *data, int dataLength);

int udpClient_getDataFrom_timeout   (udpClient *client,           struct sockaddr_in *addr, char *data, int dataLength, time_t s, suseconds_t us);
int udpClient_getDataFrom_I2_timeout(udpClient *client, char *addrStr, unsigned short port, char *data, int dataLength, time_t s, suseconds_t us);

int udpClient_sendDataTo   (udpClient *client,           struct sockaddr_in *addr, char *data, int dataLength);
int udpClient_sendDataTo_I2(udpClient *client, char *addrStr, unsigned short port, char *data, int dataLength);


int udpClient_broadcast_setEnable(udpClient* client, bool enable);

const char* udpClient_getErrMsg(udpClient* client);
int         udpClient_getErrno (udpClient* client);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
