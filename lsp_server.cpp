#include "lsp_server.h"

double epoch_delay = _EPOCH_LTH; // number of seconds between epochs
unsigned int num_epochs = _EPOCH_CNT; // number of epochs that are allowed to pass before a connection is terminated

/*
 *
 *
 *				LSP RELATED FUNCTIONS
 *
 *
 */  

// Set length of epoch (in seconds)
void lsp_set_epoch_lth(double lth){
    if(lth > 0)
        epoch_delay = lth;
}

// Set number of epochs before timing out
void lsp_set_epoch_cnt(int cnt){
    if(cnt > 0)
        num_epochs = cnt;
}

// Set fraction of packets that get dropped along each connection
void lsp_set_drop_rate(double rate){
    network_set_drop_rate(rate);
}

/*
 *
 *
 *				SERVER RELATED FUNCTIONS
 *
 *
 */

 // Create a server listening on a specified port.
// Returns NULL if server cannot be created
lsp_server* lsp_server_create(int port){
    // initialize the server data structure
    lsp_server *server = new lsp_server();

    server->port = port;
    server->nextConnID = 1;
    server->running = true;
    server->connection = network_setup_server(port);

    if(!server->connection){
        // the server could not be bound to the specified port
        delete server;
        return NULL;
    }
    
    // initialize the mutex
    pthread_mutex_init(&(server->mutex),NULL);
    
    // create the epoch thread
    int res;
    if((res = pthread_create(&(server->epochThread), NULL, ServerEpochThread, (void*)server)) != 0){
        printf("Error: Failed to start the epoch thread: %d\n",res);
        lsp_server_close(server,0);
        return NULL;
    }
        
    // create the read/write threads listening on a certain port
    
    // if((res = pthread_create(&(server->writeThread), NULL, ServerWriteThread, (void*)server)) != 0){
    //     printf("Error: Failed to start the write thread: %d\n",res);
    //     lsp_server_close(server,0);
    //     return NULL;
    // }

    return server;
}

// Server Write. Should not send NULL
bool lsp_server_write(lsp_server* a_srv, void* pld, int lth, uint32_t conn_id){
    if(pld == NULL || lth == 0)
        return false; // don't send bad messages
    
    pthread_mutex_lock(&(a_srv->mutex));
    Connection *conn = a_srv->clients[conn_id];
    conn->lastSentSeq++;
    if(DEBUG) printf("Server queueing msg %d for conn %d for write\n",conn->lastSentSeq,conn->id);
    
    // build the message object
    networkMessage *msg = network_build_message(conn->id,conn->lastSentSeq,(char*)pld,lth);
    
    // queue it up for writing
    conn->outbox.push(msg);
    pthread_mutex_unlock(&(a_srv->mutex));
    
    return true;
}

// Close connection.
bool lsp_server_close(lsp_server* a_srv, uint32_t conn_id){
    if(conn_id == 0){
        // close all connections
        if(DEBUG) printf("Shutting down the server\n");
        
        for(std::map<unsigned int,Connection*>::iterator it = a_srv->clients.begin();
            it != a_srv->clients.end();
            ++it){
            Connection *conn = it->second;
            cleanup_connection(conn);
        }
        a_srv->clients.clear();
        a_srv->connections.clear();
        a_srv->running = false;
        
        // wait for the threads to terminate
        void *status;
        if(a_srv->readThread)
            pthread_join(a_srv->readThread,&status);
        if(a_srv->writeThread)
            pthread_join(a_srv->writeThread,&status);
        if(a_srv->epochThread)
            pthread_join(a_srv->epochThread,&status);
            
        pthread_mutex_destroy(&(a_srv->mutex));
        cleanup_connection(a_srv->connection);
        delete a_srv;     
    } else {
        // close one connection
        if(DEBUG) printf("Shutting down client %d\n",conn_id);
        Connection *conn = a_srv->clients[conn_id];
        //delete conn->addr; //remove socket related items
        delete conn;        
        a_srv->clients.erase(conn_id);
    }
    return false;
}

/* Internal Methods */

void* ServerEpochThread(void *params){
    lsp_server *server = (lsp_server*)params;
    
    while(true){
        usleep(epoch_delay * 1000000); // convert seconds to microseconds
        if(DEBUG) printf("Server epoch handler waking up\n");
        
        // epoch is happening; send required messages
        pthread_mutex_lock(&(server->mutex));
        if(!server->running)
            break;
            
        // iterate over all of the connections and apply the Epoch rules for each one
        for(std::map<unsigned int,Connection*>::iterator it=server->clients.begin();
            it != server->clients.end();
            ++it){
           
            Connection *conn = it->second;
            
            if(conn->status == DISCONNECTED)
                continue;
            
             // send ACK for most recent message
            if(DEBUG) printf("Server acknowledging last received message %d for conn %d\n",conn->lastReceivedSeq,conn->id);
            network_acknowledge(conn);
            
            // resend the first message in the outbox, if any
            if(conn->outbox.size() > 0) {
                if(DEBUG) printf("Server resending msg %d for conn %d\n",conn->outbox.front()->seqnum,conn->id);
                network_send_message(conn,conn->outbox.front());
            }
            
            if(++(conn->epochsSinceLastMessage) >= num_epochs){
                // oops, we haven't heard from the client in a while;
                // mark the connection as disconnected
                if(DEBUG) printf("Too many epochs have passed since we heard from client %d... disconnecting\n",conn->id);
                conn->status = DISCONNECTED;
                
                // place a "disconnected" message in the queue to notify the client
                server->inbox.push(network_build_message(conn->id,0,NULL,0));
            }
        }
            
        pthread_mutex_unlock(&(server->mutex));
    }
    pthread_mutex_unlock(&(server->mutex));
    if(DEBUG) printf("Epoch Thread exiting\n");
    return NULL;
}

void cleanup_connection(Connection *s){
    if(!s)
        return;

    // close the file descriptor and free memory
    /*
    if(s->fd != -1)
        close(s->fd);
    delete s->addr;
    //*/
    delete s;
}

