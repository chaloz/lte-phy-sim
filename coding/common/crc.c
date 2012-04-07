#include "crc.h"

/* ========================================================================= */
/*              LOCAL FUNCTIONS DECLARATION                                  */
/* ========================================================================= */
void calc_crc24a(char *inout, int len);
void calc_crc24b(char *inout, int len);
void calc_crc16(char *inout, int len);
void calc_crc8(char *inout, int len);

/**
 * @brief CRC calculation
 *
 * Calculate the CRC according to 5.1.1, 36.212
 * 
 * @param inout - input & output buffer for bit stream
 * @param len   - the length of input bit stream
 * @param type  - the CRC generation polynomial type
 */
void calc_crc(char *inout, int len, int type)
{
	if (type == CRC_POLY_24A)
		calc_crc24a(inout, len);
	else if (type == CRC_POLY_24B)
		calc_crc24b(inout, len);
	else if (type == CRC_POLY_16)
		calc_crc16(inout, len);
	else if (type == CRC_POLY_8)
		calc_crc8(inout, len);
	else
		exit(-1);
}

/**
 * @brief CRC24A calculation
 *
 * Calculate the CRC according to g_CRC24A(D) = 
 *     D24 + D23 + D18 + D17 + D14 + D11 + D10 + D7 + D6 + D5 + D4 + D3 + D + 1
 * 
 * @param inout - input & output buffer for bit stream
 * @param len   - the length of input bit stream
 */
static void calc_crc24a(char *inout, int len)
{
	int i;
	int status = 0;
	int output, input;
	
	for (i=0; i<len; i++) {
		output = (status>>23) + (status>>18) + (status>>17) + (status>>14) +
			(status>>11) + (status>>10) + (status>>7) + (status>>6) +
			(status>>5) + (status>>4) + (status>>3) + (status>>1) + status;
		
		input = (*inout++ + output) & 0x1;
		
		status = status >> 1;
		status = status | (input<<23);
	}
	
	for (i=0; i<24; i++) {
		output = (status>>23) + (status>>18) + (status>>17) + (status>>14) +
			(status>>11) + (status>>10) + (status>>7) + (status>>6) +
			(status>>5) + (status>>4) + (status>>3) + (status>>1) + status;
		
		input = (output) & 0x1;
		
		status = status >> 1;
		status = status | (input<<23);
		
		*inout++ = input;
	}
}

/**
 * @brief CRC24B calculation
 *
 * Calculate the CRC according to g_CRC24B(D) = 
 *     D24 + D23 + D6 + D5 + D + 1
 * 
 * @param inout - input & output buffer for bit stream
 * @param len   - the length of input bit stream
 */
static void calc_crc24b(char *inout, int len)
{
	int i;
	int status = 0;
	int output, input;
	
	for (i=0; i<len; i++) {
		output = (status>>23) + (status>>6) + (status>>5) + (status>>1) + status;
		
		input = (*inout++ + output) & 0x1;
		
		status = status >> 1;
		status = status | (input<<23);
	}
	
	for (i=0; i<24; i++) {
		output = (status>>23) + (status>>6) + (status>>5) + (status>>1) + status;
		
		input = (output) & 0x1;
		
		status = status >> 1;
		status = status | (input<<23);
		
		*inout++ = input;
	}
}

/**
 * @brief CRC16 calculation
 *
 * Calculate the CRC according to g_CRC16(D) = 
 *     D16 + D12 + D5 + 1
 * 
 * @param inout - input & output buffer for bit stream
 * @param len   - the length of input bit stream
 */
static void calc_crc16(char *inout, int len)
{
	int i;
	int status = 0;
	int output, input;
	
	for (i=0; i<len; i++) {
		output = (status>>12) + (status>>5) + status;
		
		input = (*inout++ + output) & 0x1;
		
		status = status >> 1;
		status = status | (input<<15);
	}
	
	for (i=0; i<16; i++) {
		output = (status>>12) + (status>>5) + status;
		
		input = (output) & 0x1;
		
		status = status >> 1;
		status = status | (input<<15);
		
		*inout++ = input;
	}
}

/**
 * @brief CRC8 calculation
 *
 * Calculate the CRC according to g_CRC8(D) = 
 *     D8 + D7 + D4 + D3 + D + 1
 * 
 * @param inout - input & output buffer for bit stream
 * @param len   - the length of input bit stream
 */
static void calc_crc8(char *inout, int len)
{
	int i;
	int status = 0;
	int output, input;
	
	for (i=0; i<len; i++) {
		output = (status>>7) + (status>>4) + (status>>3) + (status>>1) + status;
		
		input = (*inout++ + output) & 0x1;
		
		status = status >> 1;
		status = status | (input<<7);
	}
	
	for (i=0; i<16; i++) {
		output = (status>>7) + (status>>4) + (status>>3) + (status>>1) + status;
		
		input = (output) & 0x1;
		
		status = status >> 1;
		status = status | (input<<7);
		
		*inout++ = input;
	}
}

