//
//  SwatchEncoder.cpp
//  SwatchEncoder
//
//  Created on 02/12/2018.
//  Updated 02/25/2018
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

static const uint16_t kColorSpaceRGB        = 0;
static const uint16_t kColorSpaceCMYK       = 2;
static const uint16_t kColorSpaceLAB        = 7;

typedef struct __internal_aco_struct {
    uint16_t w;
    uint16_t x;
    uint16_t y;
    uint16_t z;
    size_t len;
    uint16_t* name;
} _internal_aco_struct;

void appendBytes(vector<uint8_t> &vec, uint8_t* bytes, size_t length) {
    vec.insert(vec.end(), bytes, bytes + length);
}

uint16_t swap_uint16(uint16_t val) {
    return (val << 8) | (val >> 8 );
}

vector<uint8_t> __encodeSwatch(int16_t version, _internal_aco_struct* colors, int16_t nColors, uint16_t colorSpace ) {
    vector<uint8_t> mData;
    // version
    int16_t versionBE = swap_uint16(version);
    appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&versionBE)), 2);
    // number of color
    int16_t nColorsBE = swap_uint16(nColors);
    appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&nColorsBE)), 2);
    // color space
    int16_t colorSpaceBE = swap_uint16(colorSpace);
    
    // variable for the total number of colors
    for (size_t i = 0; i < nColors; i++) {
        // Swap everything to Big Endian
        int16_t w = swap_uint16(colors[i].w);
        int16_t x = swap_uint16(colors[i].x);
        int16_t y = swap_uint16(colors[i].y);
        int16_t z = swap_uint16(colors[i].z);
        
        // color space
        appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&colorSpaceBE)), 2);
        // w
        appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&w)), 2);
        // x
        appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&x)), 2);
        // y
        appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&y)), 2);
        // z
        appendBytes(mData, static_cast<uint8_t*>(static_cast<void*>(&z)), 2);
        
        // append color name
        // add 1 to length for null terminator
        int16_t len = swap_uint16(colors[i].len + 1);
        if(len < 1) continue;
        
        // no need to swap, name must be in UTF-16BE, it is up to the caller to ensure that
        uint16_t* utf16name = (colors[i].name);
        
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

vector<uint8_t> encodeSwatch(aco_struct_lab* colors, size_t nColors) {
    _internal_aco_struct _colors;
    _colors.w = colors->L;
    _colors.x = colors->a;
    _colors.y = colors->b;
    _colors.z = kZero;
    _colors.len = colors->len;
    _colors.name = colors->name;
    
    return __encodeSwatch(2, &_colors, (int16_t)nColors, kColorSpaceLAB);
}
                               
vector<uint8_t> encodeSwatch(aco_struct_rgb* colors, size_t nColors) {
    _internal_aco_struct _colors;
    _colors.w = colors->R;
    _colors.x = colors->G;
    _colors.y = colors->B;
    _colors.z = kZero;
    _colors.len = colors->len;
    _colors.name = colors->name;
    
    return __encodeSwatch(2, &_colors, (int16_t)nColors, kColorSpaceRGB);
}

vector<uint8_t> encodeSwatch(aco_struct_cmyk* colors, size_t nColors) {
    _internal_aco_struct _colors;
    _colors.w = colors->C;
    _colors.x = colors->M;
    _colors.y = colors->Y;
    _colors.z = colors->K;
    _colors.len = colors->len;
    _colors.name = colors->name;
    
    return __encodeSwatch(2, &_colors, (int16_t)nColors, kColorSpaceRGB);
}

