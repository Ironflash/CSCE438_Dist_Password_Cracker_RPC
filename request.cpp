#include <iostream>
#include "lsp_client.h"
#include <rpc/rpc.h>
#include "cracker.h"

int main(int argc, char* argv[]){
    // randomization seed requested by Dr. Stoleru
    srand(12345);
    
    if(argc != 4) {
        printf("Usage: ./request host:port hash len\n");
        return -1;
    }
    
    // parse ip and port from arguments
    char* portstr = strstr(argv[1],":");
    int port;
    if(portstr != NULL) {
        port = atoi(portstr+1);
    } else {
        printf("Usage: ./request host:port hash len\n");
        return -1;
    }
    portstr[0] = NULL;
    
    char* hash = argv[2];
    int len = atoi(argv[3]);

    // do not allow passwords longer than 6 characters
    if(len > 6) {
        printf("The password you provided is too large. Maximum password length is 6 characters.\n");
        return -1;
    }
    
    //lsp_set_drop_rate(0.2); // 20% of packets dropped
    //lsp_set_epoch_lth(0.1); // 100 ms per epoch = fast resends on failure
    //lsp_set_epoch_cnt(20); // 20 epochs (2 seconds) with no response
    
    //Create the client
    char *host = argv[1];
    char type[] = "udp";
    CLIENT *cl = clnt_create(host, CRACKER_PROG, CRACKER_VERS,type);
    if (cl == NULL) 
    {
        clnt_pcreateerror(argv[1]);
        exit(1);
    }

    // create a lsp client and connect to server
    // lsp_client *client = lsp_client_create(argv[1], port);
    // if(!client){
    //     printf("The connection to the server failed. Exiting...\n");
    //     return -1;
    // }
    
    printf("The connection to the server has been established\n");
    
    // calculate the lower and upper passwords for a certain length
    char buffer[1024];
    char lower[7];
    char upper[7];
    for(int k = 0; k < len; k++){
        lower[k] = 'a';
        upper[k] = 'z';
    }
    lower[len] = NULL;
    upper[len] = NULL;
    int buflen = sprintf(buffer, "c %s %s %s", hash, lower, upper);
    
    printf("sending [%d]: %s\n", buflen, buffer);

    // *******************************
    // setup RPC
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
    (void)svc_register(xprt, /*(rpcprog_t)*/ x, 1, (void(*)())callback, 0);
 
    //printf(" making RPC call\n");
    ans = callrpc(host, CRACKER_PROG, CRACKER_VERS,
            (__const u_long) 2, (__const xdrproc_t) xdr_int, &x, (__const xdrproc_t) xdr_void, 0);

    printf(" RPC called\n");
    if ((enum clnt_stat) ans != RPC_SUCCESS) {
        fprintf(stderr, "call callrpc: ");
        clnt_perrno((clnt_stat)ans);
        fprintf(stderr, "\n");
    }
    // *******************************
    
    networkMessage* result_1;
    // send password crack request to server
    networkMessage crack_password_1_arg;
    crack_password_1_arg.connid = 0;
    crack_password_1_arg.seqnum = 0;
    crack_password_1_arg.payload = "";
    //crack_password_1_arg.len = buflen+1;
    result_1 = send_message_1(&crack_password_1_arg, cl);

    printf("Print out result\n");
    printf("result connid = %i\n", result_1->connid);
    //printf("result length %d\n", (int)strlen(result_1->payload));

    //for (int i=0; i<; i++) {
      //  printf("%c\n", result_1->payload[i]);
    //}
    
    //check response
    if (result_1 == NULL) {
        char failed[] = "call failed:";
        clnt_perror(cl, failed);
    }
    else {

        if(result_1->payload[0] == 'x')
            printf("Not Found\n");
        else if (result_1->payload[0] == 'f')
            printf("Found: %s\n",result_1->payload + 2);
        else
            printf("Unknown response: %s\n",result_1->payload);
    }
    // lsp_client_write(client,(uint8_t*)buffer,buflen+1);
    // int bytes_read = lsp_client_read(client,(uint8_t*)buffer);
    // if(bytes_read == 0){
    //     printf("Disconnected\n");
    // } 
    // else {
    //     if(buffer[0] == 'x')
    //         printf("Not Found\n");
    //     else if (buffer[0] == 'f')
    //         printf("Found: %s\n",buffer + 2);
    //     else
    //         printf("Unknown response: %s\n",buffer);
    // }
    // lsp_client_close(client);    
    clnt_destroy( cl );
    return 0;
}
