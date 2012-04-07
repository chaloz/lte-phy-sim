#include <stdio.h>
#include "coding.h"

int test_conv()
{
	char input_byte[4] = {0xAC, 0xBC, 0xD2, 0x00};
	char output_byte[12] = {0x0};
	char output_ref[12]  = {0x95, 0xD3, 0x69, 0x18, 0xC6, 0xE8, 0xE6, 0x88, 0xD9, 0xB4, 0xB1, 0xE8};
	char input_bit[32];
	char output_bit[96];
	int  i;
	
	unpack(input_byte, 4, input_bit);
	conv_encode(input_bit, 32, output_bit);
	pack(output_bit, 96, output_byte);
	
	for (i=0; i<12; i++) {
		//printf("%x - %x\n", output_byte[i], output_ref[i]);
		if (output_byte[i] != output_ref[i]) {
			return -1;
		}
	}
	
	return 0;
}

void main()
{
	int result;
	
	result = test_conv();
	if (result == 0)
		printf("test_conv PASSED!\n");
	else
		printf("test_conv FAILED!\n");
	
	// test_turbo
}