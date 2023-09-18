
#include <Bitcoin.h>
#include <Hash.h>
#include <Arduino.h>
#include <assert.h> 
#include "BitConserve-UR.h"
static uint8_t entropy_state[256];
#define MAX_BUF_LENGTH 256
void setup(){
  Serial.begin(115200);
  while(!Serial){
    ; // wait for serial port to open
  }

  test_crc32();
 
}

void loop(){
 
  
}



static bool _test_crc32(const String& input, const String& expected_hex) {
    uint8_t* data = string_to_bytes(input);
    size_t data_length = input.length();
    uint8_t checksum_result[4];
    crc32_bytes(data, data_length, checksum_result);
    delete[] data;
    String checksum_str = data_to_hex(checksum_result, 4);
   
    Serial.println(checksum_str);
     return checksum_str == expected_hex;
}

static void test_crc32() {
    if (!_test_crc32("Hello, world!", "ebe6c6e6")) {
        Serial.println("Test 'Hello, world!' failed.");
    }

    if (!_test_crc32("Wolf", "598c84dc")) {
        Serial.println("Test 'Wolf' failed.");
    }
}
String data_to_hex(const uint8_t* in, size_t len) {
    const char* hex = "0123456789abcdef";
    String result = "";
    for(size_t i = 0; i < len; i++) {
        uint8_t c = in[i];
        result += hex[(c >> 4) & 0xF];
        result += hex[c & 0xF];
    }
    return result;
}
uint8_t* string_to_bytes(const String& input) {
    uint8_t* output = new uint8_t[input.length()];

    for (unsigned int i = 0; i < input.length(); i++) {
        output[i] = static_cast<uint8_t>(input.charAt(i));
    }

    return output;
} 


void crc32_bytes(const uint8_t buf[MAX_BUF_LENGTH], size_t length, uint8_t result[4]) {
    uint32_t checksum = ur_crc32n(buf, length);
    
    // Store the 32-bit checksum into a 4-byte array
    result[0] = (checksum >> 24) & 0xFF;
    result[1] = (checksum >> 16) & 0xFF;
    result[2] = (checksum >> 8) & 0xFF;
    result[3] = checksum & 0xFF;
}