#include "lsp_server.h"

double epoch_delay = _EPOCH_LTH; // number of seconds between epochs
unsigned int num_epochs = _EPOCH_CNT; // number of epochs that are allowed to pass before a connection is terminated
 lsp_server *server;

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
lsp_server* lsp_server_create(){
    // initialize the server data structure
    server = new lsp_server();

    server->nextConnID = 1;
    server->running = true;
    
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
    /*if((res = pthread_create(&(server->readThread), NULL, ServerReadThread, (void*)server)) != 0){
        printf("Error: Failed to start the epoch thread: %d\n",res);
        lsp_server_close(server,0);
        return NULL;
    } */
    if((res = pthread_create(&(server->writeThread), NULL, ServerWriteThread, (void*)server)) != 0){
	printf("Error: Failed to start the write thread: %d\n",res);
	lsp_server_close(server,0);
        return NULL;
    }
    //save the server for later
    m_server = server;
    return server;
}

// Read from connection. Return NULL when connection lost
// Returns number of bytes read. conn_id is an output parameter
int lsp_server_read(lsp_server* a_srv, void* pld, unsigned int* conn_id){
    // block until a message arrives or the client becomes disconnected
    while(true){
        pthread_mutex_lock(&(a_srv->mutex));
        bool running = a_srv->running;
        networkMessage *msg = NULL;
        if(running) {
            // try to pop a message from the inbox queue
            if(a_srv->inbox.size() > 0){
                msg = a_srv->inbox.front();
                a_srv->inbox.pop();
            }
        }
        pthread_mutex_unlock(&(a_srv->mutex));
        if(!running)
            break;
        if(msg){
            // we got a message, return it
            std::string payload = msg->payload;
            *conn_id = msg->connid;
            delete msg;
            //memcpy(pld,payload.c_str(),strlen(pld)+1);
            return payload.size();
        }
        
        // still connected, but no message has arrived...
        // sleep for a bit
        usleep(10000); // 10 ms = 10,0000 microseconds
    }
    *conn_id = 0; // all clients are disconnected
    return 0; // NULL, no bytes read (all clients disconnected)
}

