#include "prsg.h"
#include "complex.h"
#include "lte_macros.h"
#include "errcode.h"


/* ========================================================================= */
/*              GLOBAL VARIABLES DECLARATION                                 */
/* ========================================================================= */
extern complex dl_resources[14][72];
extern int dl_resource_map[14][72];

/* ========================================================================= */
/*              LOCAL VARIABLES DEFINITION                                   */
/* ========================================================================= */
static char crs_random_sequence[4*MAX_NUM_DL_RB];
static prsg_status status;
static complex crs_sequence[2*MAX_NUM_DL_RB];

static int getv(int p, int l, int ns)
{
	if (p==0)
		return (l==0)?(0):(3);
	else if (p==1)
		return (l==0)?(3):(0);
	else if (p==2)
		return 3*(ns&0x1);
	else if (p==3)
		return 3*(ns&0x1) + 3;
	else
		exit(ANTENNA_PORT_ERROR);
}

void crs_generate(int ns, int cptype, int cellid, int num_rb, int num_antport)
{
	int l, m, p, mbar;
	int Nsymb, Ncp;
	int cinit;
	int k, v, v_shift;
	complex t;
	complex *dlsymbol;
	int *dlmap;
	
	Nsymb = (cptype == NORMAL_CP)?(7):(6);
	Ncp = (cptype == NORMAL_CP)?(1):(0);
	v_shift = cellid % 6;
	
	for (l=0; l<Nsymb; l++) {
		/* cell-specific RS is ONLY on symbol 0, 1, Nsymb-3 */
		if ( !((l==0) || (l==1) || (l==(Nsymb-3))) )
			continue;
			
		/* The pseudo-random sequence generator shall be initialised with */
		cinit = (7*(ns+1)+l+1) * ((2*cellid+1)<<10) + 2*cellid + Ncp;
		/* at the start of each OFDM symbol */
		prsg_init(&status, cinit);
		prsg_run(&status, 4*MAX_NUM_DL_RB, crs_random_sequence);
		
		/* reference-signal sequence */
		for (m=0; m<2*MAX_NUM_DL_RB; m++) {
			COMPLEX_NEW(t, (1-2*crs_random_sequence[2*m]), (1-2*crs_random_sequence[2*m+1]));
			COMPLEX_SCALE(t, 1/sqrt(2), crs_sequence[m]);
		}
		
		/* mapping to resource elements */
		m = ((ns&0x1)==0)?(0):(Nsymb);
		dlsymbol = &dl_resources[l+m][0];
		dlmap = &dl_resource_map[l+m][0];
		
		for (p=0; p<num_antport; p++) {
			/* p = 0, 1, except symbol 1 */
			if ( (p<2) && (l==1) ) continue;
			/* p = 2, 3, only on symbol 1 */
			if ( (p>1) && (l!=1) ) continue;
			
			v = getv(p, l, ns);
			
			for (m=0; m<2*num_rb; m++) {
				k = 6*m + (v+v_shift)%6;
				mbar = m + MAX_NUM_DL_RB - num_rb;
				COMPLEX_ASSIGN(crs_sequence[mbar], dlsymbol[k]);
				
				/* resource map */
				dlmap[k] |= (1<<(p<<3));
			}
		}
	}
}