#include "cracker.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_t p_thread;
pthread_attr_t attr;

void *
serv_request(void *data)
{
	struct thr_data
	{
		struct svc_req *rqstp;
		SVCXPRT *transp;
	}*ptr_data;

	union {
		int fill;
	} argument;
	union {
	} result;
	bool_t retval;
	xdrproc_t _xdr_argument, _xdr_result;
	bool_t (*local)(char *, void *, struct svc_req *);

	ptr_data = (struct thr_data  *)data;
	struct svc_req *rqstp = ptr_data-> rqstp;
	register SVCXPRT *transp = ptr_data-> transp;

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return NULL;

	case TEST_FUNC2:
		_xdr_argument = (xdrproc_t) xdr_void;
		_xdr_result = (xdrproc_t) xdr_void;
		local = (bool_t (*) (char *, void *,  struct svc_req *))test_func2_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return NULL;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return NULL;
	}
	retval = (bool_t) (*local)((char *)&argument, (void *)&result, rqstp);
	if (retval > 0 && !svc_sendreply(transp, (xdrproc_t) _xdr_result, (char *)&result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
}

static void
ping_2(struct svc_req *rqstp, register SVCXPRT *transp)
{
	struct data_str
	{
		struct svc_req *rqstp;
		SVCXPRT *transp;
	} 

	*data_ptr=(struct data_str*)malloc(sizeof(struct data_str));
	data_ptr-> rqstp = rqstp;
	data_ptr-> transp = transp;

	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	pthread_create(&p_thread,&attr,serv_request,(void *)data_ptr);
}

void send_msg_to_client(char *host, char *input) {
	CLIENT *clnt;
	//enum clnt_stat retval_1;
	//void *result_1;
	stuff *result; /* return value */
	stuff  out;  /* outgoing parameters */

	out.name = input;
	out.val = 1234;

	clnt = clnt_create(host, TEST_PROG1, TEST_VERS1, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}

	result = test_func1_1(&out, clnt);	/* call the remote function */
	/* test if the RPC succeeded */
	if (result == NULL) {
		clnt_perror(clnt, "call failed:");
		exit(1);
	}
	clnt_destroy (clnt);
}

void *server_app(char *host){
	while(1) {
		printf("Trying to message the client... \n");
		//send_msg_to_client(host, "yay!");
		//printf("Done\n");
		sleep(2);
	}
}

int main(int argc, char** argv) {
	CLIENT *clnt;
	char *host;
	
	host = "localhost";

	pthread_t server_client;
	pthread_create(&server_client,NULL,server_app,host);

	register SVCXPRT *transp;

	pmap_unset (TEST_PROG2, TEST_VERS2);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, TEST_PROG2, TEST_VERS2, ping_2, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (PING, ALPHA, udp).");
		exit(1);
	}
	printf("Done\n");
	svc_run();

	//printf("function returned: \"%s\"\n", result->name);
	//printf("result: %d\n", result);
	//printf("*result: %d\n", *result);
	//stuff *response;
	//response = (stuff*)(*result);
	//printf("function returned: \"%s\"\n", (*response).name);
	clnt_destroy( clnt );
}
