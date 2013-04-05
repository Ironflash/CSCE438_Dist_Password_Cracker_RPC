 #CC = gcc
CFLAGS = -g -DRPC_SVC_FG

all: client server

client: client.o rpc_clnt.o rpc_xdr.o
	$(CC) $(CFLAGS) -o client client.o rpc_clnt.o rpc_xdr.o #-lnsl

server: server.o rpc_xdr.o
	$(CC) $(CFLAGS) -o server server.o rpc_xdr.o #-lnsl

rpc_svc.o: rpc.h
	$(CC) $(CFLAGS) -c rpc_svc.c

rpc_clnt.o: rpc.h
	$(CC) $(CFLAGS) -c rpc_clnt.c

rpc_xdr.o: rpc.h
	$(CC) $(CFLAGS) -c rpc_xdr.c

server.o: server.c rpc.h
	$(CC) $(CFLAGS) -c server.c

client.o: client.c rpc.h
	$(CC) $(CFLAGS) -c client.c

rpc.h: rpc.x
	rpcgen -C rpc.x

clean:
	rm -f client client.o server server.o rpc_xdr.* rpc_clnt.* rpc_svc.* rpc.h
