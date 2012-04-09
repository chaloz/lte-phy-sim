#include <stdio.h>
#include "ofdm.h"

#define N_DATA 48
#define N_FFT  64
#define N_CP   16

int test_ofdm()
{
	complex in[N_DATA];
	complex out[N_FFT+N_CP];
	complex ref[N_DATA];
	int i;
	
	for (i=0; i<N_DATA; i++) {
		COMPLEX_NEW(in[i], i, -i);
	}
	
	ofdm_mod_init(N_FFT);
	ofdm_dem_init(N_FFT);
	
	ofdm_mod(in, N_DATA, N_FFT, N_CP, out);
	ofdm_dem(&out[N_CP], N_FFT, ref, N_DATA);
	
	ofdm_mod_clean();
	ofdm_dem_clean();
	
	for (i=0; i<N_DATA; i++) {
		//printf("(%f, %f) - (%f, %f)\n", in[i][0], in[i][1], ref[i][1], ref[i][1]);
		if ( !(COMPLEX_EQUAL(in[i], ref[i])) )
			return -1;
	}
	
	return 0;
}

void main()
{
	int result;
	
	result = test_ofdm();
	
	if (result == 0)
		printf("test_ofdm PASSED!\n");
	else
		printf("test_ofdm FAILED!\n");
}