/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "cracker.h"


void
cracker_prog_1( char* host, char* name)
{
	CLIENT *clnt;
	networkMessage *result; /* return value */
	networkMessage  out;  /* outgoing parameters */

	clnt = clnt_create(host, CRACKER_PROG, CRACKER_VERS, "tcp");
	//clnt = clnt_create(host, CRACKER_PROG, CRACKER_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}
	out.connid = 1;
	out.seqnum = 1;
	out.payload = name;
	result = send_message_1(&out, clnt);	/* call the remote function */
	/* test if the RPC succeeded */
	if (result == NULL) {
		clnt_perror(clnt, "call failed:");
		exit(1);
	}
	printf("function returned: \"%s\"\n", result->payload);
	clnt_destroy( clnt );
}

int
main(int argc, char** argv)
{
	char *host;
	char *arg2;

	if(argc < 3) {
		printf("usage: %s server_host test_string\n", argv[0]);
		exit(1);
	}
	host = argv[1];
	arg2 = argv[2];

	cracker_prog_1( host, arg2 );
}
