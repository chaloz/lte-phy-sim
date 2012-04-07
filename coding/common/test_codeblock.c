#include <stdio.h>
#include "codeblock.h"

void main()
{
	codeblock_param param;
	char input[16000];
	char output[20000];
	int B_byte[5] = {4, 15, 120, 1000, 2000};
	int i;
	
	for (i=0; i<5; i++) {
		calc_codeblock_param(B_byte[i]*8, &param);
		
		printf("code block parameters:\n");
		printf("B' = %d\n", param.B_bar);
		printf("L = %d\n", param.L);
		printf("C = %d\n", param.C);
		printf("C+ = %d\n", param.C_plus);
		printf("C- = %d\n", param.C_minus);
		printf("K+ = %d\n", param.K_plus);
		printf("K- = %d\n", param.K_minus);
		printf("F = %d\n", param.F);
		
		segment_codeblock(input, &param, output);
	}
}