#include "coding.h"

/* ========================================================================= */
/*              LOCAL VARIABLES DEFINITION                                   */
/* ========================================================================= */
static char bit_buffer[6144];

static int f1_table[] = {
	3, 7, 19, 7, 7, 11, 5, 11, 7, 41, 103, 15, 9, 17, 9, 21, 101, 21, 57, 23, 13, 27, 11, 27, 
	85, 29, 33, 15, 17, 33, 103, 19, 19, 37, 19, 21, 21, 115, 193, 21, 133, 81, 45, 23, 243, 
	151, 155, 25, 51, 47, 91, 29, 29, 247, 29, 89, 91, 157, 55, 31, 17, 35, 227, 65, 19, 37, 
	41, 39, 185, 43, 21, 155, 79, 139, 23, 217, 25, 17, 127, 25, 239, 17, 137, 215, 29, 15, 
	147, 29, 59, 65, 55, 31, 17, 171, 67, 35, 19, 39, 19, 199, 21, 211, 21, 43, 149, 45, 49, 
	71, 13, 17, 25, 183, 55, 127, 27, 29, 29, 57, 45, 31, 59, 185, 113, 31, 17, 171, 209, 253, 
	367, 265, 181, 39, 27, 127, 143, 43, 29, 45, 157, 47, 13, 111, 443, 51, 51, 451, 257, 57, 
	313, 271, 179, 331, 363, 375, 127, 31, 33, 43, 33, 477, 35, 233, 357, 337, 37, 71, 71, 37, 
	39, 127, 39, 39, 31, 113, 41, 251, 43, 21, 43, 45, 45, 161, 89, 323, 47, 23, 47, 263
};

static int f2_table[] = {
	10, 12, 42, 16, 18, 20, 22, 24, 26, 84, 90, 32, 34, 108, 38, 120, 84, 44, 46, 48, 50, 52, 
	36, 56, 58, 60, 62, 32, 198, 68, 210, 36, 74, 76, 78, 120, 82, 84, 86, 44, 90, 46, 94, 48, 
	98, 40, 102, 52, 106, 72, 110, 168, 114, 58, 118, 180, 122, 62, 84, 64, 66, 68, 420, 96, 74, 
	76, 234, 80, 82, 252, 86, 44, 120, 92, 94, 48, 98, 80, 102, 52, 106, 48, 110, 112, 114, 58, 
	118, 60, 122, 124, 84, 64, 66, 204, 140, 72, 74, 76, 78, 240, 82, 252, 86, 88, 60, 92, 846, 
	48, 28, 80, 102, 104, 954, 96, 110, 112, 114, 116, 354, 120, 610, 124, 420, 64, 66, 136, 420, 
	216, 444, 456, 468, 80, 164, 504, 172, 88, 300, 92, 188, 96, 28, 240, 204, 104, 212, 192, 220, 
	336, 228, 232, 236, 120, 244, 248, 168, 64, 130, 264, 134, 408, 138, 280, 142, 480, 146, 444, 
	120, 152, 462, 234, 158, 80, 96, 902, 166, 336, 170, 86, 174, 176, 178, 120, 182, 184, 186, 94, 
	190, 480
};

static void turbo_interleave(char *in, int len, int index, char *out)
{
	int f1, f2;
	int i, j;
	
	f1 = f1_table[index];
	f2 = f2_table[index];
	
	for (i=0; i<len; i++) {
		j = (f1*i + f2*i*i) % len;
		*out++ = in[j];
	}
}

static void turbo_encode_p(char *in1, char *in2, int len, char *out)
{
	char *d0, *d1, *d2;
	char R1 = 0, R2 = 0;
	char fb1, fb2;
	char out1, out2;
	char fb1_t[3], fb2_t[3];
	char out1_t[3], out2_t[3];
	int i;
	
	/* for turbo coding, D = K+4 */
	d0 = out;
	d1 = d0 + len + 4;
	d2 = d1 + len + 4;
	
	memcpy(d0, in1, len * sizeof(char));
	d0 += len;
	
	for (i=0; i<len; i++) {
		fb1 = ((R1>>1) + R1 + *in1++) & 0x1;
		out1 = (fb1 + (R1>>2) + R1) & 0x1;
		
		fb2 = ((R2>>1) + R2 + *in2++) & 0x1;
		out2 = (fb2 + (R2>>2) + R2) & 0x1;
		
		*d1++ = out1;
		*d2++ = out2;
	}
	
	/* termination */
	for (i=0; i<3; i++) {
		fb1_t[i] = ((R1>>1) + R1) & 0x1;
		out1_t[i] = (fb1_t[i] + (R1>>2) + R1) & 0x1;
		
		fb2_t[i] = ((R2>>1) + R2) & 0x1;
		out2_t[i] = (fb2_t[i] + (R2>>2) + R2) & 0x1;
	}
	
	*d0++ = fb1_t[0];
	*d1++ = out1_t[0];
	*d2++ = fb1_t[1];
	*d0++ = out1_t[1];
	*d1++ = fb1_t[2];
	*d2++ = out1_t[2];
	*d0++ = fb2_t[0];
	*d1++ = out2_t[0];
	*d2++ = fb2_t[1];
	*d0++ = out2_t[1];
	*d1++ = fb2_t[2];
	*d2++ = out2_t[2];
}

void turbo_encode(char *in, codeblock_param *param, int cbindex, char *out)
{
	int K;
	
	K = (cbindex<param->C_minus)?(param->K_minus):(param->K_plus);
	turbo_interleave(in, K, param->index, bit_buffer);
	turbo_encode_p(in, bit_buffer, K, out);
	
	/* If the code block to be encoded is the 0-th code block and 
	 * the number of filler bits is greater than zero, i.e., F > 0, 
	 * then the encoder shall set ck, = 0, k = 0,…,(F-1) at its input 
	 * and shall set dk(0)=<NULL>, k = 0,…,(F-1) and dk(1)=<NULL>, k = 0,…,(F-1) at its output.
	 */
	if ( (cbindex == 0) && (param->F > 0) ) {
		memset(out, 0xFF, param->F * sizeof(char));
		memset(out+K+4, 0xFF, param->F * sizeof(char));
	}
}