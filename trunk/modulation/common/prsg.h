#ifndef _PRSG_H_
#define _PRSG_H_

typedef struct prsg_status_s
{
	int x1;
	int x2;
} prsg_status;

/**
 * @brief Initialize the pseudo-random sequence generator
 *
 * Pseudo-random sequence generation, 7.2, 36.211
 * @param status - the register status
 * @param cinit  - c_init, used to initialize the second m-sequence
 *
 */
void prsg_init(prsg_status *status, int cinit);

/**
 * @brief Generate the sequence
 *
 * Generate the pseudo-random sequence, run-time
 * @param status - the register status
 * @param steps  - the length of output sequence, M_PN
 * @param out    - output sequence c(n)
 *
 */
void prsg_run(prsg_status *status, int steps, char *out);

#endif //_PRSG_H_