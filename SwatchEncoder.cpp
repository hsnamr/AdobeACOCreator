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

typedef struct __internal_aco_struct {
    uint16_t w;
    uint16_t x;
    uint16_t y;
    size_t len;         // [Optional] pass 0 if you don't want to assign a name to the color
    uint16_t* name;     // [Optional] name will be ignored if len is 0
} _internal_aco_struct;

void appendBytes(vector<uint8_t> &vec, uint8_t* bytes, size_t length) {
    vec.insert(vec.end(), bytes, bytes + length);
}

uint16_t swap_uint16(uint16_t val) {
    return (val << 8) | (val >> 8 );
}

vector<uint8_t> __encodeSwatch(int16_t version, _internal_aco_struct* colors, int16_t nColors) {
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
        int16_t w = swap_uint16((uint16_t)colors[i].w);
        int16_t x = swap_uint16((uint16_t)colors[i].x);
        int16_t y = swap_uint16((uint16_t)colors[i].y);
        
        // color space
        appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&colorSpace)), 2);
        // w
        appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&w)), 2);
        // x
        appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&x)), 2);
        // y
        appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&y)), 2);
        // unused
        appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&kZero)), 2);
        
        // append color name
        // add 1 to length for null terminator
        int16_t len = swap_uint16(colors[i].len + 1);
        if(len < 1) continue;
        
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

vector<uint8_t> encodeSwatch_lab(aco_struct_lab* colors, size_t nColors) {
    return __encodeSwatch(2, (__internal_aco_struct*)colors, (int16_t)nColors);
}

vector<uint8_t> encodeSwatch_lab(aco_struct_rgb* colors, size_t nColors) {
    return __encodeSwatch(2, (__internal_aco_struct*)colors, (int16_t)nColors);
}
