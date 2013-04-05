#include "rpc.h"
#include <stdio.h>
#include <stdlib.h>

void test_fn(stuff* argp) {
	printf("incoming string: \"%s\"\n", argp->name);
	printf("incoming value: %d\n", argp->val);
}

gettransient(int proto, int  vers, int* sockp) {
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

int callback(struct svc_req *rqstp, SVCXPRT * transp)
{
	union {
		stuff test_func_1_arg;
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

			if (!svc_getargs(transp, xdr_stuff, &argument)) {
				svcerr_decode(transp);
				return (1);
			}
			test_fn((stuff *)&argument);
				fprintf(stderr, "client got callback\n");
			if (!svc_sendreply(transp, (xdrproc_t) xdr_void, 0)) {
				fprintf(stderr, "err: exampleprog");
				return (1);
			}
	}
}

int main(int argc, char** argv) {

	CLIENT *clnt;
	char *host;
	char **result; // return value

	if(argc < 3) {
		printf("usage: %s server_host test_string\n", argv[0]);
		exit(1);
	}
	host = argv[1];
	clnt = clnt_create(host, NFS_PROGRM, NFS_VERS, "udp");

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


	//printf(" making RPC call\n");
	ans = callrpc(host, NFS_PROGRM, NFS_VERS,
			(__const u_long) 2, (__const xdrproc_t) xdr_int, &x, (__const xdrproc_t) xdr_void, 0);

	printf(" RPC called\n");
	if ((enum clnt_stat) ans != RPC_SUCCESS) {
		fprintf(stderr, "call callrpc: ");
		clnt_perrno(ans);
		fprintf(stderr, "\n");
	}
	
	stuff out;
	out.name = argv[2];
	out.val = 1234;

	result = test_func_1(&out, clnt);	// call the remote function
	printf(" test_func_1 succeeded \n");

	// test if the RPC succeeded
	if (result == NULL) {
		clnt_perror(clnt, "call test_func failed:");
		exit(1);
	}
	//printf(" result %s %s \n",*result,(*((stuff*)result1)).name);
	//sleep(10);
	svc_run();
	clnt_destroy( clnt );
}
