#pragma once

#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

// include GLM
#include <glm/glm.hpp>

// 3D transform component
struct Transform{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 size;
};

// 2D transform component
struct Transform2D{
    glm::vec2 position;
    float rotation;
    glm::vec2 size;
};

#endif