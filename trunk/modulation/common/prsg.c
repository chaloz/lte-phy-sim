#include "prsg.h"

#define FREE_RUN_STEPS 1600

void prsg_run(prsg_status *status, int steps, char *out)
{
	int i;
	int x1, x2;
	int b1, b2, output, flag;
	
	x1 = status->x1;
	x2 = status->x2;
	flag = (out == NULL)?(0):(1);
	
	for (i=0; i<steps; i++) {
		output = (x1 + x2) & 0x1;
		b1 = ( (x1>>3) + x1 ) & 0x1;
		b2 = ( (x2>>3) + (x2>>2) + (x2>>1) + x2 ) & 0x1;
		
		x1 = x1 >> 1;
		x2 = x2 >> 1;
		
		x1 = x1 | (b1<<30);
		x2 = x2 | (b2<<30);
		
		if (flag) *out++ = output;
	}
	
	status->x1 = x1;
	status->x2 = x2;
}

void prsg_init(prsg_status *status, int cinit)
{
	status->x1 = 1;
	status->x2 = cinit;
	prsg_run(status, FREE_RUN_STEPS, NULL);
}