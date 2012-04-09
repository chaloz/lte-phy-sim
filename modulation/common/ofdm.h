#ifndef _OFDM_H_
#define _OFDM_H_

#include "complex.h"

/**
 * @brief OFDM Modulation Initialization
 *
 * Initialize the OFDM FFT-related buffers
 * @param N - FFT size
 */
void ofdm_mod_init(int N);

/**
 * @brief OFDM Modulation cleaning
 *
 * Free the OFDM FFT-related buffers
 */
void ofdm_mod_clean();

/**
 * @brief OFDM Modulation execution
 *
 * OFDM baseband signal generation, according to 6.12, 36.211
 * @param a   - frequency domain signals after mapping
 * @param len - the length of input signal (N_DL_RB * N_RB_sc)
 * @param fftsize - FFT size N
 * @param cpsize  - CP length Ncp
 * @param s   - generated time domain baseband signal
 */
void ofdm_mod(complex *a, int len, int fftsize, int cpsize, complex *s);

/**
 * @brief OFDM Demodulation Initialization
 *
 * Initialize the OFDM iFFT-related buffers
 * @param N - iFFT size
 */
void ofdm_dem_init(int N);

/**
 * @brief OFDM Demodulation cleaning
 *
 * Free the OFDM iFFT-related buffers
 */
void ofdm_dem_clean();

/**
 * @brief OFDM Demodulation execution
 *
 * OFDM baseband signal receiving, according to 6.12, 36.211
 * @param s   - received time domain baseband signal
 * @param fftsize - iFFT size N
 * @param cpsize  - CP length Ncp
 * @param a   - frequency domain signals received
 * @param len - the length of RX signal (N_DL_RB * N_RB_sc)
 */
void ofdm_dem(complex *s, int fftsize, complex *a, int len);

#endif //_OFDM_H_