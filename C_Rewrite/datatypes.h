// John Keech, UIN:819000713, CSCE 438 - HW2

#ifndef DATATYPES_H
#define DATATYPES_H

#define DEBUG false

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
//#include <queue>
//#include <map>
//#include <set>
#include <pthread.h>
#include <unistd.h>

#include "cracker.h"
//#include "lspmessage.pb.h" //remove protobuf


// **********************************************
// C implementation of std::queue
struct Node {
    networkMessage Data;
    struct Node* next;
} *rear, *front;

void delQueue() {
    struct Node *temp, *var=rear;
    if (var==rear) {
        rear = rear->next;
        free(var);
    }
    else
    printf("\nQueue Empty");
}

void push(int value) {
    struct Node *temp;
    temp=(struct Node *)malloc(sizeof(struct Node));
    temp->Data=value;
    if (front == NULL) {
        front=temp;
        front->next=NULL;
        rear=front;
    } else {
        front->next=temp;
        front=temp;
        front->next=NULL;
    }
}

void display() {
    struct Node *var=rear;
    if(var!=NULL) {
        printf("\nElements are as:  ");
        while(var!=NULL) {
            printf("\t%d",var->Data);
            var=var->next;
        }
    printf("\n");
    } else
        printf("\nQueue is Empty");
}

int main() {
     int i=0;
     front=NULL;
     printf(" \n1. Push to Queue");
     printf(" \n2. Pop from Queue");
     printf(" \n3. Display Data of Queue");
     printf(" \n4. Exit\n");
     while(1)
     {
          printf(" \nChoose Option: ");
          scanf("%d",&i);
          switch(i)
          {
                case 1:
                {
                     int value;
                     printf("\nEnter a valueber to push into Queue: ");
                     scanf("%d",&value);
                     push(value); 
                     display();
                     break;
                }
                case 2:
                {
                     delQueue();
                     display();
                     break;
                }
                case 3:
                {
                     display();
                     break;
                }
                case 4:
                {
                     exit(0);
                }
                default:
                {
                     printf("\nwrong choice for operation");
                }
          }
     }
}

// front()
// push()
// size()
// pop()


typedef enum {
    DISCONNECTED, CONNECT_SENT, CONNECTED
} Status;

typedef struct {
    const char              *host;
    unsigned int            port;
    // int                     fd;
    // struct sockaddr_in      *addr;
    CLIENT*                 client;
    Status                  status;
    unsigned int            id;
    unsigned int            lastSentSeq;
    unsigned int            lastReceivedSeq;
    unsigned int            lastReceivedAck;
    unsigned int            epochsSinceLastMessage;
    std::queue<networkMessage*> outbox;
} Connection;

#endif
