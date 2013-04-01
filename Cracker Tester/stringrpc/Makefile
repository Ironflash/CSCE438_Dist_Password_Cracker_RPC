CC = gcc
CFLAGS = -g -DRPC_SVC_FG

all: client server

client: client.o strings_clnt.o strings_xdr.o
	$(CC) $(CFLAGS) -o client client.o strings_clnt.o strings_xdr.o -lnsl

server: server.o strings_svc.o  strings_xdr.o
	$(CC) $(CFLAGS) -o server server.o strings_svc.o strings_xdr.o -lnsl

strings_svc.o: strings.h
	$(CC) $(CFLAGS) -c strings_svc.c

strings_clnt.o: strings.h
	$(CC) $(CFLAGS) -c strings_clnt.c

strings_xdr.o: strings.h
	$(CC) $(CFLAGS) -c strings_xdr.c

server.o: server.c strings.h
	$(CC) $(CFLAGS) -c server.c

client.o: client.c strings.h
	$(CC) $(CFLAGS) -c client.c

strings.h: strings.x
	rpcgen -C strings.x

clean:
	rm -f client client.o server server.o strings_xdr.* strings_clnt.* strings_svc.* strings.h
