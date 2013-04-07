CC = cc
CC2 = g++
TARGET = server request worker
DEBUG = -g
CFLAGS = $(DEBUG) -DRPC_SVC_FG
LFLAGS = $(DEBUG)
LIBS = #-lpthread -lssl -lcrypto

all: $(TARGET)

request: lsp_client.o network.o request.o cracker_clnt.o cracker_xdr.o
	$(CC2) $(CFLAGS) -o request request.o lsp_client.o network.o cracker_clnt.o cracker_xdr.o -lpthread

worker: lsp_client.o network.o worker.o cracker_clnt.o cracker_xdr.o
	$(CC2) $(CFLAGS) -o worker worker.o lsp_client.o network.o cracker_clnt.o cracker_xdr.o -lcrypto

server: lsp_server.o network.o server.o cracker_xdr.o
	$(CC2) $(CFLAGS) -o server server.o lsp_server.o network.o cracker_xdr.o

network: network.o cracker_clnt.o cracker_svc.o cracker_xdr.o
	$(CC2) $(CFLAGS) network.o cracker_clnt.o cracker_svc.o cracker_xdr.o

request2: rpc_functions.o request.o cracker_clnt.o cracker_xdr.o
	$(CC2) $(CFLAGS) -o request request.o rpc_functions.o cracker_clnt.o cracker_xdr.o

server2: network.o rpc_functions.o server.o cracker_clnt.o cracker_xdr.o
	$(CC2) $(CFLAGS) -o server server.o rpc_functions.o cracker_clnt.o cracker_xdr.o

rpc_functions.o: rpc_functions.c cracker.h
	$(CC) $(CFLAGS) -c rpc_functions.c

request.o: request.cpp
	$(CC2) $(CFLAGS) -c request.cpp

worker.o: worker.cpp
	$(CC2) $(CFLAGS) -c worker.cpp

server.o: server.cpp
	$(CC2) $(CFLAGS) -c server.cpp

lsp_client.o: lsp_client.cpp cracker.h
	$(CC2) $(CFLAGS) -c lsp_client.cpp

lsp_server.o: lsp_server.cpp cracker.h
	$(CC2) $(CFLAGS) -c lsp_server.cpp

network.o: network.cpp cracker.h
	$(CC2) $(CFLAGS) -c network.cpp

cracker_svc.o: cracker.h
	$(CC) $(CFLAGS) -c cracker_svc.c

cracker_clnt.o: cracker.h
	$(CC) $(CFLAGS) -c cracker_clnt.c

cracker_xdr.o: cracker.h
	$(CC) $(CFLAGS) -c cracker_xdr.c

cracker.h: cracker.x
	rpcgen -C cracker.x

clean:
	rm -f *.o 
	rm -f $(TARGET)
	rm -f client client.o server server.o cracker_xdr.* cracker_clnt.* cracker_svc.* cracker.h cracker_client cracker_server

