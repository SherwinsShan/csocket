#ifndef _LIB_CSOCKET_H
#define _LIB_CSOCKET_H


#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <signal.h>

#include <sys/socket.h>  
#include <sys/types.h>  
#include <sys/select.h>  
#include <sys/time.h>

#include <unistd.h>  
#include <netinet/in.h>  
#include <netinet/tcp.h>
#include <arpa/inet.h> 

#ifndef SOL_TCP
#define SOL_TCP 6
#endif



#define CSOCKET_SUCCESS      1
#define CSOCKET_END         -1
#define CSOCKET_NODATA      -1
#define CSOCKET_FAIL        -2
#define CSOCKET_TIMEOUT     -3


#define CSOCKET_ERR_NONE            0
#define CSOCKET_ERR_WRITE           1
#define CSOCKET_ERR_SELECT_WRITE    2
#define CSOCKET_ERR_READ            3
#define CSOCKET_ERR_SELECT_READ     4




#define CSOCKET_TYPE_TCPCONNECT 0x01
#define CSOCKET_TYPE_TCPSERVER  0x02
#define CSOCKET_TYPE_TCPCLIENT  0x04
#define CSOCKET_TYPE_UDPCONNECT 0x08
#define CSOCKET_TYPE_UDPSERVER  0x10
#define CSOCKET_TYPE_UDPCLIENT  0x20


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define BASE_CSOCKET_STRUCT     int sysErrno; \
int userErrno; \
int ret;    \
int fd; \
int type; \
int readFlags; \
int writeFlags; \
struct sockaddr_in  address; \
pthread_mutex_t     errorLock; \
pthread_mutex_t     writeLock; \
pthread_mutex_t     readLock; \
pthread_mutex_t     bufLock;



//start-private


typedef struct
{
    BASE_CSOCKET_STRUCT
}csocketBase;


typedef void*  pcsocketBase_t;

//base
int csocket_locks_init(pcsocketBase_t ori);
int csocket_locks_destroy(pcsocketBase_t ori);

int csocket_error_lock(pcsocketBase_t ori);
int csocket_error_unlock(pcsocketBase_t ori);

int csocket_write_lock(pcsocketBase_t ori);
int csocket_write_unlock(pcsocketBase_t ori);

int csocket_read_lock(pcsocketBase_t ori);
int csocket_read_unlock(pcsocketBase_t ori);

int csocket_buf_lock(pcsocketBase_t ori);
int csocket_buf_unlock(pcsocketBase_t ori);

void csocket_setErrno(pcsocketBase_t ori, int eno);

void csocket_writeFlags_reset(pcsocketBase_t socket);
void csocket_readFlags_reset(pcsocketBase_t socket);

//tcp rw
int csocket_tcp_write        (pcsocketBase_t socket, char* buf, int len);
int csocket_tcp_write_timeout(pcsocketBase_t socket, char* buf, int len, time_t s, suseconds_t us);

int csocket_tcp_read        (pcsocketBase_t socket, char* buf, int len);
int csocket_tcp_read_timeout(pcsocketBase_t socket, char* buf, int len, time_t s, suseconds_t us);

//udp rw
int csocket_udp_write        (pcsocketBase_t ori, struct sockaddr_in* address, char* buf, int len);
int csocket_udp_write_timeout(pcsocketBase_t ori, struct sockaddr_in* address, char* buf, int len, time_t s, suseconds_t us);

int csocket_udp_read        (pcsocketBase_t socket, struct sockaddr_in* address, char* buf, int len);
int csocket_udp_read_timeout(pcsocketBase_t socket, struct sockaddr_in* address, char* buf, int len, time_t s, suseconds_t us);


//end-private


//commond
int select_read(int fd, time_t sec, suseconds_t usec);
int select_write(int fd, time_t sec, suseconds_t usec);

int setSocketReuse(int socket, bool bReuseaddr);
int setSocketSendTimeout(int socket,int ms);
int setSocketRecvTimeout(int socket,int ms);
int setSocketSendBufSize(int socket, int b);
int setSocketRecvBufSize(int socket, int b);
int setSocketKeepAlive(int socket, bool keepAlive);
int setSocketKeepIdle(int socket, int keepIdle);
int setSocketKeepInterval(int socket, int keepInterval);
int setSocketKeepCount(int socket, int keepCount);

void           getIpFromAddress(struct sockaddr_in* in, char* buf);
unsigned short getPortFromAddress(struct sockaddr_in* in);

//SyncEvent

extern int* csocket_ret_p;
extern pthread_mutex_t csocket_ret_mutex;


#define OnSyncEvent(x)  {pthread_mutex_lock(&csocket_ret_mutex); csocket_ret_p=&((x)->ret);}
#define EventHandler    int tcp_ret_tmp = *csocket_ret_p;pthread_mutex_unlock(&csocket_ret_mutex);if(0){}
#define OnFail()        else if(tcp_ret_tmp==CSOCKET_FAIL)
#define OnEnd()         else if(tcp_ret_tmp==CSOCKET_END)
#define OnNoData()      else if(tcp_ret_tmp==CSOCKET_NODATA)
#define OnTimeout()     else if(tcp_ret_tmp==CSOCKET_TIMEOUT)
#define OnSuccess()     else if((tcp_ret_tmp!=CSOCKET_FAIL)&&(tcp_ret_tmp!=CSOCKET_END)&&(tcp_ret_tmp!=CSOCKET_TIMEOUT))



#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
