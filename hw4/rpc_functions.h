#include "cracker.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <rpc/rpc.h>
#include <rpc/pmap_clnt.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// typedef void (*ServerCallback)(networkMessage*); 
// typedef void (*ClientCallback)(networkMessage*); 
// #include "lsp_server.h"
// #include "lsp_client.h"
// ServerCallback m_server_callback;
// ClientCallback m_client_callback;
// Intitialize
void initialize_server();

int initialize_client(char *host, CLIENT* clnt);

// RPC Functions:
int gettransient(int proto, int  vers, int* sockp);

int callback(struct svc_req *rqstp, SVCXPRT * transp);

void * callbackfn_1_svc(int *pnump,struct svc_req * req);

void docallback();

networkMessage * send_message_1_svc(networkMessage * argp,struct svc_req * req);

// SIGNAL HANDLING:
void timer_handler(int s);

int setupinterrupt();

int setupitimer();

// void send_message_to_server(const char *host,CLIENT* clnt,int progNum);
// void set_server_callback(ServerCallback callback){m_server_callback = callback;}
// void set_client_callback(ClientCallback callback){m_client_callback = callback;}
