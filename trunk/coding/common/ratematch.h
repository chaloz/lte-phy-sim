#ifndef _RATE_MATCHING_H_
#define _RATE_MATCHING_H_

typedef struct ratematch_param_s
{
	int E;
	int Ncb;
	int rv;
} ratematch_param;

void conv_ratematch(char *in, int D, char *out, int E);

void turbo_ratematch(char *in, int D, char *out, ratematch_param *param);

#endif //_RATE_MATCHING_H_