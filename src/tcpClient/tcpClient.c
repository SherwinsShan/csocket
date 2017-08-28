#include "tcpClient.h"



//************************************* ERROR LOG *********************************/
static const char* const tcpClientErrMsg[] = {
        "No error",
        "Client create error",
        "Client connect error"
};


const char* tcpClient_getErrMsg(tcpClient* client)
{
    static char buf[64];

    csocket_error_lock(client); 
    sprintf(buf, "%s", tcpClientErrMsg[client->userErrno]);
    if(client->sysErrno == 0)
        sprintf(buf + strlen(buf), "\n");
    else
        sprintf(buf + strlen(buf),": %s\n",strerror(client->sysErrno));
    csocket_error_unlock(client);

    return buf;
}

int tcpClient_getErrno(tcpClient* client)
{
    return client->userErrno;
}
//******************************************************************************/
int tcpClient_init(tcpClient* client)
{
    int ret = tcpConnect_init((tcpConnect*)client);
    client->type = CSOCKET_TYPE_TCPCLIENT;
    return ret;
}

int tcpClient_close(tcpClient* client)
{
    return tcpConnect_close((tcpConnect*)client);
}


int tcpClient_create(tcpClient* client)
{
    int ret = socket(AF_INET, SOCK_STREAM, 0);
    if(ret == -1)
    {
        csocket_setErrno(client, TCP_CLIENT_ERR_CREATE);
        client->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }
    client->fd = ret;

    client->ret = CSOCKET_SUCCESS;
    return CSOCKET_SUCCESS;
}

int tcpClient_connect(tcpClient* client, struct sockaddr_in* addr_ser)
{
    
    int ret;
    memcpy(&(client->address), addr_ser, sizeof(struct sockaddr_in));

    ret = connect(client->fd, (struct sockaddr *)&(client->address), sizeof(struct sockaddr_in));  
    if(ret == -1)
    {
        csocket_setErrno(client, TCP_CLIENT_ERR_CONNECT);
        client->ret = CSOCKET_FAIL;
        return CSOCKET_FAIL;
    }
    client->ret = CSOCKET_SUCCESS;
    return CSOCKET_SUCCESS;
}


int tcpClient_connect_I2(tcpClient* client, const char *addr, unsigned short port)
{
    struct sockaddr_in addr_ser;  
    memset(&addr_ser, 0, sizeof(struct sockaddr_in));
    addr_ser.sin_family      = AF_INET;  
    addr_ser.sin_addr.s_addr = inet_addr(addr);  
    addr_ser.sin_port        = htons(port);  

    return tcpClient_connect(client, &addr_ser);
}

tcpConnect* tcpClient_getConnect(tcpClient *client)
{
    client->type |= CSOCKET_TYPE_TCPCONNECT;
    return (tcpConnect*)client;
}
