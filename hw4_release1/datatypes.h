// John Keech, UIN:819000713, CSCE 438 - HW2
#pragma once
#ifndef DATATYPES_H
#define DATATYPES_H

#define DEBUG false

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <queue>
#include <map>
#include <set>
#include <pthread.h>
#include <unistd.h>

#include "cracker.h"
//#include "lspmessage.pb.h" //remove protobuf

typedef enum {
    DISCONNECTED, CONNECT_SENT, CONNECTED
} Status;

typedef struct {
    const char              *host;
    unsigned int            port;
    int                     fd;
    struct sockaddr_in      *addr;
    CLIENT*                 client;
    Status                  status;
    unsigned int            id;
    unsigned int            lastSentSeq;
    unsigned int            lastReceivedSeq;
    unsigned int            lastReceivedAck;
    unsigned int            epochsSinceLastMessage;
    int                     progNum;
    std::queue<networkMessage*> outbox;
} Connection;

#endif
