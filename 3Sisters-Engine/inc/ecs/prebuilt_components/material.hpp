#pragma once

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

// include GLM
#include <glm/glm.hpp>

// include standard array library
#include <array>

// 2D material component
struct Material2D {
    int texIndex;
    std::array<glm::vec2, 4> texCoords = {{
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f}
    }};
    glm::vec4 color = glm::vec4(1.0f);
};

#endif
