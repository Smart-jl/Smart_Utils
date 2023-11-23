#include <string.h>
#include <math.h>
#include "baseConverter.h"

char hexToDecimal(char hex) {
    char decimal = 0;
    int base = 1;

    while (hex != 0) {
        int remainder = hex % 10;
        decimal += remainder * base;
        hex = hex / 10;
        base = base * 16;
    }

    return decimal;
}

void hex2Decimal(char *data, int length) {
    for(int i = 0; i < length; i++){
        data[i] = hexToDecimal(data[i]);
    }
}

char decimal2Hex(char decimal){
    char hex = 0;
    int base = 1;

    while (decimal != 0) {
        int remainder = decimal % 16;
        hex += remainder * base;
        decimal = decimal / 16;
        base = base * 10;
    }

    return hex;
}

void decimalToHex(char* data, int length){
    for(int i = 0; i < length; i++){
        data[i] = decimal2Hex(data[i]);
    }
}