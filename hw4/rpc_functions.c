/* 
    File: rpc_functions.c

    Authors: Brad Coffman
             Andy Hamption
             Gregory LaFlash
             Daniel Timothy S. Tan
             Department of Computer Science
             Texas A&M University
    Date   : 04/05/2013

    RPC related functions for HW4 in CSCE 438-500
*/

#include "rpc_functions.h"

/*
 **************************************************
 *
 *			RPC SERVER RELATED FUNCTIONS
 *
 **************************************************
 */

static int pnum;
static int count = 0;
static int pnums[1];

//adds the client program number to the array of program numbers
void * callbackfn_1_svc(int *pnump,struct svc_req * req) {
	printf("callbackfn exec start\n");
	pnum = *(int *)pnump;

	pnums[count] = pnum;
	count++;
	printf("callbackfn exec end\n");
	return NULL;
}

void docallback() {
	char* host="127.0.0.1";
	int ans = 0;
	networkMessage out;
	int done = 0;

	int i;
	for (i=0; i< sizeof(pnums)/sizeof(pnums[0]); i++) {
		if (pnums[i]!=NULL){
			done = 1;
			break;
		}
	}
	
	for (i=0; i< sizeof(pnums)/sizeof(pnums[0]); i++) {
		printf("docallback exec %d\n",pnums[i]);
		if (i == 0) {
			out.payload = "Finally_0";
		} else {}
		out.connid = 1;
		out.seqnum = 1;

		ans = callrpc(host, pnums[i], 1, 1, (__const xdrproc_t) xdr_networkMessage, ( __const char *)  &out,
				(__const xdrproc_t) xdr_void, ( char *) 0);
		if (ans != 0) {
			fprintf(stderr, "server docallback: ");
			clnt_perrno(ans);
			fprintf(stderr, "\n");
		}
		if (pnums[i]!=NULL){
			pnums[i]=NULL;
		}
	}
	printf("docallbackfn exec end\n");
}

// SIGNAL HANDLING:
void timer_handler(int s) {
	signal(SIGALRM, SIG_IGN);
	printf("Triggered!!\n");
	docallback();
	signal(SIGALRM, timer_handler);
}

int setupinterrupt() { // set up myhandler for SIGPROF
	struct sigaction act;
	act.sa_handler = timer_handler;
	act.sa_flags = 0;
	return (sigemptyset(&act.sa_mask) || sigaction(SIGALRM, &act, NULL));
}

int setupitimer() { // set ITIMER_REAL for ___-second intervals
    struct itimerval value;
    value.it_interval.tv_sec = 2; //___
    value.it_interval.tv_usec = 0;
    value.it_value = value.it_interval;
    return (setitimer(ITIMER_REAL, &value, NULL));
}

// void* test_func_1_svc(networkMessage * argp,struct svc_req * req) {
// 	//static char *result = "this goes back";
// 	fprintf(stderr,"reached4");
// 	char* result = "this goes back";
// 	fprintf(stderr,"reached3");
// 	printf("incoming connid: %d\n", argp->connid);
//     printf("incoming seqnum: %d\n", argp->seqnum);
//     printf("incoming payload: \"%s\"\n", argp->payload);
//     // send_message_to_server(argp);

// 	//return(&result);
// }

// ****************************************
// RPC SERVER MAIN FUNCTION:
// ****************************************

void initialize_server() {
	register SVCXPRT *transp;
	pmap_unset (NFS_PROGRM, NFS_VERS);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}

    registerrpc(NFS_PROGRM, NFS_VERS,1,test_func_1_svc,xdr_networkMessage,xdr_wrapstring);
	registerrpc(NFS_PROGRM, NFS_VERS,2, callbackfn_1_svc, xdr_int, xdr_void);
	fprintf(stderr, "server going into svc_run\n");

	//**************************
	fprintf(stderr, "set up period interrupts\n");	
	if (setupinterrupt() == -1) {
		perror("Failed to set up handler for SIGALRM");
		//return 1;
	}
	if (setupitimer() == -1) {
		perror("Failed to set up the ITIMER_REAL interval timer");
		//return 1;
	} //*/
	//**************************

	fprintf(stderr, "server into svc_run\n");
	svc_run ();
	fprintf (stderr, "%s %d", "check point5\n.",transp->xp_port);
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}

