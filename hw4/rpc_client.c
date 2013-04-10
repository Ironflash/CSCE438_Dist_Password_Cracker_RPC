#include "rpc_functions.h"

int main(int argc, char** argv) {

	CLIENT *clnt;
	char *host;

	if(argc < 3) {
		printf("usage: %s server_host test_string\n", argv[0]);
		exit(1);
	}
	host = argv[1];
	initialize_client(host, clnt);
}
