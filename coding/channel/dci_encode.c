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


void dci_encode(char *tbptr, int A, int rnti, int antport, char *out, int E)
{
	char *cbptr = &scratch_bitbuffer1[0];
	char *ccptr = &scratch_bitbuffer2[0];
	int B, D;
	
	/* transport block CRC attachment */
	memcpy(cbptr, tbptr, A * sizeof(char));
	B = calc_crc(cbptr, A, CRC_POLY_16);
	
	for (i=0; i<15; i++) {
		cbptr[A+i] = ( cbptr[A+i] + (rnti>>(31-i)) ) & 0x1;
	}
	cbptr[A+i] = ( cbptr[A+i] + (rnti>>(31-i)) + antport ) & 0x1;
	
	/* channel coding */
	D = conv_encode(cbptr, B, ccptr);
	
	/* rate matching */
	conv_ratematch(ccptr, D, out, E);
}