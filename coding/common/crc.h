#ifndef _CRC_H_
#define _CRC_H_

#define CRC_POLY_24A 0
#define CRC_POLY_24B 1
#define CRC_POLY_16  2
#define CRC_POLY_8   3

void calc_crc(char *inout, int len, int type);

#endif //_CRC_H_