 

#include <Arduino.h>
#include <assert.h> 
#include "crc32.h"
#include "xoshiro256.hpp"
#include "utils.hpp"
#include "random-sampler.hpp"
#include "fountain-encoder.hpp"
#include "fountain-utils.hpp"
#include "test-utils.hpp"
#include <vector>
#include <array>
#include <string>
#include <assert.h>
#include <algorithm>

#include <sstream>
#include <iostream>
#include <iomanip>

static void test_psbtqr_encoder(){
  //59017F
   std::string hex = "70736274FF0100710200000001ED84DAEC9AC7339866EBDD783D2EEF73D852B19727D12CFBA452C5B4C55DACA60100000000FDFFFFFF02AE7F020000000000160014FD15359F2FB4535B53EADD5D8B22419DC8F51DB69CAD0000000000001600149715AAF35C6CABC6AA7B74A21BC3C6A5203D4463BC0D0C004F010488B21E0343B8F0F08000000085B6BA9FD5EFC1E94A2EF228A69F2A0F3227BD09E99B3F93C94AC82CBD805554029359AF864F4EED5EEAA3A98C3A3E719E806153BCFEE162E352BB3F4AAA2D6F30107E1B62C25400008000000080000000800001011F0241030000000000160014DB78D10BC82CB54A8CBE0EAB10D5B6F5FA4D92F10103040100000022060348C5CD76FEE2E91017333FFC35F27EB895D1146DA0A1DDB6C38CB69361ECF734187E1B62C25400008000000080000000800100000001000000002202030FA9F933DBDB87540324D501EC5695D7E1CFAB40E037A26FE3EE398AA5CB2F58187E1B62C254000080000000800000008001000000020000000000";

  ur::ByteVector message = hexStringToByteVector(hex);
  ur::ByteVector cbor;
  ur::CborLite::encodeBytes(cbor, message);
  auto ur =  ur::UR("crypto-psbt", cbor);
  auto encoder = ur::UREncoder(ur, 10);
  ur::StringVector parts;
 
  for(int i = 0; i < 120; i++) {
    parts.push_back(encoder.next_part());
  }
    // for (const std::string& str : parts) {
    //     Serial.println(str.c_str()); 
    // }

}

ur::ByteVector hexStringToByteVector(const std::string& hex) {
    std::vector<uint8_t> bytes;

    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::stoul(byteString, nullptr, 16));
        bytes.push_back(byte);
    }

    return bytes;
}