/*
 **************************************************
 *
 *			RPC CLIENT RELATED FUNCTIONS
 *
 **************************************************
 */

void test_fn(networkMessage* argp) {
    printf("incoming connid: \"%d\"\n", argp->connid);
    printf("incoming seqnum: %d\n", argp->seqnum);
    printf("incoming payload: %s\n", argp->payload);
    printf("server executing this on the client\n");
    //analyze the message
	// if(argp) {
 //        if(argp->connid == client->connection->id){
 //            pthread_mutex_lock(&(client->mutex));
            
 //            // reset counter for epochs since we have received a message
 //            client->connection->epochsSinceLastMessage = 0;
            
 //            if(argp->payload.length() == 0){
 //                // we received an ACK
 //                if(DEBUG) printf("Client received an ACK for argp %d\n",argp->seqnum;
 //                if(argp->seqnum == (client->connection->lastReceivedAck + 1)){
 //                    // this sequence number is next in line, even if it overflows
 //                    client->connection->lastReceivedAck = argp->seqnum();
 //                }
 //                if(client->connection->outbox.size() > 0 && argp->seqnum() == client->connection->outbox.front()->seqnum {
 //                    delete client->connection->outbox.front();
 //                    client->connection->outbox.pop();
 //                }
 //            } else {
 //                // data packet
 //                if(DEBUG) printf("Client received argp %d\n",argp->seqnum;
 //                if(argp->seqnum == (client->connection->lastReceivedSeq + 1)){
 //                    // next in the list
 //                    client->connection->lastReceivedSeq++;
 //                    client->inbox.push(argp);
                    
 //                    // send ack for this message
 //                    network_acknowledge(client->connection);
 //                }
 //            }
            
 //            pthread_mutex_unlock(&(client->mutex));
 //        }
 //    }
}

int gettransient(int proto, int  vers, int* sockp) {
	static int prognum = 0x40040000;
	int s, len, socktype;
	struct sockaddr_in addr;
	switch(proto) {
		case IPPROTO_UDP:
			socktype = SOCK_DGRAM;
			break;
		case IPPROTO_TCP:
			socktype = SOCK_STREAM;
			break;
		default:
			fprintf(stderr, "unknown protocol type\n");
			return 0;
	}
	if (*sockp == RPC_ANYSOCK) {
		if ((s = socket(AF_INET, socktype, 0)) < 0) {
			perror("socket");
			return (0);
		}
		*sockp = s;
	}
	else
		s = *sockp;
	addr.sin_addr.s_addr = 0;
	addr.sin_family = AF_INET;
	addr.sin_port = 0;
	len = sizeof(addr);
	
	// may be already bound, so don’t check for error

	bind(s, (struct sockaddr *) &addr, len);
	if (getsockname(s, (struct sockaddr *) &addr, &len)< 0) {
		perror("getsockname");
		return (0);
	}
	while (!pmap_set(prognum++, vers, proto,
				ntohs(addr.sin_port))) continue;
	return (prognum-1);
}

int callback(struct svc_req *rqstp, SVCXPRT * transp) {
	union {
        networkMessage test_func_1_arg;
    } argument;

	fprintf(stderr , "hello\n");

	switch (rqstp->rq_proc) {
		case 0:
			if (!svc_sendreply(transp, (xdrproc_t) xdr_void, 0)) {
				fprintf(stderr, "err: exampleprog\n");
				return (1);
			}
			return (0);
		case 1:
			
			memset ((char *)&argument, 0, sizeof (argument));

			if (!svc_getargs(transp, xdr_networkMessage, &argument)) {
				svcerr_decode(transp);
				return (1);
			}
			test_fn((networkMessage *)&argument);
				fprintf(stderr, "client got callback\n");
			if (!svc_sendreply(transp, (xdrproc_t) xdr_void, 0)) {
				fprintf(stderr, "err: exampleprog");
				return (1);
			}
	}

}

// ****************************************
// RPC CLIENT MAIN FUNCTION:
// ****************************************

// void initialize_client(char *host) {
// 	CLIENT *clnt;
// 	char**result; // return value

