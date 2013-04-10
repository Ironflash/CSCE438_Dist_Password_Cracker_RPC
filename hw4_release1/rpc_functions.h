#include "cracker.h"

#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include <rpc/pmap_clnt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <memory.h>
#include <sys/signal.h>
#include <utmp.h>
#include <sys/errno.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

// typedef void (*ServerCallback)(networkMessage*); 
// typedef void (*ClientCallback)(networkMessage*); 
// #include "lsp_server.h"
// #include "lsp_client.h"
// ServerCallback m_server_callback;
// ClientCallback m_client_callback;
// Intitialize

// ********** RPC Functions ********** 
// RPC Server
void * callbackfn_1_svc(int *pnump,struct svc_req * req);

void docallback();

static void nfs_progrm_1(struct svc_req *rqstp, register SVCXPRT *transp);

char ** test_func_1_svc(networkMessage * argp,struct svc_req * req);

void initialize_server();

// RPC Client

void svc_run_mine();

void svc_run_mine_server();

networkMessage * getGlobalMessage();

int callback(struct svc_req *rqstp, SVCXPRT * transp);

int initialize_client(char *host);

void rpc_send_message_to_server(networkMessage *arg, CLIENT *cl);

//networkMessage * send_message_1_svc(networkMessage * argp,struct svc_req * req);

// SIGNAL HANDLING:
void timer_handler(int s);

int setupinterrupt();

int setupitimer();

// void send_message_to_server(const char *host,CLIENT* clnt,int progNum);
// void set_server_callback(ServerCallback callback){m_server_callback = callback;}
// void set_client_callback(ClientCallback callback){m_client_callback = callback;}
