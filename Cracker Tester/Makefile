CC = g++
CFLAGS = -g -DRPC_SVC_FG

all: client server

client: client.o cracker_clnt.o cracker_xdr.o
	$(CC) $(CFLAGS) -o client client.o cracker_clnt.o cracker_xdr.o -lnsl

server: server.o cracker_svc.o  cracker_xdr.o
	$(CC) $(CFLAGS) -o server server.o cracker_svc.o cracker_xdr.o -lnsl

cracker_svc.o: cracker.h
	$(CC) $(CFLAGS) -c cracker_svc.c

cracker_clnt.o: cracker.h
	$(CC) $(CFLAGS) -c cracker_clnt.c

cracker_xdr.o: cracker.h
	$(CC) $(CFLAGS) -c cracker_xdr.c

server.o: server.c cracker.h
	$(CC) $(CFLAGS) -c server.c

client.o: client.c cracker.h
	$(CC) $(CFLAGS) -c client.c

cracker.h: cracker.x
	rpcgen -C cracker.x

clean:
	rm -f client client.o server server.o cracker_xdr.* cracker_clnt.* cracker_svc.* cracker.h
