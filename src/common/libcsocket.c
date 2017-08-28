#include "libcsocket.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//********************************** extern ***********************************//
int* csocket_ret_p;
pthread_mutex_t csocket_ret_mutex = PTHREAD_MUTEX_INITIALIZER;

//************************** lock ***************************/

int csocket_locks_init(pcsocketBase_t ori)
{
    csocketBase* base = (csocketBase*)ori;
    int ret = 0;
    ret += pthread_mutex_init(&(base->errorLock), NULL);
    ret += pthread_mutex_init(&(base->readLock), NULL);
    ret += pthread_mutex_init(&(base->writeLock), NULL);
    ret += pthread_mutex_init(&(base->bufLock), NULL);

    if(ret != 0 )  
    {  
        return CSOCKET_FAIL;
    } 
    else
    {
        return CSOCKET_SUCCESS;
    }
}

int csocket_locks_destroy(pcsocketBase_t ori)
{
    csocketBase* base = (csocketBase*)ori;
    int ret = 0;
    ret += pthread_mutex_destroy(&(base->errorLock));
    ret += pthread_mutex_destroy(&(base->readLock));
    ret += pthread_mutex_destroy(&(base->writeLock));
    ret += pthread_mutex_destroy(&(base->bufLock));

    if(ret != 0)
    {
        return CSOCKET_FAIL;
    }
    else
    {
        return CSOCKET_SUCCESS;
    }
}

//errorLock
int csocket_error_lock(pcsocketBase_t ori)
{
    csocketBase* base = (csocketBase*)ori;
    if(pthread_mutex_lock(&(base->errorLock)) != 0)
    {
        return CSOCKET_FAIL;
    }
    else
    {
        return CSOCKET_SUCCESS;
    }
}
int csocket_error_unlock(pcsocketBase_t ori)
{
    csocketBase* base = (csocketBase*)ori;
    if(pthread_mutex_unlock(&(base->errorLock)) != 0)
    {
        return CSOCKET_FAIL;
    }
    else
    {
        return CSOCKET_SUCCESS;
    }
} 


//WriteLock
int csocket_write_lock(pcsocketBase_t ori)
{
    csocketBase* base = (csocketBase*)ori;
    if(pthread_mutex_lock(&(base->writeLock)) != 0)
    {
        return CSOCKET_FAIL;
    }
    else
    {
        return CSOCKET_SUCCESS;
    }
    
} 
int csocket_write_unlock(pcsocketBase_t ori)
{
    csocketBase* base = (csocketBase*)ori;
    if(pthread_mutex_unlock(&(base->writeLock)) != 0)
    {
        return CSOCKET_FAIL;
    }
    else
    {
        return CSOCKET_SUCCESS;
    }
}


//ReadLock
int csocket_read_lock(pcsocketBase_t ori)
{
    csocketBase* base = (csocketBase*)ori;
    if(pthread_mutex_lock(&(base->readLock)) != 0)
    {
        return CSOCKET_FAIL;
    }
    else
    {
        return CSOCKET_SUCCESS;
    }
    
} 
int csocket_read_unlock(pcsocketBase_t ori)
{
    csocketBase* base = (csocketBase*)ori;
    if(pthread_mutex_unlock(&(base->readLock)) != 0)
    {
        return CSOCKET_FAIL;
    }
    else
    {
        return CSOCKET_SUCCESS;
    }
}

//BufLock
int csocket_buf_lock(pcsocketBase_t ori)
{
    csocketBase* base = (csocketBase*)ori;
    if(pthread_mutex_lock(&(base->bufLock)) != 0)
    {
        return CSOCKET_FAIL;
    }
    else
    {
        return CSOCKET_SUCCESS;
    }
    
} 
int csocket_buf_unlock(pcsocketBase_t ori)
{
    csocketBase* base = (csocketBase*)ori;
    if(pthread_mutex_unlock(&(base->bufLock)) != 0)
    {
        return CSOCKET_FAIL;
    }
    else
    {
        return CSOCKET_SUCCESS;
    }
}

//********************** error **************************//

void csocket_setErrno(pcsocketBase_t ori, int eno)
{
    csocketBase* base = (csocketBase*)ori;
    csocket_error_lock(base); 
    base->sysErrno = errno; 
    base->userErrno = eno; 
    csocket_error_unlock(base);
}

//************************ select **********************//

int select_read(int fd, time_t sec, suseconds_t usec)
{
    fd_set fdSet; 
    struct timeval waitTime;  
    int ret = 0; 

    FD_ZERO(&fdSet);    
    FD_SET(fd, &fdSet);      
    
    waitTime.tv_sec = sec;    
    waitTime.tv_usec = usec; 

    ret = select(fd + 1, &fdSet, NULL, NULL, &waitTime);   
    if(ret < 0)//error
    {
        return -1;
    }
    else if(ret == 0)//timeout
    {
        errno = ETIMEDOUT;    
        return 0;   
    }
    else//readable
    {
        return 1;
    }
}

