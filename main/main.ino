

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
void setup(){
  Serial.begin(115200);
  while(!Serial){
    ; // wait for serial port to open
  }


 test_crc32();
 test_rng_1();
 test_rng_2();
 test_rng_3();
 test_xor();
 test_random_sampler();
 test_find_fragment_length();
 test_shuffle();
 test_partition_and_join();
 test_choose_degree();
 test_choose_fragments();
 test_fountain_encoder();
 test_fountain_encoder_cbor();
  test_fountain_encoder_is_complete();
 test_fountain_decoder();
 test_fountain_cbor();
 test_single_part_ur();
  test_ur_encoder();
//  test_multipart_ur();


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

void test_xor() {
    ur::Xoshiro256 rng("Wolf");
    auto data1 = rng.next_data(10);
    if(ur::data_to_hex(data1) != "916ec65cf77cadf55cd7"){
      Serial.println("xor data1 failed");
    }else{
      Serial.println("xor data1 passed");
    }

    auto data2 = rng.next_data(10);
    if(ur::data_to_hex(data2) != "f9cda1a1030026ddd42e"){
      Serial.println("xor data2 failed");
    }else{
      Serial.println("xor data2 passed");
    }

    auto data3 = data1;
    ur::xor_into(data3, data2);
    if(ur::data_to_hex(data3) != "68a367fdf47c8b2888f9"){
      Serial.println("xor data3 failed");
    }else {
      Serial.println("xor data3 passed");
    }
       
    
    ur::xor_into(data3, data1);
    if(data3 != data2){
      Serial.println("xor comparison failed");
    }else{
      Serial.println("xor comparison passed");
    }
       
}

static void test_random_sampler() {
    std::vector<double> probs = { 1, 2, 4, 8 };
    auto sampler = ur::RandomSampler(probs);
    auto rng = ur::Xoshiro256("Wolf");
    std::vector<int> samples;
    auto f = [&](){ return rng.next_double(); };
    for(int i = 0; i < 500; i++) {
        samples.push_back(sampler.next(f));
    }
    std::vector<int> expected_samples = {3, 3, 3, 3, 3, 3, 3, 0, 2, 3, 3, 3, 3, 1, 2, 2, 1, 3, 3, 2, 3, 3, 1, 1, 2, 1, 1, 3, 1, 3, 1, 2, 0, 2, 1, 0, 3, 3, 3, 1, 3, 3, 3, 3, 1, 3, 2, 3, 2, 2, 3, 3, 3, 3, 2, 3, 3, 0, 3, 3, 3, 3, 1, 2, 3, 3, 2, 2, 2, 1, 2, 2, 1, 2, 3, 1, 3, 0, 3, 2, 3, 3, 3, 3, 3, 3, 3, 3, 2, 3, 1, 3, 3, 2, 0, 2, 2, 3, 1, 1, 2, 3, 2, 3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 2, 3, 1, 2, 1, 1, 3, 1, 3, 2, 2, 3, 3, 3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 2, 3, 2, 3, 3, 1, 2, 3, 3, 1, 3, 2, 3, 3, 3, 2, 3, 1, 3, 0, 3, 2, 1, 1, 3, 1, 3, 2, 3, 3, 3, 3, 2, 0, 3, 3, 1, 3, 0, 2, 1, 3, 3, 1, 1, 3, 1, 2, 3, 3, 3, 0, 2, 3, 2, 0, 1, 3, 3, 3, 2, 2, 2, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 2, 3, 3, 2, 0, 2, 3, 3, 3, 3, 2, 1, 1, 1, 2, 1, 3, 3, 3, 2, 2, 3, 3, 1, 2, 3, 0, 3, 2, 3, 3, 3, 3, 0, 2, 2, 3, 2, 2, 3, 3, 3, 3, 1, 3, 2, 3, 3, 3, 3, 3, 2, 2, 3, 1, 3, 0, 2, 1, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3, 3, 3, 2, 2, 2, 3, 1, 1, 3, 2, 2, 0, 3, 2, 1, 2, 1, 0, 3, 3, 3, 2, 2, 3, 2, 1, 2, 0, 0, 3, 3, 2, 3, 3, 2, 3, 3, 3, 3, 3, 2, 2, 2, 3, 3, 3, 3, 3, 1, 1, 3, 2, 2, 3, 1, 1, 0, 1, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 1, 2, 3, 3, 2, 2, 2, 2, 3, 3, 2, 0, 2, 1, 3, 3, 3, 3, 0, 3, 3, 3, 3, 2, 2, 3, 1, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 3, 2, 3, 2, 1, 3, 3, 3, 3, 2, 2, 0, 1, 2, 3, 2, 0, 3, 3, 3, 3, 3, 3, 1, 3, 3, 2, 3, 2, 2, 3, 3, 3, 3, 3, 2, 2, 3, 3, 2, 2, 2, 1, 3, 3, 3, 3, 1, 2, 3, 2, 3, 3, 2, 3, 2, 3, 3, 3, 2, 3, 1, 2, 3, 2, 1, 1, 3, 3, 2, 3, 3, 2, 3, 3, 0, 0, 1, 3, 3, 2, 3, 3, 3, 3, 1, 3, 3, 0, 3, 2, 3, 3, 1, 3, 3, 3, 3, 3, 3, 3, 0, 3, 3, 2};
    assert(samples == expected_samples);
    if (samples == expected_samples) {
      Serial.println("test_random_sampler Passed");
    } else{
      Serial.println("test_random_sampler failed");
    }
}

static void test_shuffle() {
    auto rng = ur::Xoshiro256("Wolf");
    std::vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<std::vector<int>> result;
    for(int i = 0; i < 10; i++) {
        result.push_back(ur::shuffled(values, rng));
    }
    std::vector<std::vector<int>> expectedResult = {
        {6, 4, 9, 3, 10, 5, 7, 8, 1, 2},
        {10, 8, 6, 5, 1, 2, 3, 9, 7, 4},
        {6, 4, 5, 8, 9, 3, 2, 1, 7, 10},
        {7, 3, 5, 1, 10, 9, 4, 8, 2, 6},
        {8, 5, 7, 10, 2, 1, 4, 3, 9, 6},
        {4, 3, 5, 6, 10, 2, 7, 8, 9, 1},
        {5, 1, 3, 9, 4, 6, 2, 10, 7, 8},
        {2, 1, 10, 8, 9, 4, 7, 6, 3, 5},
        {6, 7, 10, 4, 8, 9, 2, 3, 1, 5},
        {10, 2, 1, 7, 9, 5, 6, 3, 4, 8}
    };
    assert(result == expectedResult);
    if (result == expectedResult) {
      Serial.println("test_shuffle Passed");
    } else{
      Serial.println("test_shuffle failed");
    }
}


static void test_find_fragment_length() {
    assert(ur::FountainEncoder::find_nominal_fragment_length(12345, 1005, 1955) == 1764);
    assert(ur::FountainEncoder::find_nominal_fragment_length(12345, 1005, 30000) == 12345);

      if(ur::FountainEncoder::find_nominal_fragment_length(12345, 1005, 1955) != 1764){
        Serial.println("Test 1 find_nominal_fragment_length failed");
       } else {
        Serial.println("Test 1 find_nominal_fragment_length passed!");
       }
    if(ur::FountainEncoder::find_nominal_fragment_length(12345, 1005, 30000) != 12345){
       Serial.println("Test 2 find_nominal_fragment_length failed");
    } else {
       Serial.println("Test 2 find_nominal_fragment_length passed!");
    }
}


static void test_partition_and_join() {
    auto message = make_message(1024);
    auto fragment_len = ur::FountainEncoder::find_nominal_fragment_length(message.size(), 10, 100);
    auto fragments = ur::FountainEncoder::partition_message(message, fragment_len);
    std::vector<std::string> fragments_hex;
    transform(fragments.begin(), fragments.end(), back_inserter(fragments_hex), [](const ur::ByteVector& b) -> std::string { return ur::data_to_hex(b); });
    std::vector<std::string> expected_fragments = {
        "916ec65cf77cadf55cd7f9cda1a1030026ddd42e905b77adc36e4f2d3ccba44f7f04f2de44f42d84c374a0e149136f25b01852545961d55f7f7a8cde6d0e2ec43f3b2dcb644a2209e8c9e34af5c4747984a5e873c9cf5f965e25ee29039f",
        "df8ca74f1c769fc07eb7ebaec46e0695aea6cbd60b3ec4bbff1b9ffe8a9e7240129377b9d3711ed38d412fbb4442256f1e6f595e0fc57fed451fb0a0101fb76b1fb1e1b88cfdfdaa946294a47de8fff173f021c0e6f65b05c0a494e50791",
        "270a0050a73ae69b6725505a2ec8a5791457c9876dd34aadd192a53aa0dc66b556c0c215c7ceb8248b717c22951e65305b56a3706e3e86eb01c803bbf915d80edcd64d4d41977fa6f78dc07eecd072aae5bc8a852397e06034dba6a0b570",
        "797c3a89b16673c94838d884923b8186ee2db5c98407cab15e13678d072b43e406ad49477c2e45e85e52ca82a94f6df7bbbe7afbed3a3a830029f29090f25217e48d1f42993a640a67916aa7480177354cc7440215ae41e4d02eae9a1912",
        "33a6d4922a792c1b7244aa879fefdb4628dc8b0923568869a983b8c661ffab9b2ed2c149e38d41fba090b94155adbed32f8b18142ff0d7de4eeef2b04adf26f2456b46775c6c20b37602df7da179e2332feba8329bbb8d727a138b4ba7a5",
        "03215eda2ef1e953d89383a382c11d3f2cad37a4ee59a91236a3e56dcf89f6ac81dd4159989c317bd649d9cbc617f73fe10033bd288c60977481a09b343d3f676070e67da757b86de27bfca74392bac2996f7822a7d8f71a489ec6180390",
        "089ea80a8fcd6526413ec6c9a339115f111d78ef21d456660aa85f790910ffa2dc58d6a5b93705caef1091474938bd312427021ad1eeafbd19e0d916ddb111fabd8dcab5ad6a6ec3a9c6973809580cb2c164e26686b5b98cfb017a337968",
        "c7daaa14ae5152a067277b1b3902677d979f8e39cc2aafb3bc06fcf69160a853e6869dcc09a11b5009f91e6b89e5b927ab1527a735660faa6012b420dd926d940d742be6a64fb01cdc0cff9faa323f02ba41436871a0eab851e7f5782d10",
        "fbefde2a7e9ae9dc1e5c2c48f74f6c824ce9ef3c89f68800d44587bedc4ab417cfb3e7447d90e1e417e6e05d30e87239d3a5d1d45993d4461e60a0192831640aa32dedde185a371ded2ae15f8a93dba8809482ce49225daadfbb0fec629e",
        "23880789bdf9ed73be57fa84d555134630e8d0f7df48349f29869a477c13ccca9cd555ac42ad7f568416c3d61959d0ed568b2b81c7771e9088ad7fd55fd4386bafbf5a528c30f107139249357368ffa980de2c76ddd9ce4191376be0e6b5",
        "170010067e2e75ebe2d2904aeb1f89d5dc98cd4a6f2faaa8be6d03354c990fd895a97feb54668473e9d942bb99e196d897e8f1b01625cf48a7b78d249bb4985c065aa8cd1402ed2ba1b6f908f63dcd84b66425df00000000000000000000"
    };
    assert(fragments_hex == expected_fragments);
    if (fragments_hex == expected_fragments) {
          Serial.println("partition passed!");
    } else {
       Serial.println("partition failed");
    }
    auto rejoined_message = ur::FountainDecoder::join_fragments(fragments, message.size());
    assert(message == rejoined_message);
    if (fragments_hex == expected_fragments) {
          Serial.println("join passed!");
    } else {
       Serial.println("join failed");
    }
}
static void test_choose_degree() {
    auto message = make_message(1024);
    auto fragment_len = ur::FountainEncoder::find_nominal_fragment_length(message.size(), 10, 100);
    auto fragments = ur::FountainEncoder::partition_message(message, fragment_len);
    std::vector<size_t> degrees;
    for(int nonce = 1; nonce <= 200; nonce++) {
        auto part_rng = ur::Xoshiro256("Wolf-" + std::to_string(nonce));
        degrees.push_back(ur::choose_degree(fragments.size(), part_rng));
    }
    std::vector<size_t> expected_degrees = {11, 3, 6, 5, 2, 1, 2, 11, 1, 3, 9, 10, 10, 4, 2, 1, 1, 2, 1, 1, 5, 2, 4, 10, 3, 2, 1, 1, 3, 11, 2, 6, 2, 9, 9, 2, 6, 7, 2, 5, 2, 4, 3, 1, 6, 11, 2, 11, 3, 1, 6, 3, 1, 4, 5, 3, 6, 1, 1, 3, 1, 2, 2, 1, 4, 5, 1, 1, 9, 1, 1, 6, 4, 1, 5, 1, 2, 2, 3, 1, 1, 5, 2, 6, 1, 7, 11, 1, 8, 1, 5, 1, 1, 2, 2, 6, 4, 10, 1, 2, 5, 5, 5, 1, 1, 4, 1, 1, 1, 3, 5, 5, 5, 1, 4, 3, 3, 5, 1, 11, 3, 2, 8, 1, 2, 1, 1, 4, 5, 2, 1, 1, 1, 5, 6, 11, 10, 7, 4, 7, 1, 5, 3, 1, 1, 9, 1, 2, 5, 5, 2, 2, 3, 10, 1, 3, 2, 3, 3, 1, 1, 2, 1, 3, 2, 2, 1, 3, 8, 4, 1, 11, 6, 3, 1, 1, 1, 1, 1, 3, 1, 2, 1, 10, 1, 1, 8, 2, 7, 1, 2, 1, 9, 2, 10, 2, 1, 3, 4, 10};
    assert(degrees == expected_degrees);
     if (degrees == expected_degrees) {
          Serial.println("choose_degree passed!");
    } else {
       Serial.println("choose_degree failed");
    }
}

static void test_choose_fragments() {
    auto message = make_message(1024);
    auto checksum = ur::crc32_int(message);
    auto fragment_len = ur::FountainEncoder::find_nominal_fragment_length(message.size(), 10, 100);
    auto fragments = ur::FountainEncoder::partition_message(message, fragment_len);
    std::vector<std::vector<size_t>> fragment_indexes;
    for(uint32_t seq_num = 1; seq_num <= 30; seq_num++) {
        auto indexes_set = ur::choose_fragments(seq_num, fragments.size(), checksum);
        auto indexes = std::vector<size_t>(indexes_set.begin(), indexes_set.end());
        sort(indexes.begin(), indexes.end());
        fragment_indexes.push_back(indexes);
    }
    std::vector<std::vector<size_t>> expected_fragment_indexes = {
        {0},
        {1},
        {2},
        {3},
        {4},
        {5},
        {6},
        {7},
        {8},
        {9},
        {10},
        {9},
        {2, 5, 6, 8, 9, 10},
        {8},
        {1, 5},
        {1},
        {0, 2, 4, 5, 8, 10},
        {5},
        {2},
        {2},
        {0, 1, 3, 4, 5, 7, 9, 10},
        {0, 1, 2, 3, 5, 6, 8, 9, 10},
        {0, 2, 4, 5, 7, 8, 9, 10},
        {3, 5},
        {4},
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        {0, 1, 3, 4, 5, 6, 7, 9, 10},
        {6},
        {5, 6},
        {7}
    };
    assert(fragment_indexes == expected_fragment_indexes);
    if (fragment_indexes == expected_fragment_indexes) {
          Serial.println("choose_fragments passed!");
    } else {
       Serial.println("choose_fragments failed");
    }
}

static void test_fountain_encoder() {
    auto message = make_message(256);
    auto encoder = ur::FountainEncoder(message, 30);
    ur::StringVector parts;
    for(int i = 0; i < 20; i++) {
        parts.push_back(encoder.next_part().description());
    }
    ur::StringVector expected_parts = {
        "seqNum:1, seqLen:9, messageLen:256, checksum:23570951, data:916ec65cf77cadf55cd7f9cda1a1030026ddd42e905b77adc36e4f2d3c",
        "seqNum:2, seqLen:9, messageLen:256, checksum:23570951, data:cba44f7f04f2de44f42d84c374a0e149136f25b01852545961d55f7f7a",
        "seqNum:3, seqLen:9, messageLen:256, checksum:23570951, data:8cde6d0e2ec43f3b2dcb644a2209e8c9e34af5c4747984a5e873c9cf5f",
        "seqNum:4, seqLen:9, messageLen:256, checksum:23570951, data:965e25ee29039fdf8ca74f1c769fc07eb7ebaec46e0695aea6cbd60b3e",
        "seqNum:5, seqLen:9, messageLen:256, checksum:23570951, data:c4bbff1b9ffe8a9e7240129377b9d3711ed38d412fbb4442256f1e6f59",
        "seqNum:6, seqLen:9, messageLen:256, checksum:23570951, data:5e0fc57fed451fb0a0101fb76b1fb1e1b88cfdfdaa946294a47de8fff1",
        "seqNum:7, seqLen:9, messageLen:256, checksum:23570951, data:73f021c0e6f65b05c0a494e50791270a0050a73ae69b6725505a2ec8a5",
        "seqNum:8, seqLen:9, messageLen:256, checksum:23570951, data:791457c9876dd34aadd192a53aa0dc66b556c0c215c7ceb8248b717c22",
        "seqNum:9, seqLen:9, messageLen:256, checksum:23570951, data:951e65305b56a3706e3e86eb01c803bbf915d80edcd64d4d0000000000",
        "seqNum:10, seqLen:9, messageLen:256, checksum:23570951, data:330f0f33a05eead4f331df229871bee733b50de71afd2e5a79f196de09",
        "seqNum:11, seqLen:9, messageLen:256, checksum:23570951, data:3b205ce5e52d8c24a52cffa34c564fa1af3fdffcd349dc4258ee4ee828",
        "seqNum:12, seqLen:9, messageLen:256, checksum:23570951, data:dd7bf725ea6c16d531b5f03254783803048ca08b87148daacd1cd7a006",
        "seqNum:13, seqLen:9, messageLen:256, checksum:23570951, data:760be7ad1c6187902bbc04f539b9ee5eb8ea6833222edea36031306c01",
        "seqNum:14, seqLen:9, messageLen:256, checksum:23570951, data:5bf4031217d2c3254b088fa7553778b5003632f46e21db129416f65b55",
        "seqNum:15, seqLen:9, messageLen:256, checksum:23570951, data:73f021c0e6f65b05c0a494e50791270a0050a73ae69b6725505a2ec8a5",
        "seqNum:16, seqLen:9, messageLen:256, checksum:23570951, data:b8546ebfe2048541348910267331c643133f828afec9337c318f71b7df",
        "seqNum:17, seqLen:9, messageLen:256, checksum:23570951, data:23dedeea74e3a0fb052befabefa13e2f80e4315c9dceed4c8630612e64",
        "seqNum:18, seqLen:9, messageLen:256, checksum:23570951, data:d01a8daee769ce34b6b35d3ca0005302724abddae405bdb419c0a6b208",
        "seqNum:19, seqLen:9, messageLen:256, checksum:23570951, data:3171c5dc365766eff25ae47c6f10e7de48cfb8474e050e5fe997a6dc24",
        "seqNum:20, seqLen:9, messageLen:256, checksum:23570951, data:e055c2433562184fa71b4be94f262e200f01c6f74c284b0dc6fae6673f"
    };
    assert(parts == expected_parts);
     if (parts == expected_parts) {
          Serial.println("fountain_encoder passed!");
    } else {
       Serial.println("fountain_encoder failed");
    }
}

static void test_fountain_encoder_cbor() {
    auto message = make_message(256);
    auto encoder = ur::FountainEncoder(message, 30);
    ur::StringVector parts;
    for(int i = 0; i < 20; i++) {
        parts.push_back(ur::data_to_hex(encoder.next_part().cbor()));
    }
    ur::StringVector expected_parts = {
        "8501091901001a0167aa07581d916ec65cf77cadf55cd7f9cda1a1030026ddd42e905b77adc36e4f2d3c",
        "8502091901001a0167aa07581dcba44f7f04f2de44f42d84c374a0e149136f25b01852545961d55f7f7a",
        "8503091901001a0167aa07581d8cde6d0e2ec43f3b2dcb644a2209e8c9e34af5c4747984a5e873c9cf5f",
        "8504091901001a0167aa07581d965e25ee29039fdf8ca74f1c769fc07eb7ebaec46e0695aea6cbd60b3e",
        "8505091901001a0167aa07581dc4bbff1b9ffe8a9e7240129377b9d3711ed38d412fbb4442256f1e6f59",
        "8506091901001a0167aa07581d5e0fc57fed451fb0a0101fb76b1fb1e1b88cfdfdaa946294a47de8fff1",
        "8507091901001a0167aa07581d73f021c0e6f65b05c0a494e50791270a0050a73ae69b6725505a2ec8a5",
        "8508091901001a0167aa07581d791457c9876dd34aadd192a53aa0dc66b556c0c215c7ceb8248b717c22",
        "8509091901001a0167aa07581d951e65305b56a3706e3e86eb01c803bbf915d80edcd64d4d0000000000",
        "850a091901001a0167aa07581d330f0f33a05eead4f331df229871bee733b50de71afd2e5a79f196de09",
        "850b091901001a0167aa07581d3b205ce5e52d8c24a52cffa34c564fa1af3fdffcd349dc4258ee4ee828",
        "850c091901001a0167aa07581ddd7bf725ea6c16d531b5f03254783803048ca08b87148daacd1cd7a006",
        "850d091901001a0167aa07581d760be7ad1c6187902bbc04f539b9ee5eb8ea6833222edea36031306c01",
        "850e091901001a0167aa07581d5bf4031217d2c3254b088fa7553778b5003632f46e21db129416f65b55",
        "850f091901001a0167aa07581d73f021c0e6f65b05c0a494e50791270a0050a73ae69b6725505a2ec8a5",
        "8510091901001a0167aa07581db8546ebfe2048541348910267331c643133f828afec9337c318f71b7df",
        "8511091901001a0167aa07581d23dedeea74e3a0fb052befabefa13e2f80e4315c9dceed4c8630612e64",
        "8512091901001a0167aa07581dd01a8daee769ce34b6b35d3ca0005302724abddae405bdb419c0a6b208",
        "8513091901001a0167aa07581d3171c5dc365766eff25ae47c6f10e7de48cfb8474e050e5fe997a6dc24",
        "8514091901001a0167aa07581de055c2433562184fa71b4be94f262e200f01c6f74c284b0dc6fae6673f"
    };
    assert(parts == expected_parts);
     if (parts == expected_parts) {
          Serial.println("fountain_encoder cbor passed!");
    } else {
       Serial.println("fountain_encoder cbor failed");
    }
}



static void test_fountain_encoder_is_complete() {
    auto message = make_message(256);
    auto encoder = ur::FountainEncoder(message, 30);
    size_t generated_parts_count = 0;
    while(!encoder.is_complete()) {
        encoder.next_part();
        generated_parts_count += 1;
    }
    assert(encoder.seq_len() == generated_parts_count);
      if (encoder.seq_len() == generated_parts_count) {
          Serial.println("fountain_encoder_is_complete passed!");
    } else {
       Serial.println("fountain_encoder_is_complete failed");
    }
}

static void test_fountain_decoder() {
    std::string message_seed = "Wolf";
    size_t message_size = 32767;
    size_t max_fragment_len = 1000;

    auto message = make_message(message_size, message_seed);
    auto encoder = ur::FountainEncoder(message, max_fragment_len, 100);
    auto decoder = ur::FountainDecoder();
    do {
        auto part = encoder.next_part();
        decoder.receive_part(part);
    } while(!decoder.is_complete());

    if(decoder.is_success()) {
        assert(decoder.result_message() == message);
    } else {
       // cout << decoder.result_error().what() << endl;
        assert(false);
    }

     if (decoder.is_success()&&decoder.result_message() == message) {
          Serial.println("fountain_decoder passed!");
    } else {
       Serial.println("fountain_decoder failed");
    }
}

static void test_fountain_cbor() {
    auto part = ur::FountainEncoder::Part(12, 8, 100, 0x12345678, {1, 5, 3, 3 ,5});
    auto cbor = part.cbor();
    auto part2 = ur::FountainEncoder::Part(cbor);
    auto cbor2 = part2.cbor();
    assert(cbor == cbor2);
    if (cbor == cbor2) {
          Serial.println("fountain_cbor passed!");
    } else {
       Serial.println("fountain_cbor failed");
    }
}
static void test_single_part_ur() {
    auto ur = make_message_ur(50);
    auto encoded = ur::UREncoder::encode(ur);
    std::string expected = "ur:bytes/hdeymejtswhhylkepmykhhtsytsnoyoyaxaedsuttydmmhhpktpmsrjtgwdpfnsboxgwlbaawzuefywkdplrsrjynbvygabwjldapfcsdwkbrkch";
    assert(encoded == expected);
    auto decoded = ur::URDecoder::decode(encoded);
    assert(ur == decoded);

      if (encoded == expected&&ur == decoded) {
          Serial.println("test_single_part_ur passed!");
    } else {
       Serial.println("test_single_part_ur failed");
    }
}

static void test_ur_encoder() {
    auto ur = make_message_ur(256);
    auto encoder = ur::UREncoder(ur, 30);
    ur::StringVector parts;
    for(int i = 0; i < 20; i++) {
        parts.push_back(encoder.next_part());
    }
    ur::StringVector expected_parts = {
        "ur:bytes/1-9/lpadascfadaxcywenbpljkhdcahkadaemejtswhhylkepmykhhtsytsnoyoyaxaedsuttydmmhhpktpmsrjtdkgslpgh",
        "ur:bytes/2-9/lpaoascfadaxcywenbpljkhdcagwdpfnsboxgwlbaawzuefywkdplrsrjynbvygabwjldapfcsgmghhkhstlrdcxaefz",
        "ur:bytes/3-9/lpaxascfadaxcywenbpljkhdcahelbknlkuejnbadmssfhfrdpsbiegecpasvssovlgeykssjykklronvsjksopdzmol",
        "ur:bytes/4-9/lpaaascfadaxcywenbpljkhdcasotkhemthydawydtaxneurlkosgwcekonertkbrlwmplssjtammdplolsbrdzcrtas",
        "ur:bytes/5-9/lpahascfadaxcywenbpljkhdcatbbdfmssrkzmcwnezelennjpfzbgmuktrhtejscktelgfpdlrkfyfwdajldejokbwf",
        "ur:bytes/6-9/lpamascfadaxcywenbpljkhdcackjlhkhybssklbwefectpfnbbectrljectpavyrolkzczcpkmwidmwoxkilghdsowp",
        "ur:bytes/7-9/lpatascfadaxcywenbpljkhdcavszmwnjkwtclrtvaynhpahrtoxmwvwatmedibkaegdosftvandiodagdhthtrlnnhy",
        "ur:bytes/8-9/lpayascfadaxcywenbpljkhdcadmsponkkbbhgsoltjntegepmttmoonftnbuoiyrehfrtsabzsttorodklubbuyaetk",
        "ur:bytes/9-9/lpasascfadaxcywenbpljkhdcajskecpmdckihdyhphfotjojtfmlnwmadspaxrkytbztpbauotbgtgtaeaevtgavtny",
        "ur:bytes/10-9/lpbkascfadaxcywenbpljkhdcahkadaemejtswhhylkepmykhhtsytsnoyoyaxaedsuttydmmhhpktpmsrjtwdkiplzs",
        "ur:bytes/11-9/lpbdascfadaxcywenbpljkhdcahelbknlkuejnbadmssfhfrdpsbiegecpasvssovlgeykssjykklronvsjkvetiiapk",
        "ur:bytes/12-9/lpbnascfadaxcywenbpljkhdcarllaluzmdmgstospeyiefmwejlwtpedamktksrvlcygmzemovovllarodtmtbnptrs",
        "ur:bytes/13-9/lpbtascfadaxcywenbpljkhdcamtkgtpknghchchyketwsvwgwfdhpgmgtylctotzopdrpayoschcmhplffziachrfgd",
        "ur:bytes/14-9/lpbaascfadaxcywenbpljkhdcapazewnvonnvdnsbyleynwtnsjkjndeoldydkbkdslgjkbbkortbelomueekgvstegt",
        "ur:bytes/15-9/lpbsascfadaxcywenbpljkhdcaynmhpddpzmversbdqdfyrehnqzlugmjzmnmtwmrouohtstgsbsahpawkditkckynwt",
        "ur:bytes/16-9/lpbeascfadaxcywenbpljkhdcawygekobamwtlihsnpalnsghenskkiynthdzotsimtojetprsttmukirlrsbtamjtpd",
        "ur:bytes/17-9/lpbyascfadaxcywenbpljkhdcamklgftaxykpewyrtqzhydntpnytyisincxmhtbceaykolduortotiaiaiafhiaoyce",
        "ur:bytes/18-9/lpbgascfadaxcywenbpljkhdcahkadaemejtswhhylkepmykhhtsytsnoyoyaxaedsuttydmmhhpktpmsrjtntwkbkwy",
        "ur:bytes/19-9/lpbwascfadaxcywenbpljkhdcadekicpaajootjzpsdrbalpeywllbdsnbinaerkurspbncxgslgftvtsrjtksplcpeo",
        "ur:bytes/20-9/lpbbascfadaxcywenbpljkhdcayapmrleeleaxpasfrtrdkncffwjyjzgyetdmlewtkpktgllepfrltataztksmhkbot"
    };
    assert(parts == expected_parts);
     if (parts == expected_parts) {
          Serial.println("test_ur_encoder passed!");
    } else {
       Serial.println("test_ur_encoder failed");
    }
}

static void test_multipart_ur() {
    auto ur = make_message_ur(32767);
    size_t max_fragment_len = 1000;
    uint32_t first_seq_num = 100;
    auto encoder = ur::UREncoder(ur, max_fragment_len, first_seq_num);
    auto decoder = ur::URDecoder();
    do {
        auto part = encoder.next_part();
        decoder.receive_part(part);
        Serial.println("1");
    } while(!decoder.is_complete());

    if(decoder.is_success()) {
        assert(decoder.result_ur() == ur);
    } else {
      //  cout << decoder.result_error().what() << endl;
        assert(false);
    }
        if (decoder.is_success()&&decoder.result_ur() == ur) {
          Serial.println("test_multipart_ur passed!");
    } else {
       Serial.println("test_multipart_ur failed");
    }
}
