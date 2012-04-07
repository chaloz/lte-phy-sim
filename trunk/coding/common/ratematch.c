#include <assert.h>

#define COLUMN_WIDTH 32
#define MAX_K_PI (192*32)

/* ========================================================================= */
/*              LOCAL VARIABLES DEFINITION                                   */
/* ========================================================================= */
static int permutate_pattern_conv[COLUMN_WIDTH] = {
	1, 17, 9, 25, 5, 21, 13, 29, 3, 19, 11, 27, 7, 23, 15, 31, 
	0, 16, 8, 24, 4, 20, 12, 28, 2, 18, 10, 26, 6, 22, 14, 30
};

static int permutate_pattern_turbo[COLUMN_WIDTH] = {
	0, 16, 8, 24, 4, 20, 12, 28, 2, 18, 10, 26, 6, 22, 14, 30, 
	1, 17, 9, 25, 5, 21, 13, 29, 3, 19, 11, 27, 7, 23, 15, 31
};

static char tmp_buf[MAX_K_PI];
static char interleave_buf[3][MAX_K_PI];
static char circular_buf[3*MAX_K_PI];

void conv_ratematch(char *in, int D, char *out, int E)
{
	float t;
	int row, kpi, filler;
	int i, j, r, c, pc;
	char *cir_buf_ptr;
	
	t = (float)D / COLUMN_WIDTH;
	row = (int)t;
	if (row != t) row++;
	kpi = row * COLUMN_WIDTH;
	assert(kpi <= MAX_K_PI);
	filler = kpi - D;
	
	for (i=0; i<3; i++) {
		if (filler > 0)
			memset(tmp_buf, 0xFF, filler * sizeof(char));
		
		memcpy(&tmp_buf[filler], in, D * sizeof(char));
		
		cir_buf_ptr = &circular_buf[i*kpi];
		for (c=0; c<COLUMN_WIDTH; c++) {
			pc = permutate_pattern_conv[c];
			for (r=0; r<row; r++)
				cir_buf_ptr[c*row+r] = tmp_buf[r*COLUMN_WIDTH+pc];
		}
	}
	
	/* bit collection, selection and transmission */
	for (i=0,j=0; i<E; i++) {
		if (circular_buf[j] != -1) {
			*out++ = circular_buf[j];
			if (++j == kpi) j = 0;
		}
	}
}

void turbo_ratematch(char *in, int D, char *out, ratematch_param *param)
{
	float t;
	int row, kpi, filler;
	int i, j, r, c, pc, k0;
	char *cir_buf_ptr;
	
	t = (float)D / COLUMN_WIDTH;
	row = (int)t;
	if (row != t) row++;
	kpi = row * COLUMN_WIDTH;
	assert(kpi <= MAX_K_PI);
	filler = kpi - D;
	
	/* d0 and d1 use block permutation */
	for (i=0; i<2; i++) {
		if (filler > 0)
			memset(tmp_buf, 0xFF, filler * sizeof(char));
		
		memcpy(&tmp_buf[filler], in, D * sizeof(char));
		
		for (c=0; c<COLUMN_WIDTH; c++) {
			pc = permutate_pattern_turbo[c];
			for (r=0; r<row; r++)
				interleave_buf[i][c*row+r] = tmp_buf[r*COLUMN_WIDTH+pc];
		}
	}
	
	/* d2 is special */
	if (filler > 0)
		memset(tmp_buf, 0xFF, filler * sizeof(char));
	
	memcpy(&tmp_buf[filler], in, D * sizeof(char));
	
	for (j=0; j<kpi; j++) {
		c = j / row;
		r = j - c*row; /* j % row */
		pik = (permutate_pattern_turbo[c] + r*COLUMN_WIDTH + 1) % kpi;
		interleave_buf[2][j] = tmp_buf[pik];
	}
	
	/* bit collection, selection and transmission */
	memcpy(circular_buf, interleave_buf[0], kpi * sizeof(char));
	
	for (i=0; i<kpi; i++) {
		circular_buf[kpi + 2*i]     = interleave_buf[1][i];
		circular_buf[kpi + 2*i + 1] = interleave_buf[2][i];
	}
	
	t = (float)(param->Ncb) / row;
	c = (int)t;
	if (t != c) c++;
	k0 = row * (2 * c * param->rv + 2);
	
	for (i=0,j=k0; i<E; i++) {
		if (circular_buf[j] != -1) {
			*out++ = circular_buf[j];
			if (++j == param->Ncb) j = 0;
		}
	}
}