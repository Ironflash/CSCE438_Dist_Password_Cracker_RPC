#include "cracker.h"
#include <stdio.h>

stuff *
test_func1_1_svc(stuff *argp, struct svc_req *rqstp)
{
	//static char *result = "this goes back";

	static stuff result;

	result.name = "this goes back";
	result.val = 0;
	
	printf("incoming string: \"%s\"\n", argp->name);
	printf("incoming value: %d\n", argp->val);
	//printf("returning string: %s\n", ((stuff*)result)->name);
	
	return(&result);
}
