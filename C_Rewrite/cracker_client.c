/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "cracker.h"


void
cracker_prog_1( char* host )
{
	CLIENT *clnt;
	networkMessage  *result_1;
	networkMessage  send_message_1_arg;
	void  *result_2;
	int  callbackfn_1_arg;
	clnt = clnt_create(host, CRACKER_PROG, CRACKER_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}
	result_1 = send_message_1(&send_message_1_arg, clnt);
	if (result_1 == NULL) {
		clnt_perror(clnt, "call failed:");
	}
	result_2 = callbackfn_1(&callbackfn_1_arg, clnt);
	if (result_2 == NULL) {
		clnt_perror(clnt, "call failed:");
	}
	clnt_destroy( clnt );
}


main( int argc, char* argv[] )
{
	char *host;

	if(argc < 2) {
		printf("usage: %s server_host\n", argv[0]);
		exit(1);
	}
	host = argv[1];
	cracker_prog_1( host );
}