// Server Write. Should not send NULL
bool lsp_server_write(lsp_server* a_srv, void* pld, int lth, unsigned int conn_id){
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
                network_send_message_from_server(conn,conn->outbox.front());
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

int initialized = 0;

/*networkMessage *
send_message_1_svc(networkMessage *argp, struct svc_req *rqstp)
{
    //Initialization:
    if (!initialized) {
        initialized = 1;
    }

    static networkMessage result;

    result.connid = 0;
    result.seqnum = 0;
    //char temp_response[] = "f test";
    result.payload = "f test";

    
    printf("incoming string: \"%s\"\n", argp->payload);
    printf("incoming connid: %d\n", argp->connid);
    printf("incoming seqnum: %d\n", argp->seqnum);
    printf("returning string: %s\n", result.payload);

    return(&result);
}*/
networkMessage *
send_message_1_svc(networkMessage *msg, struct svc_req *rqstp)
{
//void* ServerReadThread(void *params){
    // continously attempt to read messages from the socket. When one arrives, parse it
    // and take the appropriate action
    //Initialization:
    if (!initialized) {
	lsp_server_create();
        initialized = 1;
    }
        pthread_mutex_lock(&(server->mutex));
        if(!server->running)
            return NULL;
        pthread_mutex_unlock(&(server->mutex));
        if(msg) {
            // we got a message, let's parse it
            pthread_mutex_lock(&(server->mutex));
            if(msg->connid == 0 && msg->seqnum == 0 && strlen(msg->payload) == 0){
                // connection request, if first time, make the connection
                //sprintf(host,"%s:%d",inet_ntoa(addr.sin_addr),addr.sin_port);
                //if(server->connections.count(host) == 0){
                    // this is the first time we've seen this host, add it to the server's list of seen hosts
                    //server->connections.insert(host);
                    
                    if(DEBUG) printf("Connection request received from client\n");
                    
                    // build up the new connection object
                    Connection *conn = new Connection();
                    conn->status = CONNECTED;
                    conn->id = server->nextConnID;
                    server->nextConnID++;
                    conn->lastSentSeq = 0;
                    conn->lastReceivedSeq = 0;
                    conn->epochsSinceLastMessage = 0;
                    
		     // send an ack for the connection request
		    if(DEBUG) printf("Sending back connection ack with id %i\n", conn->id);

		    msg->connid = conn->id;
                    return msg;
                    
                    // insert this connection into the list of connections
                    server->clients.insert(std::pair<int,Connection*>(conn->id,conn));
                //}
            } else {
                if(server->clients.count(msg->connid) == 0){
                    printf("Bogus connection id received: %d, skipping message...\n",msg->connid);
                } else {
                    Connection *conn = server->clients[msg->connid];
                
                    // reset counter for epochs since we have received a message
                    conn->epochsSinceLastMessage = 0;
                
                    if(strlen(msg->payload)== 0){
                        // we received an ACK
                        if(DEBUG) printf("Server received an ACK for conn %d msg %d\n",msg->connid,msg->seqnum);
                        if(msg->seqnum == (conn->lastReceivedAck + 1))
                            conn->lastReceivedAck = msg->seqnum;
                        if(conn->outbox.size() > 0 && msg->seqnum == conn->outbox.front()->seqnum) {
                            delete conn->outbox.front();
                            conn->outbox.pop();
                        }
                    } else {
                        // data packet
                        if(DEBUG) printf("Server received msg %d for conn %d\n",msg->seqnum,msg->connid);
                        if(msg->seqnum == (conn->lastReceivedSeq + 1)){
                            // next in the list
                            conn->lastReceivedSeq++;
                            server->inbox.push(msg);
                        
                            // send ack for this message
                            msg->connid = conn->id;
			    msg->seqnum = conn->lastReceivedSeq;
			    msg->payload = "";
			    return msg;
                        }
                    }
                }
            }
            pthread_mutex_unlock(&(server->mutex));
        }
    pthread_mutex_unlock(&(server->mutex));
    if(DEBUG) printf("Read Thread exiting\n");
    return NULL;
}

// this write thread will ensure that messages can be sent/received faster than only
// on epoch boundaries. It will continuously poll for messages that are eligible to
// bet sent for the first time, and then send them out.
void* ServerWriteThread(void *params){
    lsp_server *server = (lsp_server*)params;
    
    // continuously poll for new messages to send
    
    // store the last sent seq number for each client so that
    // we only send each message once
    std::map<unsigned int, unsigned int> lastSent;
    
    while(true){
        pthread_mutex_lock(&(server->mutex));
        if(!server->running)
            break;
            
        // iterate through all clients and see if they have messages to send
        for(std::map<unsigned int,Connection*>::iterator it=server->clients.begin();
            it != server->clients.end();
            ++it){
           
            Connection *conn = it->second;
            
            if(conn->status == DISCONNECTED)
                continue;
            
            unsigned int nextToSend = conn->lastReceivedAck + 1;
            if(nextToSend > lastSent[conn->id]){
                // we have received an ack for the last message, and we haven't sent the
                // next one out yet, so if it exists, let's send it now
                if(conn->outbox.size() > 0) {
                    network_send_message_from_server(conn,conn->outbox.front());
                    lastSent[conn->id] = conn->outbox.front()->seqnum;
                }
            }
        }
        pthread_mutex_unlock(&(server->mutex));
        usleep(5000); // 5ms
    }
    pthread_mutex_unlock(&(server->mutex));
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

void set_server(lsp_server* server)
{
    m_server = server;
}

void send_message_to_server(networkMessage* msg)
{   
    if(msg) {
        // we got a message, let's parse it
        pthread_mutex_lock(&(m_server->mutex));
        if(msg->connid == 0 && msg->seqnum == 0 && strlen(msg->payload) == 0){
            // connection request, if first time, make the connection
            //sprintf(host,"%s:%d",inet_ntoa(addr.sin_addr),addr.sin_port);
            //if(m_server->connections.count(host) == 0){
                // this is the first time we've seen this host, add it to the m_server's list of seen hosts
                //m_server->connections.insert(host);
                
                if(DEBUG) printf("Connection request received from client\n");
                
                // build up the new connection object
                Connection *conn = new Connection();
                conn->status = CONNECTED;
                conn->id = m_server->nextConnID;
                m_server->nextConnID++;
                conn->lastSentSeq = 0;
                conn->lastReceivedSeq = 0;
                conn->epochsSinceLastMessage = 0;
                
         // send an ack for the connection request
        if(DEBUG) printf("Sending back connection ack with id %i\n", conn->id);

        msg->connid = conn->id;
                // return msg;
                
                // insert this connection into the list of connections
                m_server->clients.insert(std::pair<int,Connection*>(conn->id,conn));
            //}
        } else {
            if(m_server->clients.count(msg->connid) == 0){
                printf("Bogus connection id received: %d, skipping message...\n",msg->connid);
            } else {
                Connection *conn = m_server->clients[msg->connid];
            
                // reset counter for epochs since we have received a message
                conn->epochsSinceLastMessage = 0;
            
                if(strlen(msg->payload)== 0){
                    // we received an ACK
                    if(DEBUG) printf("m_server received an ACK for conn %d msg %d\n",msg->connid,msg->seqnum);
                    if(msg->seqnum == (conn->lastReceivedAck + 1))
                        conn->lastReceivedAck = msg->seqnum;
                    if(conn->outbox.size() > 0 && msg->seqnum == conn->outbox.front()->seqnum) {
                        delete conn->outbox.front();
                        conn->outbox.pop();
                    }
                } else {
                    // data packet
                    if(DEBUG) printf("m_server received msg %d for conn %d\n",msg->seqnum,msg->connid);
                    if(msg->seqnum == (conn->lastReceivedSeq + 1)){
                        // next in the list
                        conn->lastReceivedSeq++;
                        m_server->inbox.push(msg);
                    
                        // send ack for this message
                        msg->connid = conn->id;
            msg->seqnum = conn->lastReceivedSeq;
            msg->payload = "";
            // return msg;
                    }
                }
            }
        }
        pthread_mutex_unlock(&(m_server->mutex));
    }
    pthread_mutex_unlock(&(m_server->mutex));
}


