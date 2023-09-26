

#include <Arduino.h>
#include <assert.h> 
#include "crc32.h"
#include "xoshiro256.hpp"
#include "utils.hpp"
#include <vector>
#include <array>
#include <string>
static uint8_t entropy_state[256];
#define MAX_BUF_LENGTH 256
void setup(){
  Serial.begin(115200);
  while(!Serial){
    ; // wait for serial port to open
  }
    uint32_t i = 1;
    char *c = (char*)&i;

    if (*c){
     Serial.println("Little-Endian");
    }    
       
    else{
        Serial.println("Big-Endian");
    }

 test_crc32();
 test_rng_1();
 test_rng_2();
 test_rng_3();
}

void loop(){
 
  
}




static bool _test_crc32(const std::string& input, const std::string& expected_hex) {
    auto checksum =ur::crc32_bytes(ur::string_to_bytes(input));
    auto hex = ur::data_to_hex(checksum);
    return hex == expected_hex;
}

static void test_crc32() {
    if (!_test_crc32("Hello, world!", "ebe6c6e6")) {
        Serial.println("test_crc32 'Hello, world!' failed.");
    } else {
        Serial.println("test_crc32 'Hello, world!' passed");
    }

    if (!_test_crc32("Wolf", "598c84dc")) {
        Serial.println("test_crc32 'Wolf' failed.");
    } else {
        Serial.println("test_crc32 'Wolf' passed");
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




void test_rng_1() {
  ur::Xoshiro256 rng("Wolf");
  uint64_t numbers[100];
  for (int i = 0; i < 100; i++) {
    numbers[i] = rng.next() % 100;
  }
  uint64_t expected_numbers[100] = {
    42, 81, 85, 8, 82, 84, 76, 73, 70, 88, 2, 74, 40, 48, 77, 54,
    88, 7, 5, 88, 37, 25, 82, 13, 69, 59, 30, 39, 11, 82, 19, 99,
    45, 87, 30, 15, 32, 22, 89, 44, 92, 77, 29, 78, 4, 92, 44, 68,
    92, 69, 1, 42, 89, 50, 37, 84, 63, 34, 32, 3, 17, 62, 40, 98,
    82, 89, 24, 43, 85, 39, 15, 3, 99, 29, 20, 42, 27, 10, 85, 66,
    50, 35, 69, 70, 70, 74, 30, 13, 72, 54, 11, 5, 70, 55, 91, 52,
    10, 43, 43, 52
  };
  for (int i = 0; i < 100; i++) {
    if (numbers[i] != expected_numbers[i]) {
      Serial.println("test_rng_1 failed!");
      return;
    }
  }
  Serial.println("test_rng_1 passed!");
}


void test_rng_2() {
 auto checksum = ur::bytes_to_int(ur::crc32_bytes(ur::string_to_bytes("Wolf")));
  ur::Xoshiro256 rng(checksum); // Create a Xoshiro256 object.
  ur::ByteVector v1 = ur::string_to_bytes("Wolf");
 ur::ByteVector v11 = ur::crc32_bytes(ur::string_to_bytes("Wolf"));
  for(unsigned char c : v1) {
    Serial.print(c, HEX); // Prints each byte in hexadecimal format
    Serial.print(' '); // Optionally print a space between bytes
  }
  Serial.println();
 // Serial.println(ur::crc32_bytes(ur::string_to_bytes("Wolf")));
Serial.println(checksum);
  std::vector<uint64_t> numbers;
  numbers.reserve(100); // Optional: Reserve space in advance, as we know the size.
  for(int i = 0; i < 100; i++) {
    numbers.push_back(rng.next() % 100);
  }
  
  std::vector<uint64_t> expected_numbers = {
    88, 44, 94, 74, 0, 99, 7, 77, 68, 35, 47, 78, 19, 21, 50, 15, 42,
    36, 91, 11, 85, 39, 64, 22, 57, 11, 25, 12, 1, 91, 17, 75, 29, 47,
    88, 11, 68, 58, 27, 65, 21, 54, 47, 54, 73, 83, 23, 58, 75, 27, 26,
    15, 60, 36, 30, 21, 55, 57, 77, 76, 75, 47, 53, 76, 9, 91, 14, 69,
    3, 95, 11, 73, 20, 99, 68, 61, 3, 98, 36, 98, 56, 65, 14, 80, 74,
    57, 63, 68, 51, 56, 24, 39, 53, 80, 57, 51, 81, 3, 1, 30
  };
//  for(const auto& number : numbers) {
//     Serial.println(number);
// }

  if(numbers == expected_numbers) {
    Serial.println("test_rng_2 Passed!");
  } else {
    Serial.println("test_rng_2 Failed!");
  }
}


void test_rng_3() {
    ur::Xoshiro256 rng("Wolf");
    std::vector<uint64_t> numbers;
    
    for(int i = 0; i < 100; i++) {
        numbers.push_back(rng.next_int(1, 10));
    }

    std::vector<uint64_t> expected_numbers = {6, 5, 8, 4, 10, 5, 7, 10, 4, 9, 10, 9, 7, 7, 1, 1, 2, 9, 9, 2, 6, 4, 5, 7, 8, 5, 4, 2, 3, 8, 7, 4, 5, 1, 10, 9, 3, 10, 2, 6, 8, 5, 7, 9, 3, 1, 5, 2, 7, 1, 4, 4, 4, 4, 9, 4, 5, 5, 6, 9, 5, 1, 2, 8, 3, 3, 2, 8, 4, 3, 2, 1, 10, 8, 9, 3, 10, 8, 5, 5, 6, 7, 10, 5, 8, 9, 4, 6, 4, 2, 10, 2, 1, 7, 9, 6, 7, 4, 2, 5};
    
    bool is_equal = numbers == expected_numbers;
    if(is_equal){
        Serial.println("test_rng_3 Passed!");
    } else {
        Serial.println("test_rng_3 Failed!");
    }
}