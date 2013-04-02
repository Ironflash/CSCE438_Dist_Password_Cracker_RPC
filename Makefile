CC = g++
TARGET = server request worker
DEBUG = #-g
CFLAGS = -Wall -c $(DEBUG) -g -DRPC_SVC_FG
LFLAGS = $(DEBUG)
LIBS = -lpthread -lssl -lcrypto

all: $(TARGET)

request: lsp_client.o network.o
	$(CC) $(LFLAGS) -o $@ $@.cpp $^ $(LIBS) -lnsl

worker: lsp_client.o network.o
	$(CC) $(LFLAGS) -o $@ $@.cpp $^ $(LIBS) -lnsl

server: lsp_server.o network.o
	$(CC) $(LFLAGS) -o $@ $@.cpp $^ $(LIBS) -lnsl
    
%.o: %.cpp
	$(CC) $(CFLAGS) $<

cracker_svc.o: cracker.h
	$(CC) $(CFLAGS) -c cracker_svc.c

cracker_clnt.o: cracker.h
	$(CC) $(CFLAGS) -c cracker_clnt.c

cracker_xdr.o: cracker.h
	$(CC) $(CFLAGS) -c cracker_xdr.c

cracker.h: cracker.x
	rpcgen -a -C cracker.x

clean:
	rm -f *.o 
	rm -f $(TARGET)
	rm -f client client.o server server.o cracker_xdr.* cracker_clnt.* cracker_svc.* cracker.h cracker_client.c cracker_server.c
