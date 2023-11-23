#ifndef __BASE_CONVERTER_H
#define __BASE_CONVERTER_H

#include "typedefs.h"

#ifdef __cplusplus
extern "C"{
#endif
void hex2Decimal(char *data, int length);
char hexToDecimal(char decimal);
char decimal2Hex(char decimal);
void decimalToHex(char* data, int length);
#ifdef __cplusplus
}
#endif
#endif