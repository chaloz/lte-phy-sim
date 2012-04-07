#ifndef _CODING_H_
#define _CODING_H_

#include "codeblock.h"


void conv_encode(char *in, int len, char *out);

void turbo_encode(char *in, codeblock_param *param, int cbindex, char *out);

#endif //_CODING_H_