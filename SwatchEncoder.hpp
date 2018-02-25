//
//  SwatchEncoder.hpp
//  SwatchEncoder
//
//  Created by Hussian Al-Amri on 02/12/2018.
//  Copyright © 2018 H4n. All rights reserved.
//

#include <cstdbool>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <array>

namespace SwatchEncoder {

    static const uint16_t kColorSpaceRGB        = 0;
    static const uint16_t kColorSpaceHSB        = 1;    // Not currently implemented
    static const uint16_t kColorSpaceCMYK       = 2;    // Not currently implemented
    static const uint16_t kColorSpaceLAB        = 7;
    static const uint16_t kColorSpaceGrayscale  = 8;    // Not currently implemented

    /**
     * @typedef     aco_struct_lab
     * @brief       This struct holds the information for each color
     * @constant    L       Lab value L         ( range: 0..10000 )
     * @constant    a       Lab value a         ( range: -12800..12700 )
     * @constant    b       Lab value b         ( range: -12800..12700 )
     * @constant    len     Length of name (number of characters in name) not including null value if null-terminated string
     * @constant    name    Name of color (no null terminator), must be in UTF-16 Big Endian
     */
    typedef struct __aco_struct_lab {
        int16_t L;
        int16_t a;
        int16_t b;
        size_t len;         // [Optional] pass 0 if you don't want to assign a name to the color
        uint16_t* name;     // [Optional] name will be ignored if len is 0
    } aco_struct_lab;
    
    /**
     * @typedef     aco_struct_rgb
     * @brief       This struct holds the information for each color
     * @constant    R       RGB value Red       ( range: 0..65535 )
     * @constant    G       RGB value Green     ( range: 0..65535 )
     * @constant    B       RGB value Blue      ( range: 0..65535 )
     * @constant    len     Length of name (number of characters in name) not including null value if null-terminated string
     * @constant    name    Name of color (no null terminator), must be in UTF-16 Big Endian
     */
    typedef struct __aco_struct_rgb {
        uint16_t R;
        uint16_t G;
        uint16_t B;
        size_t len;         // [Optional] pass 0 if you don't want to assign a name to the color
        uint16_t* name;     // [Optional] name will be ignored if len is 0
    } aco_struct_rgb;
    /**
     * @brief   encodes an array of ACO structs into a .aco file, no error reporting
     * @param   nColors     size_t
     * @param   colors      aco_struct_v1
     * @return              vector<uint8_t>     binary data that can be written to file
     */
    std::vector<uint8_t> encodeSwatch_lab(aco_struct_lab* colors, size_t nColors);

    /**
     * @brief   encodes an array of ACO structs into a .aco file, no error reporting
     * @param   nColors     size_t
     * @param   colors      aco_struct_v2
     * @return              vector<uint8_t>     binary data that can be written to file
     */
    std::vector<uint8_t> encodeSwatch_rgb(aco_struct_rgb* colors, size_t nColors);
}

