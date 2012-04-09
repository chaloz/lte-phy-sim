#include <math.h>
#include "complex.h"
#include "errcode.h"

/* ========================================================================= */
/*              LOCAL VARIABLES DEFINITION                                   */
/* ========================================================================= */
static complex bpsk_mapper[2] = {
	{+1, +1},
	{-1, -1}
};

static complex qpsk_mapper[4] = {
	{+1, +1},
	{+1, -1},
	{-1, +1};
	{-1, -1}
};

static complex qam16_mapper[16] = {
	{+1, +1},
	{+1, +3},
	{+3, +1},
	{+3, +3},
	{+1, -1},
	{+1, -3},
	{+3, -1},
	{+3, -3},
	{-1, +1},
	{-1, +3},
	{-3, +1},
	{-3, +3},
	{-1, -1},
	{-1, -3},
	{-3, -1},
	{-3, -3}
};

static complex qam64_mapper[64] = {
	{+3, +3}, {+3, +1}, {+1, +3}, {+1, +1},
	{+3, +5}, {+3, +7}, {+1, +5}, {+1, +7},
	{+5, +3}, {+5, +1}, {+7, +3}, {+7, +1},
	{+5, +5}, {+5, +7}, {+7, +5}, {+7, +7},
	{+3, -3}, {+3, -1}, {+1, -3}, {+1, -1},
	{+3, -5}, {+3, -7}, {+1, -5}, {+1, -7},
	{+5, -3}, {+5, -1}, {+7, -3}, {+7, -1},
	{+5, -5}, {+5, -7}, {+7, -5}, {+7, -7},
	
	{-3, +3}, {-3, +1}, {-1, +3}, {-1, +1},
	{-3, +5}, {-3, +7}, {-1, +5}, {-1, +7},
	{-5, +3}, {-5, +1}, {-7, +3}, {-7, +1},
	{-5, +5}, {-5, +7}, {-7, +5}, {-7, +7},
	{-3, -3}, {-3, -1}, {-1, -3}, {-1, -1},
	{-3, -5}, {-3, -7}, {-1, -5}, {-1, -7},
	{-5, -3}, {-5, -1}, {-7, -3}, {-7, -1},
	{-5, -5}, {-5, -7}, {-7, -5}, {-7, -7}
};

static int bpsk_mod(char *in, int len, complex *out)
{
	int i;
	
	for (i=0; i<len; i++) {
		COMPLEX_SCALE(bpsk_mapper[in[i]], 1/sqrt(2), out[i]);
	}
	
	return len;
}

static int qpsk_mod(char *in, int len, complex *out)
{
	char *ptr = in;
	int sym_num, sym_idx;
	int i;
	
	assert( (len&0x1) == 0);
	sym_num = len >> 1;
	
	for (i=0; i<sym_num; i++) {
		sym_idx = (ptr[0]<<1) + (ptr[1]);
		COMPLEX_SCALE(qpsk_mapper[sym_idx], 1/sqrt(2), out[i]);
		ptr += 2;
	}
	
	return sym_num;
}

static int qam16_mod(char *in, int len, complex *out)
{
	char *ptr = in;
	int sym_num, sym_idx;
	int i;
	
	assert( (len&0x3) == 0);
	sym_num = len >> 2;
	
	for (i=0; i<sym_num; i++) {
		sym_idx = (ptr[0]<<3) + (ptr[1]<<2) + (ptr[2]<<1) + (ptr[3]);
		COMPLEX_SCALE(qam16_mapper[sym_idx], 1/sqrt(10), out[i]);
		ptr += 4;
	}
	
	return sym_num;
}

static int qam64_mod(char *in, int len, complex *out)
{
	char *ptr = in;
	int sym_num, sym_idx;
	int i;
	
	assert( (len&0x3F) == 0);
	sym_num = len >> 6;
	
	for (i=0; i<sym_num; i++) {
		sym_idx = (ptr[0]<<7) + (ptr[1]<<6) + (ptr[2]<<5) + (ptr[3]<<4) +
				(ptr[4]<<3) + (ptr[5]<<2) + (ptr[6]<<1) + (ptr[7]);
		COMPLEX_SCALE(qam64_mapper[sym_idx], 1/sqrt(42), out[i]);
		ptr += 6;
	}
	
	return sym_num;
}

int modulation_map(char *in, int len, complex *out, int type)
{
	int sym_num;
	
	if (type == MOD_BPSK)
		sym_num = bpsk_mod(in, len, out);
	else if (type == MOD_QPSK)
		sym_num = qpsk_mod(in, len, out);
	else if (type == MOD_16QAM)
		sym_num = qam16_mod(in, len, out);
	else if (type == MOD_64QAM)
		sym_num = qam64_mod(in, len, out);
	else
		exit(MOD_TYPE_ERROR);
		
	return sym_num;
}