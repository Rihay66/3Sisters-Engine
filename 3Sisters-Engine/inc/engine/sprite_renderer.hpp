#pragma once

#ifndef SPRITE_RENDERER_HPP
#define SPRITE_RENDERER_HPP

// include GLAD and GLM
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// include standard array library
#include <array>

// include shader class
#include <resourceSystems/resource_shader.hpp>

/* A static singleton Sprite Rendering Class used to 
 render 2D render primatives. This class uses given raw data to 
 represent and render a 2D primative. This class utilizes 
 batch rendering when rendering a stack of 2D render primatives. 
 All functions and resources are static and no public 
 constructor is defined.  
 !Requires a shader class and a standard sprite size 
*/
class SpriteRenderer{    
    public:
        // initialize the quad renderer which requires a loaded shader and pixel size of all quad objects
        static void InitQuad(Shader& quadShader, glm::uvec2 pixelSize = {10.0f, 10.0f});

        // initialize the line renderer which requires a loaded shader and pixel size of all line objects
        static void InitLine(Shader& lineShader, glm::uvec2 pixelSize = {10.0f, 10.0f});

        // initialize the quad and line renderer which requires two loaded shaders, and pixel sizes of all quad objects and line objects
        static void Init(Shader& quadShader, Shader& lineShader, glm::uvec2 quadPixelSize, glm::uvec2 linePixelSize);
        
        //* draw render primatives functions

        // draw a singular quad utilizing given raw data, without interpolation
        static void DrawQuad(int texIndex, glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color = glm::vec4(1.0f), const std::array<glm::vec2, 4> texCoords = textureCoordinates, const glm::vec4 vertexPositions[] = quadVertexPositions);

        // draw a single line from two given points
        static void DrawLine(glm::vec2 p0, glm::vec2 p1, glm::vec4 color = glm::vec4(1.0f));

        // draw a single wireframe of a quad utilizing raw data and lines
        static void DrawQuadWire(glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color = glm::vec4(1.0f));

        //* stack primative functions

        /* store a single quad utilizing given raw data, without interpolation
            @Requires the Flush() after this function in order to render what was stored
            @Without the Flush() stacked objects will be rendered either way, however it's behavior is undefined
        */
        static void StackQuad(int texIndex, glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color = glm::vec4(1.0f), const std::array<glm::vec2, 4> texCoords = textureCoordinates, const glm::vec4 vertexPositions[] = quadVertexPositions);
        
        /* store a single line utilizing given points
            @Requires the Flush() after this function in order to render what was stored
            @Without the Flush() stacked objects will be rendered either way, however it's behavior is undefined
        */
        static void StackLine(glm::vec2 p0, glm::vec2 p1, glm::vec4 color = glm::vec4(1.0f));

        //* flush functions

        // used to tell the GPU to render the stored quads in the buffer
        static void FlushQuads();

        // used to tell the GPU to render the stored lines in the buffer
        static void FlushLines();

        //* setter functions

        /* Set the width of all lines
            @Can't be used to individually set the width of lines
        */
        static void SetLineWidth(float width);

        /* Set the model pixel size of quads
        */
        static void SetQuadPixelSize(glm::uvec2 pixelSize);

        /* Set the model pixel size of lines
        */
        static void SetLinesPixelSize(glm::uvec2 pixelSize);

        //* getter functions

        // get the current width of all lines
        static float GetLineWidth();

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

        // data struct of standard line's vertex information
        struct LineVertex{
            glm::vec2 position;
            glm::vec4 color;
        };

        // reference to the model pixel size for quads
        static glm::uvec2 quadPixelSize;

        // reference to the model pixel size for lines
        static glm::uvec2 linePixelSize;

        // stores data of a passed in quad shader
        static Shader quadShader;

        // stores data of a passed in line shader
        static Shader lineShader;

        // stores data of a quad
        static unsigned int quadVAO, quadVBO, quadEBO;

        // stores data of a line
        static unsigned int lineVAO, lineVBO;

        // counter to track the number of vertices of quads
        static unsigned int quadIndexCount;

        // stores max number of quads as a buffer
        static QuadVertex* quadBuffer;

        // stores the amount of wanted quad buffers
        static QuadVertex* quadBufferPtr;  

        // counter to track the number of vertices of lines
        static unsigned int lineVertexCount;

        // store the width of all lines
        static unsigned int lineWidth;

        // stores max number of lines as a buffer
        static LineVertex* lineBuffer;
        
        // stores the amount of wanter line buffers
        static LineVertex* lineBufferPtr;

        // stores the amount of quads to render
        const static int 
        maxQuadCount = 10000, 
        maxQuadVertexCount = maxQuadCount * 4, 
        maxQuadIndexCount = maxQuadCount * 6;

        // stores the amount of lines to render
        const static int 
        maxLineCount = 10000,
        maxLineVertexCount = maxLineCount * 2;

        // stores maximum amount of textures there can be
        const static int maxTextureSlots = 32;
        
        // private constructor 
        SpriteRenderer() {}

        // initial setup for quad rendering, sets ups the rendering of quads and their buffer data
        static void initQuadRenderData();

        // initial setup for line rendering, sets up the rendering of lines and their buffer data
        static void initLineRenderData();

        //* primative creation functions

        // used to add a quad and be stored in to the quad buffer 
        static void createQuad(glm::vec2& position, glm::vec2& size, float& rotation, int& texIndex, glm::vec4& color, std::array<glm::vec2, 4> texCoords, const glm::vec4 vertexPositions[]);

        // used to add a line and be stored in to the line buffer
        static void createLine(glm::vec2& p0, glm::vec2& p1, glm::vec4& color);

        //* Batch functions

        // used to set the quad vertex buffers
        static void beginQuadBatch();

        // used to set the line vertex buffers
        static void beginLineBatch();
        
        // used to calculate amount of quads to be rendered, returns false for there are no quads and true for there are quads available
        static bool endQuadBatch();

        // used to calculate amount of lines to be rendered, returns false for there are no lines and true for there are lines available
        static bool endLineBatch();

        // used to clean up resources
        static void clear();

        //! Currently EXPERIMENTAL, may cause exceptions or segfaults
        // private boolean to track automatic clear()
        static bool QuadSet;
        static bool LineSet;
        static bool isAutoClearSet;
        
        // set up automatic de-allocation of loaded resources
        static void setUpAutoClear();
};

#endif