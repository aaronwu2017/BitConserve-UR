 

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

#include "ur.hpp"
#include "SPIFFS.h"
#include <iostream>
#include <fstream>
const char* psbtData[] = {
"UR:CRYPTO-PSBT/610-2/LPCFAOIDAOCFADLFCYVEVSNTPSHDSEROIANSCPSPHLFMGODWJLFPBGKBCWIDSRRHLRTNJZNYSTEOCSIYWMUTYAFSDLWYJZTNBWP"
    "RMSDITTDWZOPRGMTTJLRYLKOSJTDPREGELKRNWFGHWSDRQZHPLPGWMOWNADAXAACHAEBBZCEMEONSIOJSNNDPPMAYEEGTNSBYKBHSZCATIABAASKEBBJNNBO"
    "YUTRPTLLKOEAAJYFGAAISJYTLUTSPRHCXOECWFXSWONCXRYFYIARFLGBTAEGWADAHLOPRCKAXHSRDWFZMDTYTEOUYHYEHWYNSWNFTRTAHCERKDASOINEEIMW"
    "SAHLPTDWDATOEREENAOIHMHEESRNDEMMTHFMUHKDLLNGWGLJNHYWDOTDTLGFTFMJSNSLAHSGURFZEFTSGSSDY",
    // Add all other data strings here
    "UR:CRYPTO-PSBT/605-2/LPCFAOHLAOCFADLFCYVEVSNTPSHDSEROIANSCPSPHLFMGODWJLFPBGKBCWIDSRRHLRTNJZNYSTEOCSIYWMUTYAFSDLWYJZTNBWP"
"RMSDITTDWZOPRGMTTJLRYLKOSJTDPREGELKRNWFGHWSDRQZHPLPGWMOWNADAXAACHAEBBZCEMEONSIOJSNNDPPMAYEEGTNSBYKBHSZCATIABAASKEBBJNNBO"
"YUTRPTLLKOEAAJYFGAAISJYTLUTSPRHCXOECWFXSWONCXRYFYIARFLGBTAEGWADAHLOPRCKAXHSRDWFZMDTYTEOUYHYEHWYNSWNFTRTAHCERKDASOINEEIMW"
"SAHLPTDWDATOEREENAOIHMHEESRNDEMMTHFMUHKDLLNGWGLJNHYWDOTDTLGFTFMJSNSLAHSGURFZEWKHFCKRS",
"UR:CRYPTO-PSBT/606-2/LPCFAOHYAOCFADLFCYVEVSNTPSHDSEROIANSCPSPHLFMGODWJLFPBGKBCWIDSRRHLRTNJZNYSTEOCSIYWMUTYAFSDLWYJZTNBWP"
"RMSDITTDWZOPRGMTTJLRYLKOSJTDPREGELKRNWFGHWSDRQZHPLPGWMOWNADAXAACHAEBBZCEMEONSIOJSNNDPPMAYEEGTNSBYKBHSZCATIABAASKEBBJNNBO"
"YUTRPTLLKOEAAJYFGAAISJYTLUTSPRHCXOECWFXSWONCXRYFYIARFLGBTAEGWADAHLOPRCKAXHSRDWFZMDTYTEOUYHYEHWYNSWNFTRTAHCERKDASOINEEIMW"
"SAHLPTDWDATOEREENAOIHMHEESRNDEMMTHFMUHKDLLNGWGLJNHYWDOTDTLGFTFMJSNSLAHSGURFZELUSGGHUT",

"UR:CRYPTO-PSBT/607-2/LPCFAOHEAOCFADLFCYVEVSNTPSHDSEROIANSCPSPHLFMGODWJLFPBGKBCWIDSRRHLRTNJZNYSTEOCSIYWMUTYAFSDLWYJZTNBWP"
"RMSDITTDWZOPRGMTTJLRYLKOSJTDPREGELKRNWFGHWSDRQZHPLPGWMOWNADAXAACHAEBBZCEMEONSIOJSNNDPPMAYEEGTNSBYKBHSZCATIABAASKEBBJNNBO"
"YUTRPTLLKOEAAJYFGAAISJYTLUTSPRHCXOECWFXSWONCXRYFYIARFLGBTAEGWADAHLOPRCKAXHSRDWFZMDTYTEOUYHYEHWYNSWNFTRTAHCERKDASOINEEIMW"
"SAHLPTDWDATOEREENAOIHMHEESRNDEMMTHFMUHKDLLNGWGLJNHYWDOTDTLGFTFMJSNSLAHSGURFZECHMEJLFN",
"UR:CRYPTO-PSBT/608-2/LPCFAOHNAOCFADLFCYVEVSNTPSHDSEHKADLBJOJKIDJYZMADAEJSAOAEAEAEADWELRTNWPNYSTEOMKIYWMUTKSFSDMWSJKTPGMP"
"AMSDITTDWZOOXGMSKQZSKHLPSOLADAEAEAEAEZCZMZMZMAOPLLBAOAEAEAEAEAECMAEBBZCBZECNEDLQZGUHPGUWDUTHLLUCPFPNTSPYKCARPNSPMAEAEAEA"
"EAEAECMAEBBMSBZPKWFHHJZPYSWPKKGJYOECWSRSWONCXFSFYIARFBTBNAEGWADAALOPRCKAXFXROWTWTLAAEAEAELPRPRDNETLWSSEWLGEDMWZDEOLNEDRB"
"SEYDIRYASWLNDFHMUSOGESPDWRYLAGOGHAOMUHKPELNGWGLWEHYWDOTPTLKFTFMJSNNLAHSGURFZEBKCNHSZE"
};
// Number of elements in the array
int numElements = sizeof(psbtData) / sizeof(psbtData[0]);
 auto decoder = ur::URDecoder();
 static void test_psbtqr_decoder(){
   for (int i = 0; i < numElements; i++) {
        Serial.println(psbtData[i]);
          decoder.receive_part(psbtData[i]);
                  Serial.println(decoder.estimated_percent_complete());
          
        // Add any additional processing you want to do with each string here
    }
      if (decoder.is_success()) {
       
        Serial.println();
        decoder.result_ur().cbor();
        printByteVector(decoder.result_ur().cbor());
      }
       Serial.println(decoder.is_success());

 Serial.println(decoder.is_complete());
}

void printByteVector(const ur::ByteVector& vec) {
    for (uint8_t byte : vec) {
        // Print as a hexadecimal number
        if (byte < 16) Serial.print("0"); // Print a leading zero for numbers less than 16 (0x10)
        Serial.print(byte, HEX); // Print the byte in hexadecimal format
        
    }
    Serial.println(); // New line after printing the vector
}