//
//  bytewords.cpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "bytewords.hpp"
#include "utils.hpp"
#include <stdexcept>
#include <algorithm>

namespace ur {

using namespace std;

static const char* bytewords = "ableacidalsoapexaquaarchatomauntawayaxisbackbaldbarnbeltbetabiasbluebodybragbrewbulbbuzzcalmcashcatschefcityclawcodecolacookcostcruxcurlcuspcyandarkdatadaysdelidicedietdoordowndrawdropdrumdulldutyeacheasyechoedgeepicevenexamexiteyesfactfairfernfigsfilmfishfizzflapflewfluxfoxyfreefrogfuelfundgalagamegeargemsgiftgirlglowgoodgraygrimgurugushgyrohalfhanghardhawkheathelphighhillholyhopehornhutsicedideaidleinchinkyintoirisironitemjadejazzjoinjoltjowljudojugsjumpjunkjurykeepkenokeptkeyskickkilnkingkitekiwiknoblamblavalazyleaflegsliarlimplionlistlogoloudloveluaulucklungmainmanymathmazememomenumeowmildmintmissmonknailnavyneednewsnextnoonnotenumbobeyoboeomitonyxopenovalowlspaidpartpeckplaypluspoempoolposepuffpumapurrquadquizraceramprealredorichroadrockroofrubyruinrunsrustsafesagascarsetssilkskewslotsoapsolosongstubsurfswantacotasktaxitenttiedtimetinytoiltombtoystriptunatwinuglyundouniturgeuservastveryvetovialvibeviewvisavoidvowswallwandwarmwaspwavewaxywebswhatwhenwhizwolfworkyankyawnyellyogayurtzapszerozestzinczonezoom";

uint8_t decode_word(const string& word, size_t word_len) {
    if(word.length() != word_len) {
        throw runtime_error("Invalid Bytewords.");
    }

    static int16_t* array = NULL;
    const size_t dim = 26;

    // Since the first and last letters of each Byteword are unique,
    // we can use them as indexes into a two-dimensional lookup table.
    // This table is generated lazily.
    if(array == NULL) {
        const size_t array_len = dim * dim;
        array = (int16_t*)malloc(array_len * sizeof(int16_t));
        for(size_t i = 0; i < array_len; i++) {
            array[i] = -1;
        }
        for(size_t i = 0; i < 256; i++) {
            const char* byteword = bytewords + i * 4;
            size_t x = byteword[0] - 'a';
            size_t y = byteword[3] - 'a';
            size_t offset = y * dim + x;
            array[offset] = i;
        }
    }

    // If the coordinates generated by the first and last letters are out of bounds,
    // or the lookup table contains -1 at the coordinates, then the word is not valid.
    int x = tolower(word[0]) - 'a';
    int y = tolower(word[word_len == 4 ? 3 : 1]) - 'a';
    if(!(0 <= x && x < dim && 0 <= y && y < dim)) {
        throw runtime_error("Invalid Bytewords.");
    }
    size_t offset = y * dim + x;
    int16_t value = array[offset];
    if(value == -1) {
        throw runtime_error("Invalid Bytewords.");
    }

    // If we're decoding a full four-letter word, verify that the two middle letters are correct.
    if(word_len == 4) {
        const char* byteword = bytewords + value * 4;
        int c1 = tolower(word[1]);
        int c2 = tolower(word[2]);
        if(c1 != byteword[1] || c2 != byteword[2]) {
            throw runtime_error("Invalid Bytewords.");
        }
    }

    // Successful decode.
    return value;
}

static const string get_word(uint8_t index) {
    auto p = &bytewords[index * 4];
    return string(p, p + 4);
}

static const string get_minimal_word(uint8_t index) {
    string word;
    word.reserve(2);
    auto p = &bytewords[index * 4];
    word.push_back(*p);
    word.push_back(*(p + 3));
    return word;
}

static const string encode(const ByteVector& buf, const string& separator) {
    auto len = buf.size();
    StringVector words;
    words.reserve(len);
    for(int i = 0; i < len; i++) {
        auto byte = buf[i];
        words.push_back(get_word(byte));
    }
    return join(words, separator);
}

static const ByteVector add_crc(const ByteVector& buf) {
    auto crc_buf = crc32_bytes(buf);
    auto result = buf;
    append(result, crc_buf);
    return result;
}

static const string encode_with_separator(const ByteVector& buf, const string& separator) {
    auto crc_buf = add_crc(buf);
    return encode(crc_buf, separator);
}

static const string encode_minimal(const ByteVector& buf) {
    string result;
    auto crc_buf = add_crc(buf);
    auto len = crc_buf.size();
    for(int i = 0; i < len; i++) {
        auto byte = crc_buf[i];
        result.append(get_minimal_word(byte));
    }
    return result;
}

static const ByteVector _decode(const string& s, char separator, size_t word_len) {
    StringVector words;
    if(word_len == 4) {
        words = split(s, separator);
    } else {
        words = partition(s, 2);
    }
    ByteVector buf;
    transform(words.begin(), words.end(), back_inserter(buf), [&](auto word) { return decode_word(word, word_len); });
    if(buf.size() < 5) {
        throw runtime_error("Invalid Bytewords.");
    }
    auto p = split(buf, buf.size() - 4);
    auto body = p.first;
    auto body_checksum = p.second;
    auto checksum = crc32_bytes(body);
    if(checksum != body_checksum) {
        throw runtime_error("Invalid Bytewords.");
    }

    return body;
}

string Bytewords::encode(style style, const ByteVector& bytes) {
    switch(style) {
        case standard:
            return encode_with_separator(bytes, " ");
        case uri:
            return encode_with_separator(bytes, "-");
        case minimal:
            return encode_minimal(bytes);
        default:
            assert(false);
    }
}

ByteVector Bytewords::decode(style style, const string& string) {
    switch(style) {
        case standard:
            return _decode(string, ' ', 4);
        case uri:
            return _decode(string, '-', 4);
        case minimal:
            return _decode(string, 0, 2);
        default:
            assert(false);
    }
}

}