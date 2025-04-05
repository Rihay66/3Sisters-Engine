#pragma once

#ifndef RESOURCE_TYPES_HPP
#define RESOURCE_TYPES_HPP

// include standard library
#include <array>

// include GLM
#include <glm/glm.hpp>

// include stb_truetype
#include <stb/stb_truetype.h>

// Font Atlas settings:
const uint32_t codePointOfFirstChar = 32;      // ASCII of ' '(Space)
const uint32_t charsToIncludeInFontAtlas = 95; // Include 95 charecters

// holds font data required for rendering characters
struct CharacterSet{
    stbtt_packedchar packedChars[charsToIncludeInFontAtlas];
    stbtt_aligned_quad alignedQuads[charsToIncludeInFontAtlas];
    unsigned int texID;
    float fontSize;
};

// define a sub texture that contains oordinates of a specific texture
struct SubTexture {
    std::array<glm::vec2, 4> TexCoords;
};

#endif 
