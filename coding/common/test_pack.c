#include <stdio.h>
#include "pack.h"

int test_packing()
{
	char byte_stream[5] = {0x5A, 0x7F, 0xFF, 0x0, 0x43};
	char bit_stream[40];
	char byte_output[5];
	int i;
	
	unpack(byte_stream, 5, bit_stream);
	pack(bit_stream, 40, byte_output);
	
	for (i=0; i<5; i++) {
		if (byte_stream[i] != byte_output[i])
			return -1;
	}
	
	return 0;
}

void main()
{
	int result;
	
	result = test_packing();
	
	if (result == 0)
		printf("test_packing PASSED!\n");
	else
		printf("test_packing FAILED!\n");
}