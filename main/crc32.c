

#include "crc32.h"

#define htonl(x) __builtin_bswap32((uint32_t) (x))

#include <stdlib.h> 


uint32_t ur_crc32(const uint8_t* bytes, size_t len) {
    static uint32_t* table = NULL;

    if(table == NULL) {
        table = malloc(256 * sizeof(uint32_t));
        for(int i = 0; i < 256; i++) {
            uint32_t c = i;
            for(int j = 0; j < 8; j++) {
                c = (c % 2 == 0) ? (c >> 1) : (0xEDB88320 ^ (c >> 1));
            }
            table[i] = c;
        }
    }

    uint32_t crc = ~0;
    for(int i = 0; i < len; i++) {
        uint32_t byte = bytes[i];
        crc = (crc >> 8) ^ table[(crc ^ byte) & 0xFF];
    }
    return ~crc;
}

uint32_t ur_crc32n(const uint8_t* bytes, size_t len) {
    // return ur_crc32(bytes, len);
    return htonl(ur_crc32(bytes, len));
}
