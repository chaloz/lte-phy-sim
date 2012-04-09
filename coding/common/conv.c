
int conv_encode(char *in, int K, char *out)
{
	char reg = 0;
	char output, *d0, *d1, *d2;
	int i;
	
	/* tail biting */
	for (i=0; i<6; i++) {
		reg = reg | (in[K-6+i] << i);
	}
	
	/* tail biting convolutional coding with rate 1/3: D = K; */
	d0 = out;
	d1 = d0 + K;
	d2 = d1 + K;
	for (i=0; i<K; i++) {
		/* G0 = 1 011 011 */
		output = *in + (reg>>4) + (reg>>3) + (reg>>1) + reg;
		*d0++ = output & 0x1;
		/* G1 = 1 111 001 */
		output = *in + (reg>>5) + (reg>>4) + (reg>>3) + reg;
		*d1++ = output & 0x1;
		/* G2 = 1 110 101 */
		output = *in + (reg>>5) + (reg>>4) + (reg>>2) + reg;
		*d2++ = output & 0x1;
		
		reg = reg >> 1;
		reg = reg | (*in<<5);
		
		in++;
	}
	
	return K;
}
