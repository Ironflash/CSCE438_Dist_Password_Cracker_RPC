#pragma once
#include "lsp.h"

typedef struct {
    unsigned int            port;
    unsigned int            nextConnID;
    bool                    running;
    Connection              *connection;
    std::map<unsigned int,Connection*> clients;
    std::set<std::string>   connections;
    std::queue<networkMessage*> inbox;
    pthread_mutex_t         mutex;
    pthread_t               readThread;
    pthread_t               writeThread;
    pthread_t               epochThread; 
} lsp_server;

static lsp_server* m_server;
// API Methods
lsp_server* lsp_server_create();
int  lsp_server_read(lsp_server* a_srv, void* pld, unsigned int* conn_id);
bool lsp_server_write(lsp_server* a_srv, void* pld, int lth, unsigned int conn_id);
bool lsp_server_close(lsp_server* a_srv, uint32_t conn_id);

// Internal Methods
void* ServerEpochThread(void *params);
void* ServerReadThread(void *params); //RPC handles reads by getting the return on a write

void* ServerWriteThread(void *params);
void cleanup_connection(Connection *s);

void set_server(lsp_server* server);
void send_message_to_server(networkMessage* msg);

