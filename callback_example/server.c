#include "rpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int pnum;

static int count = 0;
int pnums[2];

void * callbackfn_1_svc(int *pnump,struct svc_req * req) {
	printf("callbackfn exec start\n");
	pnum = *(int *)pnump;

	pnums[count] = pnum;
	count++;
	printf("callbackfn exec end\n");
	return NULL;
}

void docallback()
{
	char* host="127.0.0.1";
	int ans = 0;
	stuff out;
	int done = 0;

	for (int i=0; i< sizeof(pnums)/sizeof(pnums[0]); i++) {
		if (pnums[i]!=NULL){
			done = 1;
			break;
		}
	}
	
	for (int i=0; i< sizeof(pnums)/sizeof(pnums[0]); i++) {
		printf("docallback exec %d\n",pnums[i]);
		if (i == 0) {
			out.name = "Finally_0";
		} else {
			out.name = "Finally_1";
		}
		out.val = 64234;

		ans = callrpc(host, pnums[i], 1, 1, (__const xdrproc_t) xdr_stuff, ( __const char *)  &out,
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

int main (int argc, char **argv) {
	register SVCXPRT *transp;
	pmap_unset (NFS_PROGRM, NFS_VERS);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}

    registerrpc(NFS_PROGRM, NFS_VERS,1,test_func_1_svc,xdr_stuff,xdr_wrapstring);
	registerrpc(NFS_PROGRM, NFS_VERS,2, callbackfn_1_svc, xdr_int, xdr_void);
	fprintf(stderr, "server going into svc_run\n");

	//**************************
	fprintf(stderr, "set up period interrupts\n");	
	if (setupinterrupt() == -1) {
		perror("Failed to set up handler for SIGALRM");
		return 1;
	}
	if (setupitimer() == -1) {
		perror("Failed to set up the ITIMER_REAL interval timer");
		return 1;
	} //*/
	//**************************

	fprintf(stderr, "server into svc_run\n");
	svc_run ();
	fprintf (stderr, "%s %d", "check point5\n.",transp->xp_port);
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}

char ** test_func_1_svc(stuff * argp,struct svc_req * req)
{
	static char *result = "this goes back";
	printf("incoming string: \"%s\"\n", argp->name);
	printf("incoming value: %d\n", argp->val);
	printf("returning string: %s\n", result);
	//alarm();
	return(&result);
}
