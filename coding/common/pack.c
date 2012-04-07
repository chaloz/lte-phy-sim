
void pack(char *in, int len, char *out)
{
	int i, j;
	char result;
	
	for (i=0; i<(len>>3); i++) {
		result = 0;
		for (j=7; j>=0; j--) {
			result = result | (*in++ << j);
		}
		
		*out++ = result;
	}
}

void unpack(char *in, int len, char *out)
{
	int i, j;
	char input;
	
	for (i=0; i<len; i++) {
		input = *in++;
		for (j=7; j>=0; j--) {
			*out++ = (input>>j) & 0x1;
		}
	}
}