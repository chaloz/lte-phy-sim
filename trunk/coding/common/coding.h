#ifndef _CODING_H_
#define _CODING_H_

#include "codeblock.h"

/**
 * @brief Tail biting convolutional coding
 *
 * Convolutional encoding according to 5.1.3.1, 36.212
 * 
 * @param in    - input code block
 * @param K     - length of input code block
 * @param out   - output encoded code block
 * @return len  - length of encoded code block, the output length is 3x.
 */
int conv_encode(char *in, int K, char *out);

/**
 * @brief Turbo coding
 *
 * Turbo encoding according to 5.1.3.2, 36.212
 * 
 * @param in    - input code block
 * @param param - turbo code block parameters
 * @param cbind - index of input code block in the transport block
 * @param out   - output encoded code block
 * @return len  - length of encoded code block, the output length is 3x.
 */
int turbo_encode(char *in, codeblock_param *param, int cbindex, char *out);

#endif //_CODING_H_