 

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


// const char* psbtData[] = {
// "UR:CRYPTO-PSBT/610-2/LPCFAOIDAOCFADLFCYVEVSNTPSHDSEROIANSCPSPHLFMGODWJLFPBGKBCWIDSRRHLRTNJZNYSTEOCSIYWMUTYAFSDLWYJZTNBWP"
//     "RMSDITTDWZOPRGMTTJLRYLKOSJTDPREGELKRNWFGHWSDRQZHPLPGWMOWNADAXAACHAEBBZCEMEONSIOJSNNDPPMAYEEGTNSBYKBHSZCATIABAASKEBBJNNBO"
//     "YUTRPTLLKOEAAJYFGAAISJYTLUTSPRHCXOECWFXSWONCXRYFYIARFLGBTAEGWADAHLOPRCKAXHSRDWFZMDTYTEOUYHYEHWYNSWNFTRTAHCERKDASOINEEIMW"
//     "SAHLPTDWDATOEREENAOIHMHEESRNDEMMTHFMUHKDLLNGWGLJNHYWDOTDTLGFTFMJSNSLAHSGURFZEFTSGSSDY",
//     // Add all other data strings here
//     "UR:CRYPTO-PSBT/605-2/LPCFAOHLAOCFADLFCYVEVSNTPSHDSEROIANSCPSPHLFMGODWJLFPBGKBCWIDSRRHLRTNJZNYSTEOCSIYWMUTYAFSDLWYJZTNBWP"
// "RMSDITTDWZOPRGMTTJLRYLKOSJTDPREGELKRNWFGHWSDRQZHPLPGWMOWNADAXAACHAEBBZCEMEONSIOJSNNDPPMAYEEGTNSBYKBHSZCATIABAASKEBBJNNBO"
// "YUTRPTLLKOEAAJYFGAAISJYTLUTSPRHCXOECWFXSWONCXRYFYIARFLGBTAEGWADAHLOPRCKAXHSRDWFZMDTYTEOUYHYEHWYNSWNFTRTAHCERKDASOINEEIMW"
// "SAHLPTDWDATOEREENAOIHMHEESRNDEMMTHFMUHKDLLNGWGLJNHYWDOTDTLGFTFMJSNSLAHSGURFZEWKHFCKRS",
// "UR:CRYPTO-PSBT/606-2/LPCFAOHYAOCFADLFCYVEVSNTPSHDSEROIANSCPSPHLFMGODWJLFPBGKBCWIDSRRHLRTNJZNYSTEOCSIYWMUTYAFSDLWYJZTNBWP"
// "RMSDITTDWZOPRGMTTJLRYLKOSJTDPREGELKRNWFGHWSDRQZHPLPGWMOWNADAXAACHAEBBZCEMEONSIOJSNNDPPMAYEEGTNSBYKBHSZCATIABAASKEBBJNNBO"
// "YUTRPTLLKOEAAJYFGAAISJYTLUTSPRHCXOECWFXSWONCXRYFYIARFLGBTAEGWADAHLOPRCKAXHSRDWFZMDTYTEOUYHYEHWYNSWNFTRTAHCERKDASOINEEIMW"
// "SAHLPTDWDATOEREENAOIHMHEESRNDEMMTHFMUHKDLLNGWGLJNHYWDOTDTLGFTFMJSNSLAHSGURFZELUSGGHUT",

// "UR:CRYPTO-PSBT/607-2/LPCFAOHEAOCFADLFCYVEVSNTPSHDSEROIANSCPSPHLFMGODWJLFPBGKBCWIDSRRHLRTNJZNYSTEOCSIYWMUTYAFSDLWYJZTNBWP"
// "RMSDITTDWZOPRGMTTJLRYLKOSJTDPREGELKRNWFGHWSDRQZHPLPGWMOWNADAXAACHAEBBZCEMEONSIOJSNNDPPMAYEEGTNSBYKBHSZCATIABAASKEBBJNNBO"
// "YUTRPTLLKOEAAJYFGAAISJYTLUTSPRHCXOECWFXSWONCXRYFYIARFLGBTAEGWADAHLOPRCKAXHSRDWFZMDTYTEOUYHYEHWYNSWNFTRTAHCERKDASOINEEIMW"
// "SAHLPTDWDATOEREENAOIHMHEESRNDEMMTHFMUHKDLLNGWGLJNHYWDOTDTLGFTFMJSNSLAHSGURFZECHMEJLFN",
// "UR:CRYPTO-PSBT/608-2/LPCFAOHNAOCFADLFCYVEVSNTPSHDSEHKADLBJOJKIDJYZMADAEJSAOAEAEAEADWELRTNWPNYSTEOMKIYWMUTKSFSDMWSJKTPGMP"
// "AMSDITTDWZOOXGMSKQZSKHLPSOLADAEAEAEAEZCZMZMZMAOPLLBAOAEAEAEAEAECMAEBBZCBZECNEDLQZGUHPGUWDUTHLLUCPFPNTSPYKCARPNSPMAEAEAEA"
// "EAEAECMAEBBMSBZPKWFHHJZPYSWPKKGJYOECWSRSWONCXFSFYIARFBTBNAEGWADAALOPRCKAXFXROWTWTLAAEAEAELPRPRDNETLWSSEWLGEDMWZDEOLNEDRB"
// "SEYDIRYASWLNDFHMUSOGESPDWRYLAGOGHAOMUHKPELNGWGLWEHYWDOTPTLKFTFMJSNNLAHSGURFZEBKCNHSZE"
// };


