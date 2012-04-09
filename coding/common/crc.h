#ifndef _CRC_H_
#define _CRC_H_

#define CRC_POLY_24A 0
#define CRC_POLY_24B 1
#define CRC_POLY_16  2
#define CRC_POLY_8   3

/**
 * @brief CRC calculation
 *
 * Calculate the CRC according to 5.1.1, 36.212
 * 
 * @param inout - input & output buffer for bit stream
 * @param len   - the length of input bit stream
 * @param type  - the CRC generation polynomial type
 * @return len  - the output length
 */
int calc_crc(char *inout, int len, int type);

#endif //_CRC_H_