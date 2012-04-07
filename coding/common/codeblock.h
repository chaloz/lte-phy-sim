#ifndef _CODEBLOCK_H_
#define _CODEBLOCK_H_

#define MAX_CODE_BLOCK_SIZE 6144

typedef struct codeblock_param_s
{
	int L;
	int C;
	int B_bar;
	int C_plus;
	int C_minus;
	int K_plus;
	int K_minus;
	int index;
	int F;
} codeblock_param;

void calc_codeblock_param(int B, codeblock_param *param);
void segment_codeblock(char *in, codeblock_param *param, char *out);

#endif //_CODEBLOCK_H_