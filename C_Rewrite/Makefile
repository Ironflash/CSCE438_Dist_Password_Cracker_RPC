CC =
CC2 = CC
TARGET = server request worker
DEBUG = -g
CFLAGS = $(DEBUG) -DRPC_SVC_FG
LFLAGS = $(DEBUG)
LIBS = #-lpthread -lssl -lcrypto

all: $(TARGET)

request: lsp_client.o network.o request.o cracker_clnt.o cracker_xdr.o
	$(CC2) $(LFLAGS) -o request request.o lsp_client.o network.o cracker_clnt.o cracker_xdr.o

worker: lsp_client.o network.o worker.o cracker_clnt.o cracker_xdr.o
	$(CC2) $(LFLAGS) -o worker worker.o lsp_client.o network.o cracker_clnt.o cracker_xdr.o -lcrypto

server: lsp_server.o network.o server.o cracker_xdr.o
	$(CC2) $(LFLAGS) -o server server.o lsp_server.o network.o cracker_xdr.o

network: network.o cracker_clnt.o cracker_svc.o cracker_xdr.o
	$(CC2) $(LFLAGS) network.o cracker_clnt.o cracker_svc.o cracker_xdr.o

request.o: request.cpp
	$(CC2) $(LFLAGS) -c request.cpp

worker.o: worker.cpp
	$(CC2) $(CFLAGS) -c worker.cpp

server.o: server.cpp
	$(CC2) $(CFLAGS) -c server.cpp

lsp_client.o: lsp_client.cpp cracker.h
	$(CC2) $(CFLAGS) -c lsp_client.cpp

lsp_server.o: lsp_server.cpp cracker.h
	$(CC2) $(CFLAGS) -c lsp_server.cpp

network.o: network.c cracker.h
	$(CC2) $(CFLAGS) -c network.c

cracker_client: cracker_client.o cracker_clnt.o cracker_xdr.o
	$(CC2) -o cracker_client cracker_client.o cracker_clnt.o cracker_xdr.o

cracker_server: cracker_server.o cracker_svc.o cracker_xdr.o
	$(CC2) -o cracker_server cracker_server.o cracker_svc.o cracker_xdr.o

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
	rm -f client client.o server server.o cracker_xdr.* cracker_clnt.* cracker_svc.* cracker.h cracker_client cracker_server
