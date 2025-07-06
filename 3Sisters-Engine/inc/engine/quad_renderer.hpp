#pragma once

#ifndef QUAD_RENDERER_HPP
#define QUAD_RENDERER_HPP

// include GLAD and GLM
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// include standard array library
#include <array>

// include shader class
#include <resourceSystems/resource_shader.hpp>

/* A static singleton Quad Rendering Class used to 
 render 2D render primatives. This class uses given raw data to 
 represent and render a 2D primative. This class utilizes 
 batch rendering when rendering a stack of 2D render primatives. 
 All functions and resources are static and no public 
 constructor is defined.  
*/
class QuadRenderer{    
    public:
        // initialize the quad renderer which requires a loaded shader
        static void Init(Shader& quadShader);

        //* draw render primatives functions

        // draw a singular quad utilizing given raw data
        static void DrawQuad(int texIndex, glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color = glm::vec4(1.0f), const std::array<glm::vec2, 4> texCoords = textureCoordinates, const glm::vec4 vertexPositions[] = quadVertexPositions);

        //* stack primative functions

        /* store a single quad utilizing given raw data
            @Requires the Flush() after this function in order to render what was stored
            @Without the Flush() stacked objects will be rendered either way, however it's behavior is undefined
        */
        static void StackQuad(int texIndex, glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color = glm::vec4(1.0f), const std::array<glm::vec2, 4> texCoords = textureCoordinates, const glm::vec4 vertexPositions[] = quadVertexPositions);

        //* flush functions

        // used to tell the GPU to render the stored quads in the buffer
        static void FlushQuads();

    private:
        // used to store default offsets of quad vertex positions
        const static glm::vec4 quadVertexPositions[];
        
        // used to store default texture cooridinates
        const static std::array<glm::vec2, 4>  textureCoordinates;

        // data struct of standard quad's vertex information
        struct QuadVertex{
            glm::vec2 position;
            glm::vec2 texCoords;
            float texIndex;
            glm::vec4 color;
        };

        // storage of the quad shader
        static Shader quadShader;

        // stores data of a quad
        static unsigned int quadVAO, quadVBO, quadEBO;

        // counter to track the number of vertices of quads
        static unsigned int quadIndexCount;

        // stores max number of quads as a buffer
        static QuadVertex* quadBuffer;

        // stores the amount of wanted quad buffers
        static QuadVertex* quadBufferPtr;  

        // stores the amount of quads to render
        const static int 
        maxQuadCount = 10000, 
        maxQuadVertexCount = maxQuadCount * 4, 
        maxQuadIndexCount = maxQuadCount * 6;

        // stores maximum amount of textures there can be
        const static int maxTextureSlots = 32;
        
        // private constructor 
        QuadRenderer() {}

        // initial setup for quad rendering, sets ups the rendering of quads and their buffer data
        static void initQuadRenderData();

        //* primative creation functions

        // used to add a quad and be stored in to the quad buffer 
        static void createQuad(glm::vec2& position, glm::vec2& size, float& rotation, int& texIndex, glm::vec4& color, std::array<glm::vec2, 4> texCoords, const glm::vec4 vertexPositions[]);
        
        //* Batch functions

        // used to set the quad vertex buffers
        static void beginQuadBatch();
        
        // used to calculate amount of quads to be rendered, returns false for there are no quads and true for there are quads available
        static bool endQuadBatch();

        // used to clean up resources
        static void clear();

        //! Currently EXPERIMENTAL, may cause exceptions or segfaults
        // private boolean to track automatic clear()
        static bool isAutoClearSet;
        
        // set up automatic de-allocation of loaded resources
        static void setUpAutoClear();
};

#endif