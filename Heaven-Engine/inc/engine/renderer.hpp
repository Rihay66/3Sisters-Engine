#pragma once

#ifndef RENDERER_HPP
#define RENDERER_HPP

//include standard lib
#include <vector>
#include <algorithm>

//include math and glad
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//include shader and texture classes
#include <gameObjs/gameObject.hpp>
#include <resourceSys/shader.hpp>

//data struct for holding vertex info
struct Vertex{ 
    glm::vec2 position;
    glm::vec2 texCoords;
    float texIndex;
    glm::vec3 color;
};

struct RendererStats{
    int quadCount = 0, drawCount = 0;
};

class Renderer{
    public:
        //constructor & desconstructor
        Renderer(Shader &shader, glm::vec2 spriteSize);
        ~Renderer();

        //draw multiple pointer objects in a vector list
        void Draw2D(std::vector<GameObject*> objs);

        //reference to the model size
        glm::vec2 spriteSize;

        //contain reference to amont of quads and amount of draw calls
        RendererStats stats;

    private:
        //stores data of a passed in shader
        Shader shader;
        //stores data of a quad
        unsigned int quadVAO;
        unsigned int quadVBO;
        unsigned int quadEBO;

        //stores the quad buffer
        Vertex* quadBuffer = nullptr;
        Vertex* quadBufferPtr = nullptr;  

        //stores the ammount of triangles to render
        unsigned int indexCount;

        const static int maxQuadCount = 10000;
        const static int maxVertexCount = maxQuadCount * 4;
        const static int maxIndexCount = maxQuadCount * 6;

        //Used to store vertex pos
        glm::vec4 quadVertexPositions[4];

        //initial setup for rendering, setups the rendering of quads and their buffer data
        void initRenderData();

        //used to draw a quad
        void createQuad(glm::vec2 pos, glm::vec2 size, float rotation, float texIndex, glm::vec3 color);

        //Used to set and unset the vertex buffers
        void beginBatch();
        void endBatch();

        //Used to tell opengl to render the triangles
        void flush();

        const void resetStats();
};

#endif