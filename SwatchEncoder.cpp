//
//  SwatchEncoder.cpp
//  SwatchEncoder
//
//  Created by Hussian Al-Amri on 02/12/2018.
//  Copyright © 2018 H4n. All rights reserved.
//

#include "SwatchEncoder.hpp"
#include <vector>
#include <string>
#include <cassert>
#include <cmath>

static uint16_t kZero = 0x0;

using std::vector;
using std::string;
using namespace SwatchEncoder;

void appendBytes(vector<uint8_t> &vec, uint8_t* bytes, size_t length) {
    vec.insert(vec.end(), bytes, bytes + length);
}

uint16_t swap_uint16(uint16_t val) {
    return (val << 8) | (val >> 8 );
}

vector<uint8_t> __encodeSwatch(int16_t version, aco_struct_v2* colors, int16_t nColors) {
    vector<uint8_t> mData;
    // version
    int16_t versionBE = swap_uint16(version);
    appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&versionBE)), 2);
    // number of color
    int16_t nColorsBE = swap_uint16(nColors);
    appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&nColorsBE)), 2);
    // color space
    int16_t colorSpace = swap_uint16(kColorSpaceLAB);
    
    // variable for the total number of colors
    for (size_t i = 0; i < nColors; i++) {
        // Swap everything to Big Endian
        int16_t L = swap_uint16((uint16_t)roundf(colors[i].L*100));
        int16_t a = swap_uint16((uint16_t)roundf(colors[i].a*100));
        int16_t b = swap_uint16((uint16_t)roundf(colors[i].b*100));
        
        // color space
        appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&colorSpace)), 2);
        // L
        appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&L)), 2);
        // a
        appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&a)), 2);
        // b
        appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&b)), 2);
        // unused
        appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&kZero)), 2);
        
        if (version == 1) {
            // v1 has no name for colors
            continue;
        }
        
        // v2 only append color name
        // add 1 to length for null terminator
        int16_t len = swap_uint16(colors[i].len + 1);
        
        // no need to swap, name must be in UTF-16BE, it is up to the caller to ensure that
        uint16_t* utf16name = colors[i].name;
        
        // 0
        appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&kZero)), 2);
        // length + 1
        appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&len)), 2);
        // UTF-16 name not null terminated - name should be in UTF-16BE, it is up to the caller to ensure that
        appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(utf16name)), colors[i].len);
        // 0
        appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&kZero)), 2);
    }
    
    return mData;
}

vector<uint8_t> encodeSwatch(aco_struct_v1* colors, size_t nColors) {
    return __encodeSwatch(1, (aco_struct_v2*)colors, (int16_t)nColors);
}

vector<uint8_t> encodeSwatch(aco_struct_v2* colors, size_t nColors) {
    return __encodeSwatch(2, colors, (int16_t)nColors);
}


