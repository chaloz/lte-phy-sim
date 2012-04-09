#ifndef _MODULATION_H_
#define _MODULATION_H_

#include "complex.h"

#define MOD_BPSK  0
#define MOD_QPSK  1
#define MOD_QAM16 2
#define MOD_QAM64 3

/**
 * @brief Modulation mapper
 *
 * Modulation mapper, according to 7.1, 36.211
 * @param in  - input binary bits
 * @param len - length of input bits
 * @param out - output modulated symbols
 * @param type - modulation type
 * @return number of modulated symbols
 */
int modulation_map(char *in, int len, complex *out, int type);

#endif //_MODULATION_H_