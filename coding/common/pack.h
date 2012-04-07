#ifndef _PACK_H_
#define _PACK_H_

/**
 * @brief pack the bit stream into byte stream
 * @param in - input bit stream
 * @param len - the length of input bit stream
 * @param out - output byte stream
 */
void pack(char *in, int len, char *out);

/**
 * @brief unpack the byte stream into bit stream
 * @param in - input byte stream
 * @param len - the length of input byte stream
 * @param out - output bit stream
 */
void unpack(char *in, int len, char *out);

#endif //_PACK_H_
