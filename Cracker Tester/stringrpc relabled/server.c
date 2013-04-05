#include "cracker.h"
#include <stdio.h>

stuff *
test_func_1_svc(stuff *argp, struct svc_req *rqstp)
{
	//static char *result = "this goes back";

	static stuff result;

	result.name = "goes back";
	result.val = 0;

	/*
	printf("original char *result address: %d\n", result);
	printf("original char *result value: %d\n", *result);
	printf("original &result address: %d\n", &result);

	stuff temp;
	temp.name = "this goes back";
	temp.val = 0;

	// testing casting back and forth *****************

	printf("original stuff address: %d\n", &temp);
	printf("original stuff value: %s\n", temp.name);
	
	static char *casted_result;
	casted_result = (char*)&temp;
	printf("casted_result address: %d\n", casted_result);

	stuff *revert;
	revert = (stuff*)casted_result;
	printf("reverted to stuff address: %d\n", revert);
	printf("reverted to stuff value: %s\n", (*revert).name);

	if (&temp == revert) {
		printf("Casting worked!!\n");
	} else {
		printf("Casting did NOT work!!\n");
	}
	// ************************************************

	//result = (char*)&temp;
	
	//printf("result = (char*)&temp: %d\n", result);
	//printf("return(&result): %d\n", &result);
	//*/
	
	printf("incoming string: \"%s\"\n", argp->name);
	printf("incoming value: %d\n", argp->val);
	//printf("returning string: %s\n", ((stuff*)result)->name);
	
	return(&result);
}
