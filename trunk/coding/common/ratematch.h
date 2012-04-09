#ifndef _RATE_MATCHING_H_
#define _RATE_MATCHING_H_

typedef struct ratematch_param_s
{
	int E;
	int Ncb;
	int rv;
} ratematch_param;

/**
 * @brief Rate matchig for CC
 *
 * Rate matching for convolutionally coded transport channels and control
 *    information, according to 5.1.1, 36.212
 * 
 * @param in    - input encoded code block
 * @param D     - length of encoded code block
 * @param out   - output bit stream after rate matching
 * @param E     - length of output
 */
void conv_ratematch(char *in, int D, char *out, int E);

void turbo_ratematch(char *in, int D, char *out, ratematch_param *param);

#endif //_RATE_MATCHING_H_