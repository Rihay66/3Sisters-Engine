#pragma once 

#ifndef TEXT_RENDERING_HPP
#define TEXT_RENDERING_HPP

// include standard libraries
#include <string>

// include GLAD and GLM
#include <glad/glad.h>
#include <glm/glm.hpp>

// include resource types
#include <resourceSystems/resource_types.hpp>
#include <resourceSystems/resource_shader.hpp>

/* A static singleton Text Renderer Class used to render text
 with a given CharacterSet otherwise recognized simply as a font.
 This class utilizes batch rendering when rendering a stack of 
 characters.All functions and resources are static and no public 
 constructor is defined. 
*/
class TextRenderer{
    public:        
        // initialize the text renderer which requires a loaded shader and height of the display area which is used to calculate the pixel scale of the rendered text
        static void Init(Shader& shader, unsigned int window_height);
        
        //* draw render function
        
        /* used to draw text on the screen using a loaded font
            @ Recommended to use a CharacterSet loaded through from the ResourceManager
        */ 
        static void DrawCharacters(CharacterSet& set, std::string text, glm::vec3 position, float rotation, float size, glm::vec4 color = glm::vec4(1.0f));
        
        //* stack render function
        
        /* used to draw text on the screen using a loaded font
            @ Recommended to use a CharacterSet loaded through from the ResourceManager
            !If using multiple fonts, stack and flush for each particular font being used, you can't mix different fonts when stacking
        */
        static void StackCharacters(CharacterSet& set, std::string text, glm::vec3 position, float rotation, float size, glm::vec4 color = glm::vec4(1.0f));
        
        //* setter functions
        
        // set the window height, which is used to calculate the pixel scale of the rendered text
        static void SetWindowHeight(unsigned int height);
        
        //* flush functions
        
        // used to tell the GPU to render the stored characters in the stack buffer
        static void FlushText();
        
    private:
        // define vertex structure for characters
        struct CharacterVertex{
            glm::vec3 position;
            glm::vec2 texCoords;
            glm::vec4 color;
        };
        
        // storage of the text shader
        static Shader textShader;
        
        // stores data of a quad
        static unsigned int VAO, VBO, window_height;
        
        // counter to track the number of vertices of quads
        static unsigned int charVertexCount;
        
        // stores max number of vertices of character quads
        static CharacterVertex* characterVertexBuffer;
        
        // stores the amount of wanted character quads
        static CharacterVertex* characterVertexBufferPtr; 
        
        // stores the amount of quads to render
        const static int 
        maxQuadCount = 10000, 
        maxQuadVertexCount = maxQuadCount * 6;
        
        // define vertex order for characters
        const static int order[6];

        // private constructor       
        TextRenderer() {}
        
        // initial setup of the rendering data
        static void initTextRenderingData();
        
        //* primative craetion functions
        
        static void createCharacter(CharacterSet& set, std::string text, glm::vec3 position, float rotation, float size, glm::vec4 color);
        
        //* Batch functions
        
        // begin character batch
        static void beginCharacterBatch();
        
        // end character batch
        static bool endCharacterBatch();
        
        // used to clean up resources
        static void clear();

        //! Currently EXPERIMENTAL, may cause exceptions or segfaults
        // private boolean to track automatic clear()
        static bool isAutoClearSet;
        
        // set up automatic de-allocation of loaded resources
        static void setUpAutoClear();
};

#endif