int select_write(int fd, time_t sec, suseconds_t usec)
{
    fd_set fdSet; 
    struct timeval waitTime;  
    int ret = 0; 

    FD_ZERO(&fdSet);    
    FD_SET(fd, &fdSet);      
    
    waitTime.tv_sec = sec;    
    waitTime.tv_usec = usec; 

    ret = select(fd + 1, NULL, &fdSet, NULL, &waitTime);   
    if(ret < 0)//error
    {
        return -1;
    }
    else if(ret == 0)//timeout
    {
        errno = ETIMEDOUT;    
        return 0;   
    }
    else//writeable
    {
        return 1;
    }
}
//************************* tcp rw ***********************//
void csocket_writeFlags_reset(pcsocketBase_t ori)
{
    csocketBase* socket = (csocketBase*)ori;
    csocket_write_lock(socket);
    socket->writeFlags = 0;
    csocket_write_unlock(socket);
}
void csocket_readFlags_reset(pcsocketBase_t ori)
{
    csocketBase* socket = (csocketBase*)ori;
    csocket_write_lock(socket);
    socket->readFlags = 0;
    csocket_write_unlock(socket);
}

int csocket_tcp_write(pcsocketBase_t ori, char* buf, int len)
{
    int ret;
    csocketBase* socket = (csocketBase*)ori;
    csocket_write_lock(socket);
    ret = send(socket->fd, buf, len, socket->writeFlags);
    csocket_write_unlock(socket);
    
    if(ret < 0)
    {
        csocket_setErrno(socket, CSOCKET_ERR_WRITE);
        socket->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }
    else if(ret == 0)
    {
        socket->ret = CSOCKET_END;
        return CSOCKET_END; 
    }
        
    socket->ret = ret;
    return ret;
}

int csocket_tcp_write_timeout(pcsocketBase_t ori, char* buf, int len, time_t s, suseconds_t us)
{
    int ret;
    csocketBase* socket = (csocketBase*)ori;
    csocket_write_lock(socket);
    ret = select_write(socket->fd, s, us);

    if(ret < 0)//error
    {
        csocket_write_unlock(socket);
        csocket_setErrno(socket, CSOCKET_ERR_SELECT_WRITE);
        socket->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }
    else if(ret == 0)//timeout
    {
        csocket_write_unlock(socket);
        socket->ret = CSOCKET_TIMEOUT;  
        return CSOCKET_TIMEOUT;   
    }
    else//readable
    {
        ret = send(socket->fd, buf, len, socket->writeFlags);
        csocket_write_unlock(socket);
        
        if(ret < 0)
        {
            csocket_setErrno(socket, CSOCKET_ERR_WRITE);
            socket->ret = CSOCKET_FAIL;
            return CSOCKET_FAIL;
        }
        else if(ret == 0)
        {
            socket->ret = CSOCKET_END;
            return CSOCKET_END; 
        }
            
        socket->ret = ret;
        return ret;
    }
}


int csocket_tcp_read(pcsocketBase_t ori, char* buf, int len)
{
    int ret;
    csocketBase* socket = (csocketBase*)ori;
    csocket_read_lock(socket);
    ret = recv(socket->fd, buf, len, socket->readFlags);
    csocket_read_unlock(socket);
    
    if(ret < 0)
    {
        csocket_setErrno(socket, CSOCKET_ERR_READ);
        socket->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }
    else if(ret == 0)
    {
        socket->ret = CSOCKET_END;
        return CSOCKET_END; 
    }
        
    socket->ret = ret;
    return ret;
}

int csocket_tcp_read_timeout(pcsocketBase_t ori, char* buf, int len, time_t s, suseconds_t us)
{
    int ret;
    csocketBase* socket = (csocketBase*)ori;
    csocket_read_lock(socket);
    ret = select_read(socket->fd, s, us);

    if(ret < 0)//error
    {
        csocket_read_unlock(socket);
        csocket_setErrno(socket, CSOCKET_ERR_SELECT_READ);
        socket->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }
    else if(ret == 0)//timeout
    {
        csocket_read_unlock(socket);
        socket->ret = CSOCKET_TIMEOUT;  
        return CSOCKET_TIMEOUT;   
    }
    else//readable
    {
        ret = recv(socket->fd, buf, len, socket->readFlags);
        csocket_read_unlock(socket);
        
        if(ret < 0)
        {
            csocket_setErrno(socket, CSOCKET_ERR_READ);
            socket->ret = CSOCKET_FAIL;
            return CSOCKET_FAIL;
        }
        else if(ret == 0)
        {
            socket->ret = CSOCKET_END;
            return CSOCKET_END; 
        }
            
        socket->ret = ret;
        return ret;
    }
    
}
//************************* udp rw ***********************//
int csocket_udp_write(pcsocketBase_t ori, struct sockaddr_in* address, char* buf, int len)
{
    int ret;
    csocketBase* socket = (csocketBase*)ori;
    csocket_write_lock(socket);
    ret = sendto(socket->fd, buf, len, socket->writeFlags, (struct sockaddr *)address, sizeof(struct sockaddr_in));
    csocket_write_unlock(socket);
    
    if(ret == -1)
    {
        csocket_setErrno(socket, CSOCKET_ERR_WRITE);
        socket->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }
    socket->ret = ret;
    return ret;
}

