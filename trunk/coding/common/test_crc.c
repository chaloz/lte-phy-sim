#include <stdio.h>
#include "crc.h"

int test_crc24a()
{
	char input_byte[2] = {0xF2, 0x3B};
	char output_ref[5] = {0xF2, 0x3B, 0xF8, 0x23, 0x56};
	
	char io_bit[40];
	char output_byte[2];
	int i;
	
	unpack(input_byte, 2, io_bit);
	calc_crc(io_bit, 16, CRC_POLY_24A);
	pack(io_bit, 40, output_byte);
	
	for (i=0; i<2; i++) {
		if (output_byte[i] != output_ref[i])
			return -1;
	}
	
	return 0;
}

void main()
{
	int result;
	
	result = test_crc24a();
	
	if (result == 0)
		printf("test_crc24a PASSED!\n");
	else
		printf("test_crc24a FAILED!\n");
}