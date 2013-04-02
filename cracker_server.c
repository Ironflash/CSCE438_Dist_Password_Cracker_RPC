/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "cracker.h"

networkMessage *
send_message_1_svc(networkMessage *argp, struct svc_req *rqstp)
{

	static networkMessage  result;

	result.connid = 0;
	result.seqnum = 0;
	result.payload = "this goes back";

	printf("incoming string: \"%s\"\n", argp->payload);
	printf("incoming connid: %d\n", argp->connid);
	printf("incoming seqnum: %d\n", argp->seqnum);
	printf("returning string: %d\n", result.payload);

	return(&result);
}
