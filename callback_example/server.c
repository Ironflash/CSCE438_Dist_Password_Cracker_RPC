#include "rpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include <utmp.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static int pnum;
static int count = 0;
static int pnums[2];
static int trigger_count = 0;

void * callbackfn_1_svc(int *pnump,struct svc_req * req) {
        printf("callbackfn exec start\n");
        pnum = *(int *)pnump;

        pnums[count] = pnum;
        count++;
        printf("callbackfn exec end\n");
//      return NULL;
}

void docallback() {
        printf("docallback exec %ld\n",pnum);
        char* host="localhost";
        int ans;

        networkMessage out;
        out.connid = 1;
        out.seqnum = 1;
        out.payload = "Finally_0";;

        ans = callrpc(host, pnum, 1, 1, (__const xdrproc_t) xdr_networkMessage, ( __const char *)  &out,
                        (__const xdrproc_t) xdr_void, ( char *) 0);
        if (ans != 0) {
                fprintf(stderr, "server docallback: ");
                clnt_perrno(ans);
                fprintf(stderr, "\n");
        } else {
                printf("success; reset pnum\n");
                pnum = 0;
        }
        printf("docallbackfn exec end\n");
}

static void nfs_progrm_1(struct svc_req *rqstp, register SVCXPRT *transp) {
        union {
                networkMessage test_func_1_arg;
        } argument;
        char *result;
        xdrproc_t _xdr_argument, _xdr_result;
        char *(*local)(char *, struct svc_req *);

        switch (rqstp->rq_proc) {
        case NULLPROC:
                (void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
                return;

        case TEST_FUNC:
                _xdr_argument = (xdrproc_t) xdr_networkMessage;
                _xdr_result = (xdrproc_t) xdr_wrapstring;
                local = (char *(*)(char *, struct svc_req *)) test_func_1_svc;
                break;

        default:
                svcerr_noproc (transp);
                return;
        }
        memset ((char *)&argument, 0, sizeof (argument));
        if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
                svcerr_decode (transp);
                return;
        }
        result = (*local)((char *)&argument, rqstp);
        if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
                svcerr_systemerr (transp);
        }
        if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
                fprintf (stderr, "%s", "unable to free arguments");
                exit (1);
        }
        return;
}

// SIGNAL HANDLING:
void timer_handler(int s) {
        signal(SIGALRM, SIG_IGN);
        trigger_count++;
        printf("Triggered!! %d\n", trigger_count);
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

char ** test_func_1_svc(networkMessage * argp,struct svc_req * req) {
        static char *result = "this goes back";
        printf("incoming connid: %d\n", argp->connid);
        printf("incoming seqnum: %d\n", argp->seqnum);
        printf("incoming string: \"%s\"\n", argp->payload);
        printf("returning string: %s\n", result);
        return(&result);
}

int main (int argc, char **argv) {
        register SVCXPRT *transp;
        pmap_unset (NFS_PROGRM, NFS_VERS);

        transp = svcudp_create(RPC_ANYSOCK);
        if (transp == NULL) {
                fprintf (stderr, "%s", "cannot create udp service.");
                exit(1);
        }

        registerrpc(NFS_PROGRM, NFS_VERS,6,callbackfn_1_svc, xdr_int, xdr_void);
        registerrpc(NFS_PROGRM, NFS_VERS,5,test_func_1_svc,xdr_networkMessage,xdr_wrapstring);
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