int csocket_udp_write_timeout(pcsocketBase_t ori, struct sockaddr_in* address, char* buf, int len, time_t s, suseconds_t us)
{
    int ret;
    csocketBase* socket = (csocketBase*)ori;
    csocket_write_lock(socket);
    ret = select_write(socket->fd, s, us);

    if(ret < 0)//error
    {
        csocket_write_unlock(socket);
        csocket_setErrno(socket, CSOCKET_ERR_SELECT_WRITE);
        socket->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }
    else if(ret == 0)//timeout
    {
        csocket_write_unlock(socket);
        socket->ret = CSOCKET_TIMEOUT;  
        return CSOCKET_TIMEOUT;   
    }
    else//readable
    {
        ret = sendto(socket->fd, buf, len, socket->writeFlags, (struct sockaddr *)address, sizeof(struct sockaddr_in));
        csocket_write_unlock(socket);
        
        if(ret == -1)
        {
            csocket_setErrno(socket, CSOCKET_ERR_WRITE);
            socket->ret = CSOCKET_FAIL;
            return CSOCKET_FAIL;
        }
        socket->ret = ret;
        return ret;
    }
}

int csocket_udp_read(pcsocketBase_t ori, struct sockaddr_in* address, char* buf, int len)
{
    int ret;
    csocketBase* socket = (csocketBase*)ori;
    socklen_t addressLen = sizeof(struct sockaddr_in);
    csocket_read_lock(socket);
    ret = recvfrom(socket->fd, buf, len, socket->writeFlags, (struct sockaddr *)address, &addressLen);
    csocket_read_unlock(socket);
    
    if(ret == -1)
    {
        csocket_setErrno(socket, CSOCKET_ERR_READ);
        socket->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }
    socket->ret = ret;
    return ret;
}

int csocket_udp_read_timeout(pcsocketBase_t ori, struct sockaddr_in* address, char* buf, int len, time_t s, suseconds_t us)
{
    int ret;
    socklen_t addressLen = sizeof(struct sockaddr_in);
    csocketBase* socket = (csocketBase*)ori;
    csocket_read_lock(socket);
    ret = select_read(socket->fd, s, us);

    if(ret < 0)//error
    {
        csocket_read_unlock(socket);
        csocket_setErrno(socket, CSOCKET_ERR_SELECT_READ);
        socket->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }
    else if(ret == 0)//timeout
    {
        csocket_read_unlock(socket);
        socket->ret = CSOCKET_TIMEOUT;  
        return CSOCKET_TIMEOUT;   
    }
    else//readable
    {
        ret = recvfrom(socket->fd, buf, len, socket->writeFlags, (struct sockaddr *)address, &addressLen);
        csocket_read_unlock(socket);
        
        if(ret == -1)
        {
            csocket_setErrno(socket, CSOCKET_ERR_READ);
            socket->ret = CSOCKET_FAIL;
            return CSOCKET_FAIL;
        }
        socket->ret = ret;
        return ret;
    }
}

//********************** address **********************//

void getIpFromAddress(struct sockaddr_in* in, char* buf)
{
    static pthread_mutex_t inet_ntoa_mutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_lock(&inet_ntoa_mutex);
    strncpy(buf, inet_ntoa(in->sin_addr), 15);
    pthread_mutex_unlock(&inet_ntoa_mutex);

}

unsigned short getPortFromAddress(struct sockaddr_in* in)
{
    return ntohs(in->sin_port);
}

//******************** Other Option **********************/

int setSocketReuse(int socket, bool bReuseaddr)
{
    return setsockopt(socket, SOL_SOCKET , SO_REUSEADDR,(const char*)&bReuseaddr, sizeof(bool));
}

int setSocketSendTimeout(int socket, int ms)
{
    return setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&ms, sizeof(int));
}

int setSocketRecvTimeout(int socket, int ms)
{
    return setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&ms, sizeof(int));
}

int setSocketSendBufSize(int socket, int b)
{
    return setsockopt(socket, SOL_SOCKET, SO_SNDBUF,(const char*)&b, sizeof(int));
}

int setSocketRecvBufSize(int socket, int b)
{
    return setsockopt(socket, SOL_SOCKET, SO_RCVBUF,(const char*)&b, sizeof(int));
}

//******************* keepAlive *************************/
// 是否开启keepalive属性
int setSocketKeepAlive(int socket, bool keepAlive)
{
    int k = keepAlive?1:0;
    return setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, (void *)&k, sizeof(k));
}

// 如该连接在keepIdle秒内没有任何数据往来,则进行探测
int setSocketKeepIdle(int socket, int keepIdle)
{
    return setsockopt(socket, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle));
}

// 探测时发包的时间间隔为keepInterval秒
int setSocketKeepInterval(int socket, int keepInterval)
{
    return setsockopt(socket, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval));
}

// 探测尝试的次数.如果第1次探测包就收到响应了,则后不再发.
int setSocketKeepCount(int socket, int keepCount)
{
    return setsockopt(socket, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount));
}




#ifdef __cplusplus
}
#endif /* __cplusplus */

