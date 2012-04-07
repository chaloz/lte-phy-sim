#include <assert.h>
#include "codeblock.h"
#include "crc.h"

/* ========================================================================= */
/*              LOCAL VARIABLES DEFINITION                                   */
/* ========================================================================= */
#define MAX_NUM_OF_K 188
static int K_table[] = {
	40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128, 136, 144, 152, 160, 168, 176, 184, 192, 
	200, 208, 216, 224, 232, 240, 248, 256, 264, 272, 280, 288, 296, 304, 312, 320, 328, 336, 
	344, 352, 360, 368, 376, 384, 392, 400, 408, 416, 424, 432, 440, 448, 456, 464, 472, 480, 
	488, 496, 504, 512, 528, 544, 560, 576, 592, 608, 624, 640, 656, 672, 688, 704, 720, 736, 
	752, 768, 784, 800, 816, 832, 848, 864, 880, 896, 912, 928, 944, 960, 976, 992, 1008, 1024, 
	1056, 1088, 1120, 1152, 1184, 1216, 1248, 1280, 1312, 1344, 1376, 1408, 1440, 1472, 1504, 
	1536, 1568, 1600, 1632, 1664, 1696, 1728, 1760, 1792, 1824, 1856, 1888, 1920, 1952, 1984, 
	2016, 2048, 2112, 2176, 2240, 2304, 2368, 2432, 2496, 2560, 2624, 2688, 2752, 2816, 2880, 
	2944, 3008, 3072, 3136, 3200, 3264, 3328, 3392, 3456, 3520, 3584, 3648, 3712, 3776, 3840, 
	3904, 3968, 4032, 4096, 4160, 4224, 4288, 4352, 4416, 4480, 4544, 4608, 4672, 4736, 4800, 
	4864, 4928, 4992, 5056, 5120, 5184, 5248, 5312, 5376, 5440, 5504, 5568, 5632, 5696, 5760, 
	5824, 5888, 5952, 6016, 6080, 6144
};



void calc_codeblock_param(int B, codeblock_param *param)
{
	int tmp, i;
	float t;
	
	assert(B > 0);
	if (B <= MAX_CODE_BLOCK_SIZE) {
		param->L = 0;
		param->C = 1;
		param->B_bar = B;
	}
	else {
		param->L = 24;
		t = (float)B / (MAX_CODE_BLOCK_SIZE - param->L);
		param->C = (int)t;
		if (param->C != t)
			param->C++;
			
		param->B_bar = B + param->C * param->L;
	}
	
	/* first segmentation size = 
	 *   minimum K in table 5.1.3-3 such that C*K >= B'
	 */
	assert(param->C > 0);
	tmp = param->B_bar / param->C;
	for (i=0; i<MAX_NUM_OF_K; i++) {
		if (K_table[i] >= tmp) break;
	}
	assert(i < MAX_NUM_OF_K);
	param->index = i;
	
	if (param->C == 1) {
		param->K_plus = K_table[i];
		param->C_plus = 1;
		param->K_minus = 0;
		param->C_minus = 0;
	}
	else {
		//assert(i>0);
		param->K_plus = K_table[i];
		param->K_minus = K_table[i-1];
		tmp = K_table[i] - K_table[i-1];
		param->C_minus = (param->C * param->K_plus - param->B_bar) / tmp;
		param->C_plus = param->C - param->C_minus;
	}
	
	param->F = param->C_plus * param->K_plus + param->C_minus * param->K_minus - param->B_bar;
}

void segment_codeblock(char *in, codeblock_param *param, char *out)
{
	int i;
	int cblen, filler, copylen;
	
	if (param->F > 0) {
		memset(out, 0, param->F * sizeof(char));
	}
	
	if (param->C == 1) {
		memcpy(out+param->F, in, param->B_bar * sizeof(char));
	}
	else {
		for (i=0; i<param->C; i++) {
			cblen = (i<param->C_minus)?(param->K_minus):(param->K_plus);
			filler = (i==0)?(param->F):(0);
			copylen = cblen - param->L - filler;
			memcpy(out+filler, in, copylen * sizeof(char));
			
			/* add code block CRC */
			calc_crc(out, cblen - param->L, CRC_POLY_24B);
			
			in += copylen;
			out += cblen;
		}
	}
}