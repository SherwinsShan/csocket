#ifndef _UDPCONNECTNORMAL_H
#define _UDPCONNECTNORMAL_H

#include "libcsocket.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct _udpConnect
{
    BASE_CSOCKET_STRUCT

    void* server;
}udpConnect;


int udpConnect_init(udpConnect* socketConnect);
int udpConnect_close(udpConnect* socketConnect);

int udpConnect_read        (udpConnect* socketConnect, char* buffer, int bufferLen);
int udpConnect_read_timeout(udpConnect* socketConnect, char* buffer, int bufferLen, time_t s, suseconds_t us);

int udpConnect_write        (udpConnect* socketConnect, char* buffer, int bufferLen);
int udpConnect_write_timeout(udpConnect* socketConnect, char* buffer, int bufferLen, time_t s, suseconds_t us);

void udpConnect_set_writeFlag  (udpConnect* socketConnect, int flags);
void udpConnect_reset_writeFlag(udpConnect* socketConnect);

void udpConnect_set_readFlag  (udpConnect* socketConnect, int flags);
void udpConnect_reset_readFlag(udpConnect* socketConnect);

const char*    udpConnect_getIp  (udpConnect* socketConnect);
unsigned short udpConnect_getPort(udpConnect* socketConnect);

const char* udpConnect_getErrMsg(udpConnect* server);
int         udpConnect_getErrno (udpConnect* server);    



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
