#include "oper.h"
#include <stdlib.h>

double *
avg_1_svc(in1 *argp, struct svc_req *rqstp)
{
	static double result;
	int i;
	double sum = 0;

	for(i = 0; i < argp->y.y_len ; i++)
		sum = sum + argp->y.y_val[i];

	result = sum/argp->y.y_len;
	return &result;
}

out1 *
maxmin_1_svc(in1 *argp, struct svc_req *rqstp)
{
	static out1  result;
	int i;
	int min = argp->y.y_val[0];
	int max = argp->y.y_val[0];

    // Min and Max
    for (i = 1; i < argp->y.y_len; i++) {
        if (argp->y.y_val[i] < min) {
            min = argp->y.y_val[i];
        }
        if (argp->y.y_val[i] > max) {
            max = argp->y.y_val[i];
        }
    } 

	result.maxmin[0] = max; 
	result.maxmin[1] = min; 
	return &result;
}

out2 *
proday_1_svc(in2 *argp, struct svc_req *rqstp)
{
	static out2 result;
	int i;

	result.pr.pr_len = argp->y.y_len;
	result.pr.pr_val = (double*) malloc(argp->y.y_len*sizeof(double));
	if (result.pr.pr_val == NULL) 
	{
        perror("ERROR allocating memory");
		exit(1);
    }

	for(i = 0; i < argp->y.y_len ; i++)
		result.pr.pr_val[i] = argp->a * argp->y.y_val[i];
	
	return &result;
}