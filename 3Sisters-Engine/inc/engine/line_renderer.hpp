#pragma once

#ifndef LINE_RENDERER_HPP
#define LINE_RENDERER_HPP

// include GLAD and GLM
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// include shader class
#include <resourceSystems/resource_shader.hpp>

/* A static singleton Line Rendering Class used to 
 render 2D render primatives. This class uses given raw data to 
 represent and render a 2D primative. This class utilizes 
 batch rendering when rendering a stack of 2D render primatives. 
 All functions and resources are static and no public 
 constructor is defined.  
 !Requires a shader class to be given
*/
class LineRenderer{    
    public:
        // initialize the line renderer which requires a loaded shader
        static void Init(Shader& lineShader);

        //* draw render primatives functions

        // draw a single line from two given points
        static void DrawLine(glm::vec2 p0, glm::vec2 p1, glm::vec4 color = glm::vec4(1.0f));

        // draw a single wireframe of a quad utilizing raw data and lines
        static void DrawQuadWire(glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color = glm::vec4(1.0f));

        //* stack primative functions

        /* store a single line utilizing given points
            @Requires the Flush() after this function in order to render what was stored
            @Without the Flush() stacked objects will be rendered either way, however it's behavior is undefined
        */
        static void StackLine(glm::vec2 p0, glm::vec2 p1, glm::vec4 color = glm::vec4(1.0f));

        //* flush functions

        // used to tell the GPU to render the stored lines in the buffer
        static void FlushLines();

        //* setter functions

        /* Set the width of all lines
            @Can't be used to individually set the width of lines
        */
        static void SetLineWidth(float width);

        //* getter functions

        // get the current width of all lines
        static float GetLineWidth();

    private:
        // used to store default offsets of quad vertex positions
        const static glm::vec4 quadVertexPositions[];

        // data struct of standard line's vertex information
        struct LineVertex{
            glm::vec2 position;
            glm::vec4 color;
        };

        // stores data of a passed in line shader
        static Shader lineShader;

        // stores data of a line
        static unsigned int lineVAO, lineVBO;

        // counter to track the number of vertices of lines
        static unsigned int lineVertexCount;

        // store the width of all lines
        static unsigned int lineWidth;

        // stores max number of lines as a buffer
        static LineVertex* lineBuffer;
        
        // stores the amount of wanter line buffers
        static LineVertex* lineBufferPtr;

        // stores the amount of lines to render
        const static int 
        maxLineCount = 10000,
        maxLineVertexCount = maxLineCount * 2;

        // private constructor 
        LineRenderer() {}

        // initial setup for line rendering, sets up the rendering of lines and their buffer data
        static void initLineRenderData();

        //* primative creation functions

        // used to add a line and be stored in to the line buffer
        static void createLine(glm::vec2& p0, glm::vec2& p1, glm::vec4& color);

        //* Batch functions

        // used to set the line vertex buffers
        static void beginLineBatch();

        // used to calculate amount of lines to be rendered, returns false for there are no lines and true for there are lines available
        static bool endLineBatch();

        // used to clean up resources
        static void clear();

        //! Currently EXPERIMENTAL, may cause exceptions or segfaults
        // private boolean to track automatic clear()
        static bool isAutoClearSet;
        
        // set up automatic de-allocation of loaded resources
        static void setUpAutoClear();
};

#endif