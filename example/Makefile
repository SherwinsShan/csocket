TCPSERVER_TEST=tcpServerTest
TCPCLIENT_TEST=tcpClientTest
UDPSERVER_TEST=udpServerTest
UDPCLIENT_TEST=udpClientTest
BROADCASTSERVER_TEST=broadcastServerTest
BROADCASTClient_TEST=broadcastClientTest

LIB=-L$(LIBPATH)/$(LIBARCH)/ -lcsocket
INCLUDE=-I$(INCLUDEPATH)
CFLAGS=

all: $(TCPSERVER_TEST) $(TCPCLIENT_TEST) \
	$(UDPSERVER_TEST) $(UDPCLIENT_TEST) \
	$(BROADCASTSERVER_TEST) $(BROADCASTClient_TEST)


$(TCPSERVER_TEST) : tcpServer/tcpServerTest.c
	@$(CC) -o $@ $^ $(INCLUDE) $(LIB) -lpthread 

$(TCPCLIENT_TEST) : tcpClient/tcpClientTest.c
	@$(CC) -o $@ $^ $(INCLUDE) $(LIB) -lpthread 

$(UDPSERVER_TEST) : udpServer/udpServerTest.c
	@$(CC) -o $@ $^ $(INCLUDE) $(LIB) -lpthread 

$(UDPCLIENT_TEST) : udpClient/udpClientTest.c
	@$(CC) -o $@ $^ $(INCLUDE) $(LIB) -lpthread 

$(BROADCASTSERVER_TEST) : udpBroadcastServer/broadcastServerTest.c
	@$(CC) -o $@ $^ $(INCLUDE) $(LIB) -lpthread 

$(BROADCASTClient_TEST) : udpBroadcastClient/broadcastClientTest.c
	@$(CC) -o $@ $^ $(INCLUDE) $(LIB) -lpthread 

.PHONY: clean

clean:
	-@rm -f $(TCPSERVER_TEST)* $(TCPCLIENT_TEST)* \
	$(UDPSERVER_TEST)* $(UDPCLIENT_TEST)* \
	$(BROADCASTSERVER_TEST)* $(BROADCASTClient_TEST)*


