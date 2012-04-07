
void conv_encode(char *in, int len, char *out)
{
	char D = 0;
	char output, *d0, *d1, *d2;
	int i;
	
	/* tail biting */
	for (i=0; i<6; i++) {
		D = D | (in[len-6+i] << i);
	}
	
	/* for convolutional coding, D = K */
	d0 = out;
	d1 = d0 + len;
	d2 = d1 + len;
	for (i=0; i<len; i++) {
		/* G0 = 1 011 011 */
		output = *in + (D>>4) + (D>>3) + (D>>1) + D;
		*d0++ = output & 0x1;
		/* G1 = 1 111 001 */
		output = *in + (D>>5) + (D>>4) + (D>>3) + D;
		*d1++ = output & 0x1;
		/* G2 = 1 110 101 */
		output = *in + (D>>5) + (D>>4) + (D>>2) + D;
		*d2++ = output & 0x1;
		
		D = D >> 1;
		D = D | (*in<<5);
		
		in++;
	}
}
