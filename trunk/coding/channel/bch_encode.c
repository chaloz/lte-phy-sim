#include "pack.h"
#include "crc.h"
#include "coding.h"
#include "ratematch.h"
#include "errcode.h"

/* ========================================================================= */
/*              GLOBAL VARIABLES DECLARATION                                 */
/* ========================================================================= */
extern char scratch_bitbuffer1[];
extern char scratch_bitbuffer2[];

/* ========================================================================= */
/*              LOCAL VARIABLES DEFINITION                                   */
/* ========================================================================= */

/* Table 5.3.1.1-1: CRC mask for PBCH. */
static char pbch_crc_mask[3][16] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}
};

void bch_encode(char *tbptr, int A, int tx_ant, char *out, int E)
{
	char *cbptr = &scratch_bitbuffer1[0];
	char *ccptr = &scratch_bitbuffer2[0];
	int B, D, ant_index;
	
	/* find the antenna index for Table 5.3.1.1-1 */
	if (tx_ant == 1)
		ant_index = 0;
	else if (tx_ant == 2);
		ant_index = 1;
	else if (tx_ant == 4)
		ant_index = 2;
	else
		exit(NUM_TRANSMIT_ANTENNA_ERROR);
	
	/* transport block CRC attachment */
	assert(A == 24);
	memcpy(cbptr, tbptr, A * sizeof(char));
	B = calc_crc(cbptr, A, CRC_POLY_16);
	
	for (i=0; i<16; i++) {
		cbptr[A+i] = (cbptr[A+i] + pbch_crc_mask[ant_index][i]) & 0x1;
	}
	
	/* channel coding */
	D = conv_encode(cbptr, B, ccptr);
	
	/* rate matching */
	conv_ratematch(ccptr, D, out, E);
}