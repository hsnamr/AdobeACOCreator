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
    static const uint16_t kColorSpaceHSB        = 1;
    static const uint16_t kColorSpaceCMYK       = 2;
    static const uint16_t kColorSpaceLAB        = 7;
    static const uint16_t kColorSpaceGrayscale  = 8;

    /**
     * @typedef     aco_struct_v1
     * @brief       This struct holds the information for each color
     * @constant    L       Lab value L
     * @constant    a       Lab value a
     * @constant    b       Lab value b
     */
    typedef struct aco_struct_v1 {
        float L;
        float a;
        float b;
    } aco_struct_v1;

    /**
     * @typedef     aco_struct_v2
     * @brief       This struct holds the information for each color
     * @constant    L       Lab value L
     * @constant    a       Lab value a
     * @constant    b       Lab value b
     * @constant    len     Length of name (number of characters in name) not including null value if null-terminated string
     * @constant    name    Name of color (no null terminator), must be in UTF-16 Big Endian
     */
    typedef struct __aco_struct_v2 {
        float L;
        float a;
        float b;
        size_t len;
        uint16_t* name;
    } aco_struct_v2;

    /**
     * @brief   encodes an array of ACO structs into a .aco file, no error reporting
     * @param   nColors     size_t
     * @param   colors      aco_struct_v1
     * @return              vector<uint8_t>     binary data that can be written to file
     */
    std::vector<uint8_t> encodeSwatch(aco_struct_v1* colors, size_t nColors);

    /**
     * @brief   encodes an array of ACO structs into a .aco file, no error reporting
     * @param   nColors     size_t
     * @param   colors      aco_struct_v2
     * @return              vector<uint8_t>     binary data that can be written to file
     */
    std::vector<uint8_t> encodeSwatch(aco_struct_v2* colors, size_t nColors);
}

