#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned int connid;
    unsigned int seqnum;
    char* payload;
} networkMessage;

// **********************************************
// C implementation of std::queue
struct Node {
    networkMessage* Data;
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

void push(networkMessage* value) {
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
        printf("\nElements are as: ");
        while(var!=NULL) {
            printf("\t%s",var->Data->payload);
            var=var->next;
        }
    printf("\n");
    } else
        printf("\nQueue is Empty");
}

int main() {
    int i=0;
    front=NULL;

    for (int i=0; i<10; i++) {
        networkMessage *value;
        value=(networkMessage *)malloc(sizeof(networkMessage));
        value->connid = 0;
        value->seqnum = 0;
        value->payload = "hello"+i;
        push(value);
    }
    display();

    /*
    printf(" \n1. Push to Queue");
    printf(" \n2. Pop from Queue");
    printf(" \n3. Display Data of Queue");
    printf(" \n4. Exit\n");

    int boolean = 0;
    while(boolean == 0) {
        printf(" \nChoose Option: ");
        scanf("%d",&i);
        switch(i) {
            case 1:
            {
                networkMessage *value;
                value=(networkMessage *)malloc(sizeof(networkMessage));
                value->connid = 0;
                value->seqnum = 0;
                //value.payload = "hello";
                printf("\nEnter a string to push into Queue: ");
                char str[64];
                scanf("%s",str);
                value->payload = str;
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
                boolean = 1;
                break;
            }
            default:
            {
                printf("\nwrong choice for operation");
            }
        }
    }
    //*/

    printf("front = %s\n", front->Data->payload);
    printf("rear = %s\n", rear->Data->payload);
}

