/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "cracker.h"

bool_t
xdr_networkMessage(XDR *xdrs, networkMessage *objp)
{

	if (!xdr_int(xdrs, &objp->connid))
		return (FALSE);
	if (!xdr_int(xdrs, &objp->seqnum))
		return (FALSE);
	if (!xdr_string(xdrs, &objp->payload, 64))
		return (FALSE);
	return (TRUE);
}
