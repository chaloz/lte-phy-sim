#include <math.h>
#include <assert.h>
#include "complex.h"
#include "fftw3.h"

/* ========================================================================= */
/*              LOCAL VARIABLES DEFINITION                                   */
/* ========================================================================= */
static fftwf_complex *mod_in, *mod_out;
static fftwf_plan mod_p;
static fftwf_complex *dem_in, *dem_out;
static fftwf_plan dem_p;

void ofdm_mod_init(int N)
{
	mod_in  = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * N);
	mod_out = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * N);
	mod_p   = fftwf_plan_dft_1d(N, mod_in, mod_out, FFTW_FORWARD, FFTW_ESTIMATE);
}

void ofdm_mod_clean()
{
	fftwf_destroy_plan(mod_p);
	fftwf_free(mod_in);
	fftwf_free(mod_out);
}

void ofdm_mod(complex *a, int len, int fftsize, int cpsize, complex *s)
{
	int half_len;
	int i;
	float scale = 1.0/sqrt(fftsize);
	
	assert((len&0x1) == 0);
	assert(len < fftsize);
	assert(sizeof(complex) == sizeof(fftwf_complex));
	
	half_len = len >> 1;
	memset(mod_in, 0, fftsize * sizeof(complex));
	/* copy the negative frequency part */
	memcpy(&mod_in[fftsize-half_len], &a[0], half_len * sizeof(complex));
	/* copy the positive frequency part */
	memcpy(&mod_in[1], &a[half_len], half_len * sizeof(complex));
	
	fftwf_execute(mod_p);
	
	/* normalize the fft output */
	for (i=0; i<fftsize; i++) {
		/* s[cpsize+i] = mod_out[i] * scale; */
		COMPLEX_SCALE(mod_out[i], scale, s[cpsize+i]);
	}
	
	/* insert CP */
	for (i=0; i<cpsize; i++) {
		/* s[i] = s[fftsize+i]; */
		COMPLEX_ASSIGN(s[fftsize+i], s[i]);
	}
}

void ofdm_dem_init(int N)
{
	dem_in  = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * N);
	dem_out = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * N);
	dem_p   = fftwf_plan_dft_1d(N, dem_in, dem_out, FFTW_BACKWARD, FFTW_ESTIMATE);
}

void ofdm_dem_clean()
{
	fftwf_destroy_plan(dem_p);
	fftwf_free(dem_in);
	fftwf_free(dem_out);
}

void ofdm_dem(complex *s, int fftsize, complex *a, int len)
{
	int half_len;
	int i;
	float scale = 1/sqrt(fftsize);
	
	assert((len&0x1) == 0);
	assert(len < fftsize);
	assert(sizeof(complex) == sizeof(fftwf_complex));
	
	/* IFFT */
	memcpy(dem_in, s, fftsize * sizeof(complex));
	fftwf_execute(dem_p);
	
	half_len = len >> 1;
	
	/* copy the negative part and normalize the ifft output */
	for (i=0; i<half_len; i++) {
		/* a[i] = mod_out[fftsize-half_len+i] * scale */
		COMPLEX_SCALE(dem_out[fftsize-half_len+i], scale, a[i]);
	}
	
	/* copy the positive part and normalize the ifft output */
	for (i=0; i<half_len; i++) {
		/* a[half_len+i] = mod_out[1+i] * scale */
		COMPLEX_SCALE(dem_out[1+i], scale, a[half_len+i]);
	}
}