#include <Arduino.h>

#include "BitConserve-UR.h"

#define htonl(x) __builtin_bswap32((uint32_t) (x))
static uint32_t table[256];

void generateTable() {
    for(int i = 0; i < 256; i++) {
        uint32_t c = i;
        for(int j = 0; j < 8; j++) {
            c = (c % 2 == 0) ? (c >> 1) : (0xEDB88320 ^ (c >> 1));
        }
        table[i] = c;
    }
}

uint32_t ur_crc32(const uint8_t* bytes, size_t len) {
    if(table[1] == 0) {  // if the table isn't generated yet
        generateTable();
    }

    uint32_t crc = ~0;
    for(int i = 0; i < len; i++) {
        uint32_t byte = bytes[i];
        crc = (crc >> 8) ^ table[(crc ^ byte) & 0xFF];
    }
    return ~crc;
}

uint32_t ur_crc32n(const uint8_t* bytes, size_t len) {
    return ur_crc32(bytes, len);
    //return htonl(ur_crc32(bytes, len));
}
