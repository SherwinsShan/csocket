#ifndef _TCPCONNECTNORMAL_H
#define _TCPCONNECTNORMAL_H

#include "libcsocket.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct _tcpConnect
{
    BASE_CSOCKET_STRUCT

    void*   server;
}tcpConnect;




int tcpConnect_init(tcpConnect* socketConnect);
int tcpConnect_close(tcpConnect* socketConnect);

int tcpConnect_read        (tcpConnect* socketConnect, char* buffer, int bufferLen);
int tcpConnect_read_timeout(tcpConnect* socketConnect, char* buffer, int bufferLen, time_t s, suseconds_t us);

int tcpConnect_write         (tcpConnect* socketConnect, char* buffer, int bufferLen);
int tcpConnect_write_timeout(tcpConnect* socketConnect, char* buffer, int bufferLen, time_t s, suseconds_t us);

const char*    tcpConnect_getIp(tcpConnect* socketConnect);
unsigned short tcpConnect_getPort(tcpConnect* socketConnect);

const char* tcpConnect_getErrMsg(tcpConnect* server);
int         tcpConnect_getErrno(tcpConnect* server);

void tcpConnect_set_writeFlag  (tcpConnect* socketConnect, int flags);
void tcpConnect_reset_writeFlag(tcpConnect* socketConnect);

void tcpConnect_set_readFlag  (tcpConnect* socketConnect, int flags);
void tcpConnect_reset_readFlag(tcpConnect* socketConnect);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
