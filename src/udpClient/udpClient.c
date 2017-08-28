#include "udpClient.h"

const char* const udpClientErrMsg[] = {
        "No error",
        "Socket create error",
        "Socket connect error",
        "Socket send error",
        "Socket recv error",
        "Select read error",

};

const char* udpClient_getErrMsg(udpClient* server)
{
    static char buf[64];

    csocket_error_lock(server); 
    sprintf(buf, "%s", udpClientErrMsg[server->userErrno]);
    if(server->sysErrno == 0)
        sprintf(buf + strlen(buf), "\n");
    else
        sprintf(buf + strlen(buf),": %s\n",strerror(server->sysErrno));
    csocket_error_lock(server);

    return buf;
}

int udpClient_getErrno(udpClient* server)
{
    return server->userErrno;
}


//******************************* setup function **********************************/

int udpClient_init(udpClient* client)
{
    int ret = udpConnect_init(client);
    client->type = CSOCKET_TYPE_UDPCLIENT;
    return ret;
}

int udpClient_close(udpClient* client)
{
    close(client->fd);
    return udpConnect_close(client);
}

int udpClient_create(udpClient* client)
{
    int ret = socket(AF_INET, SOCK_DGRAM, 0);
    if(ret == -1)
    {
        csocket_setErrno(client, UDP_CLIENT_ERR_CREATE);
        client->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }
    client->fd = ret;
    
    client->ret = CSOCKET_SUCCESS;
    return CSOCKET_SUCCESS;
}


//******************************* RW Function *************************************/

int udpClient_broadcast_setEnable(udpClient* client, bool enable)
{
    int flag = enable?1:0;
    setsockopt(client->fd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag));  
    client->ret = CSOCKET_SUCCESS;
    return CSOCKET_SUCCESS;
}

int udpClient_getDataFrom(udpClient *client, struct sockaddr_in *addr, char *data, int dataLength)
{
    int ret = csocket_udp_read(client, addr, data, dataLength);
    if(ret != CSOCKET_FAIL)
    {
        memcpy(&(client->address),addr, sizeof(struct sockaddr_in));
        client->type |= CSOCKET_TYPE_UDPCONNECT;
    }
    return ret;
}

int udpClient_getDataFrom_I2(udpClient *client, char* addrStr, unsigned short port, char *data, int dataLength)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(addrStr);
    return udpClient_getDataFrom(client,&addr, data, dataLength);
}

int udpClient_getDataFrom_timeout(udpClient *client, struct sockaddr_in *addr, char *data, int dataLength, time_t s, suseconds_t us)
{
    int ret = csocket_udp_read_timeout(client, addr, data, dataLength, s, us);
    if(ret != CSOCKET_FAIL && ret != CSOCKET_TIMEOUT)
    {
        memcpy(&(client->address),addr, sizeof(struct sockaddr_in));
        client->type |= CSOCKET_TYPE_UDPCONNECT;
    }
    return ret;
}

int udpClient_getDataFrom_I2_timeout(udpClient *client, char *addrStr, unsigned short port, char *data, int dataLength, time_t s, suseconds_t us)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(addrStr);

    return udpClient_getDataFrom_timeout(client, &addr, data, dataLength, s, us);
}



int udpClient_sendDataTo(udpClient *client, struct sockaddr_in *addr, char *data, int dataLength)
{
    return csocket_udp_write(client, addr, data, dataLength);
}

int udpClient_sendDataTo_I2(udpClient *client, char *addrStr, unsigned short port, char *data, int dataLength)
{

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(addrStr);

    return csocket_udp_write(client, &addr, data, dataLength);
}