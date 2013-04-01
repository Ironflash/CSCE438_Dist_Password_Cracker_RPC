CC = g++
TARGET = server request worker
DEBUG = #-g
CFLAGS = -Wall -c $(DEBUG)
RFLAGS = -g -DRPC_SVC_FG
#LFLAGS = $(DEBUG)
LFLAGS = $(DEBUG) -DRPC_SVC_FG
#LIBS = -lprotobuf -lpthread -lssl -lcrypto
LIBS = -lpthread -lssl -lcrypto

all: pre-build $(TARGET)

pre-build:
	protoc --cpp_out=. lspmessage.proto
    
request: lsp_client.o lspmessage.pb.o network.o
	$(CC) $(LFLAGS) -o $@ $@.cpp $^ $(LIBS)

worker: lsp_client.o lspmessage.pb.o network.o
	$(CC) $(LFLAGS) -o $@ $@.cpp $^ $(LIBS)

server: lsp_server.o network.o cracker_svc.o  cracker_xdr.o
	$(CC) -o $@ $@.cpp $^ $(LIBS)

#server: server.o strings_svc.o  strings_xdr.o
#	$(CC) -o server server.o strings_svc.o strings_xdr.o

cracker_svc.o: cracker.h
	$(CC) $(RFLAGS) -c cracker_svc.c

cracker_clnt.o: cracker.h
	$(CC) $(RFLAGS) -c cracker_clnt.c

cracker_xdr.o: cracker_xdr.c 
	$(CC) $(RFLAGS) -c cracker_xdr.c

cracker.h: cracker.x
	rpcgen -C cracker.x

#lspmessage.pb.o: lspmessage.pb.cc
#	$(CC) $(CFLAGS) $<
    
%.o: %.cpp
	$(CC) $(CFLAGS) $<

clean:
	rm -f *.o 
	rm -f lspmessage.pb.h lspmessage.pb.cc
	rm -f $(TARGET)
	rm -f *~
	rm -f cracker_xdr.* cracker_clnt.* cracker_svc.* cracker.h

