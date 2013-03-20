/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "cracker.h"


void
cracker_1( char* host )
{
	CLIENT *clnt;
	char * *result_1;
	crackMessage  crack_password_1_arg;
	char * *result_2;
	char*  inform_available_1_arg;
	void  *result_3;
	char * deliver_result_1_arg;
	clnt = clnt_create(host, cracker, 0, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}
	result_1 = crack_password_1(&crack_password_1_arg, clnt);
	if (result_1 == NULL) {
		clnt_perror(clnt, "call failed:");
	}
	result_2 = inform_available_1((void*)&inform_available_1_arg, clnt);
	if (result_2 == NULL) {
		clnt_perror(clnt, "call failed:");
	}
	result_3 = deliver_result_1(&deliver_result_1_arg, clnt);
	if (result_3 == NULL) {
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
	cracker_1( host );
}
