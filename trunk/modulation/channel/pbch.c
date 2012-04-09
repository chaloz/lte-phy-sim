#include "modulation.h"

/* ========================================================================= */
/*              GLOBAL VARIABLES DECLARATION                                 */
/* ========================================================================= */
extern char    pbch_scramble_sequence[];
extern complex pbch_layer_buffer[][];

/* ========================================================================= */
/*              LOCAL VARIABLES DEFINITION                                   */
/* ========================================================================= */
static complex pbch_mod_buffer[960];

void pbch_mod(char *in, int len, int num_antport)
{
	int i;
	int sym_num;
	
	assert( (num_antport == 1) || (num_antport == 2)  || (num_antport == 4) );
	
	/* scrambling */
	for (i=0; i<len; i++) {
		in[i] = (in[i] + pbch_scramble_sequence[i]) & 0x1;
	}
	
	/* modulation */
	sym_num = modulation_map(in, len, &pbch_mod_buffer[0], MOD_QPSK);
	
	/* layer mapping and precoding */
	if (num_antport == 1) {
		memcpy(pbch_layer_buffer[0], pbch_mod_buffer, sym_num * sizeof(complex));
	}
	
	if (num_antport == 2) {
		/*           2i     2i+1
		 * port 0:   x0      x1
		 * port 1:  -x1'     x0'
		 */
		memcpy(pbch_layer_buffer[0], pbch_mod_buffer, sym_num * sizeof(complex));
		for (i=0; i<sym_num/2; i++) {
			COMPLEX_MINUSCONJ(pbch_mod_buffer[2*i+1], pbch_layer_buffer[1][2*i]);
			COMPLEX_CONJ(pbch_mod_buffer[2*i], pbch_layer_buffer[1][2*i+1]);
		}
	}
	
	if (num_antport == 4) {
		// todo:
		assert(0);
	}
	
	/* mapping to resource elements */
	
}