const char* psbtData[] = {
   "ur:crypto-psbt/14-36/lpbacsdkcfadlfcyvevsntpsgrtlwssewlgedmwzdeolnedrdstyaywz",
"ur:crypto-psbt/15-36/lpbscsdkcfadlfcyvevsntpsgrbseydiryaswlndfhmusogesewltyrn",
"ur:crypto-psbt/16-36/lpbecsdkcfadlfcyvevsntpsgrspdwrylagoghaomuhkpelnvotnrnfg",
"ur:crypto-psbt/17-36/lpbycsdkcfadlfcyvevsntpsgrgwglwehywdotptlkftfmjsfxmetenb",
"ur:crypto-psbt/18-36/lpbgcsdkcfadlfcyvevsntpsgrnnlahsgurfzevyidvlgmrkcscxvavl",
"ur:crypto-psbt/19-36/lpbwcsdkcfadlfcyvevsntpsgrfhgepkdpjldybekbcwidsasfjlsrec",
"ur:crypto-psbt/20-36/lpbbcsdkcfadlfcyvevsntpsgrghaeaelaaeaeaelaaeaeaegtkobnfm",
"ur:crypto-psbt/21-36/lpbzcsdkcfadlfcyvevsntpsgrlaaeadadctaofpaxaeaeaeosfdsoty",
"ur:crypto-psbt/22-36/lpcmcsdkcfadlfcyvevsntpsgraeaecmaebbuyksttbdspdwloqdlnwp",
"ur:crypto-psbt/23-36/lpchcsdkcfadlfcyvevsntpsgrregelkrnbapybetlrpykzsbaehbgcn",
"ur:crypto-psbt/24-36/lpcscscsdkcfadlfcyvevsntpsgrgtmownadaxaaadaeaeaecphloxzsbe",
"ur:crypto-psbt/25-36/lpcscfcsdkcfadlfcyvevsntpsgramaxfdsksnkozevowlbechttieinsn",
"ur:crypto-psbt/26-36/lpcscycsdkcfadlfcyvevsntpsgreofhztecwzkbromdttbbjnbdktuyts",
"ur:crypto-psbt/27-36/lpcscwcsdkcfadlfcyvevsntpsgrnboyutrpsrlkrpmuhswpyllfhkbgia",
"ur:crypto-psbt/28-36/lpcscecsdkcfadlfcyvevsntpsgreecskbcwidsaghaeaelaaeldfljlps",
"ur:crypto-psbt/29-36/lpcscacsdkcfadlfcyvevsntpsgraeaelaaeaeaelaadaeaeaekbhdlggr",
"ur:crypto-psbt/30-36/lpcsckcsdkcfadlfcyvevsntpsgradaeaeaeaecpaoaxbsptyteetlwymk",
"ur:crypto-psbt/31-36/lpcsctcsdkcfadlfcyvevsntpsgreouyuyltghaxdktladwphfcxtiuewe",
"ur:crypto-psbt/32-36/lpcscxcsdkcfadlfcyvevsntpsgrmdtsvytkpyfzvtemoejlvlcehfcmsp",
"ur:crypto-psbt/33-36/lpcsclcsdkcfadlfcyvevsntpsgrwyesleonsbdlhdcskbcwidbwntkkox",
"ur:crypto-psbt/34-36/lpcscpcsdkcfadlfcyvevsntpsgrsaghaeaelaaeaeaelaaeaefytagahy",
"ur:crypto-psbt/35-36/lpcscncsdkcfadlfcyvevsntpsgraelaadaeaeaeaoaeaeaeaejybtpehk",
"ur:crypto-psbt/36-36/lpcsdkcsdkcfadlfcyvevsntpsgraeaeaeaeaeaeaeaeaeaeaeldbkkktl",
"ur:crypto-psbt/37-36/lpcsdacsdkcfadlfcyvevsntpsgrjydtgdecylhkhhbysglocasrtkoxgy",
"ur:crypto-psbt/38-36/lpcsdscsdkcfadlfcyvevsntpsgrjsrkdlksfnlafwfrmkweascyryvlps",
    "ur:crypto-psbt/39-36/lpcsdicsdkcfadlfcyvevsntpsgrbtbnaegwadaaloprckaxfxatpassgu",
    "ur:crypto-psbt/40-36/lpcsdecsdkcfadlfcyvevsntpsgrpkmkctfduefnreidvltdrkfybzfpbd",
    "ur:crypto-psbt/41-36/lpcsdtcsdkcfadlfcyvevsntpsgrmdtsvytkpyfzvtemoejlvlltnsrsft",
    "ur:crypto-psbt/42-36/lpcsdrcsdkcfadlfcyvevsntpsgrcfmownlyaxaaadlaaeaecpsknbfmim",
    "ur:crypto-psbt/43-36/lpcsdncsdkcfadlfcyvevsntpsgrkiaeehyatluyteonwtlrgtayuohksk",
    "ur:crypto-psbt/44-36/lpcsdwcsdkcfadlfcyvevsntpsgriofecsmhlgdkamcautmkrdzmvwjedi",
    "ur:crypto-psbt/45-36/lpcsdpcsdkcfadlfcyvevsntpsgrnesovdzmjnfmbgbyhyntsesfotnbeh",
    "ur:crypto-psbt/46-36/lpcsdmcsdkcfadlfcyvevsntpsgrhptoyaotzekgpfkkidylwesnhsoywt",
    "ur:crypto-psbt/47-36/lpcsdlcsdkcfadlfcyvevsntpsgrmygasnmytltizmrfdpwzlunbaxmste",
    "ur:crypto-psbt/48-36/lpcsdycsdkcfadlfcyvevsntpsgrimntuoiotsmdidghghssneamtbmwjo",
    "ur:crypto-psbt/49-36/lpcsehcsdkcfadlfcyvevsntpsgrflkkrtfymoolaavsfemywddeoxfxlt",
    "ur:crypto-psbt/50-36/lpcseycsdkcfadlfcyvevsntpsgrioaaiapyentazefdwfjecxtnrfwktl",
    "ur:crypto-psbt/51-36/lpcseocsdkcfadlfcyvevsntpsgrdyguurqzjpatylhdkkmyjsgwwplunn",
    "ur:crypto-psbt/52-36/lpcseecsdkcfadlfcyvevsntpsgrqdknplskuteclbckiabyyluedwjomk",
    "ur:crypto-psbt/53-36/lpcseccsdkcfadlfcyvevsntpsgrdmtydtdtveyazcryeecagweymyrenb",
    "ur:crypto-psbt/54-36/lpcsencsdkcfadlfcyvevsntpsgrgmpamsdittdwzooxgmskqzwpsehnsp",
    "ur:crypto-psbt/55-36/lpcsemcsdkcfadlfcyvevsntpsgrianspslyctcxfxlabsrsytrhjnisat",
    "ur:crypto-psbt/56-36/lpcsetcsdkcfadlfcyvevsntpsgrlywsseingedmwzpdolnedrbsidbsia",
    "ur:crypto-psbt/57-36/lpcsescsdkcfadlfcyvevsntpsgrnevybsbyhlfnmnenghjpkednpkdwhs",
    "ur:crypto-psbt/58-36/lpcsftcsdkcfadlfcyvevsntpsgrbtbnaegwadaaloprckaxfxntgogekb",
    "ur:crypto-psbt/59-36/lpcsfrcsdkcfadlfcyvevsntpsgrzmlbaxpllbaoaoaeaeaeaefdiawzke",
    "ur:crypto-psbt/60-36/lpcsfncsdkcfadlfcyvevsntpsgrgtmownadaxaaadaeaeaecphecahgcw"
 };
// Number of elements in the array
int numElements = sizeof(psbtData) / sizeof(psbtData[0]);
auto decoder = ur::URDecoder();

static void test_psbtqr_decoder(){
   for (int i = 0; i < numElements; i++) {
        decoder.receive_part(psbtData[i]);
    }
      if (decoder.is_success()) {
        //printByteVector(decoder.result_ur().cbor());
      }
}

void printByteVector(const ur::ByteVector& vec) {
    for (uint8_t byte : vec) {
        // Print as a hexadecimal number
        if (byte < 16) Serial.print("0"); // Print a leading zero for numbers less than 16 (0x10)
        Serial.print(byte, HEX); // Print the byte in hexadecimal format
        
    }
    Serial.println(); // New line after printing the vector
}