// 	clnt = clnt_create(host, NFS_PROGRM, NFS_VERS, "udp");

// 	int x, ans, s;
// 	SVCXPRT *xprt;
// 	s = RPC_ANYSOCK;

// 	x = gettransient(IPPROTO_UDP, 1, &s);
// 	fprintf(stderr, "client gets prognum %d\n", x);
// 	if ((xprt = svcudp_create(s)) == NULL) {
// 		fprintf(stderr, "rpc_server: svcudp_create\n");
// 		exit(1);
// 	}
// 	// protocol is 0 - gettransient does registering
// 	(void)svc_register(xprt, /*(rpcprog_t)*/ x, 1, callback, 0);


// 	//printf(" making RPC call\n");
// 	ans = callrpc(host, NFS_PROGRM, NFS_VERS,
// 			(__const u_long) 2, (__const xdrproc_t) xdr_int, &x, (__const xdrproc_t) xdr_void, 0);

// 	printf(" RPC called\n");
// 	if ((enum clnt_stat) ans != RPC_SUCCESS) {
// 		fprintf(stderr, "call callrpc: ");
// 		clnt_perrno(ans);
// 		fprintf(stderr, "\n");
// 	}
	
// 	networkMessage out;
// 	out.connid = 0;
// 	out.seqnum = 0;
// 	out.payload = "test";

// 	result = test_func_1(&out, clnt);	// call the remote function
// 	printf(" test_func_1 succeeded \n");

// 	// test if the RPC succeeded
// 	if (result == NULL) {
// 		clnt_perror(clnt, "call test_func failed:");
// 		exit(1);
// 	}
// 	//printf(" result %s %s \n",*result,(*((stuff*)result1)).name);
// 	//sleep(10);
// 	svc_run();
// 	clnt_destroy( clnt );
// }
int initialize_client(char *host,CLIENT* clnt) {
	// CLIENT *clnt;
	char**result; // return value

	// clnt = clnt_create(host, NFS_PROGRM, NFS_VERS, "udp");

	int x, ans, s;
	SVCXPRT *xprt;
	s = RPC_ANYSOCK;

	x = gettransient(IPPROTO_UDP, 1, &s);
	fprintf(stderr, "client gets prognum %d\n", x);
	if ((xprt = svcudp_create(s)) == NULL) {
		fprintf(stderr, "rpc_server: svcudp_create\n");
		exit(1);
	}
	// protocol is 0 - gettransient does registering
	(void)svc_register(xprt, /*(rpcprog_t)*/ x, 1, callback, 0);

	// printf(" making RPC call\n");
	// //tell the server that it needs to call the callback for this client
	// ans = callrpc(host, NFS_PROGRM, NFS_VERS,
	// 		(__const u_long) 2, (__const xdrproc_t) xdr_int, &x, (__const xdrproc_t) xdr_void, 0);

	// printf(" RPC called\n");
	// if ((enum clnt_stat) ans != RPC_SUCCESS) {
	// 	fprintf(stderr, "call callrpc: ");
	// 	clnt_perrno(ans);
	// 	fprintf(stderr, "\n");
	// }
	
	// networkMessage out;
	// out.connid = 0;
	// out.seqnum = 0;
	// out.payload = "test";

	// //result = 
	// test_func_1(&out, clnt);	// call the remote function
	// printf(" test_func_1 succeeded \n");

	// test if the RPC succeeded
	// if (result == NULL) {
	// 	clnt_perror(clnt, "call test_func failed:");
	// 	exit(1);
	// }
	
	// printf(" result %s %s \n",*result,(*((stuff*)result1)).name);
	// sleep(10);
	// svc_run();
	return x;
}

// void send_message_to_server(const char *host,CLIENT* clnt,int progNum) {
	// int ans = callrpc(host, NFS_PROGRM, NFS_VERS,
	// 			(__const u_long) 2, (__const xdrproc_t) xdr_int, &progNum, (__const xdrproc_t) xdr_void, 0);

	// printf(" RPC called\n");
	// if ((enum clnt_stat) ans != RPC_SUCCESS) {
	// 	fprintf(stderr, "call callrpc: ");
	// 	clnt_perrno(ans);
	// 	fprintf(stderr, "\n");
	// }
	// svc_run();
// }
