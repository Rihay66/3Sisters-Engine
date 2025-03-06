#pragma once 

#ifndef TEXT_RENDERING_HPP
#define TEXT_RENDERING_HPP

// include standard libraries
#include <string>
#include <map>

// include GLAD and GLM
#include <glad/glad.h>
#include <glm/glm.hpp>

// include resource types
#include <resourceSystems/resource_types.hpp>
#include <resourceSystems/resource_shader.hpp>

/* A static singleton Text Renderer Class used to render text
 with a given font. This Class utilizes fonts loaded from the 
 ResourceManager Class. All functions and resources are static 
 and no public constructor is defined.  
 @ This Class is unoptimized and not very flexible thus take into consideration of its current design
*/
class TextRenderer{
    public:        
        // initialize the text renderer which requires the width and height of the display area and a loaded shader
        static void Init(Shader& shader);

        /* used to draw text on the screen using a loaded font
            @ Recommended to use a font loaded through from the ResourceManager
        */ 
        static void DrawString(std::map<char, Character>& chars, std::string text, glm::vec2 position, glm::vec2 scale, glm::vec4 color = glm::vec4(1.0f));
    
    private:
        // stores data of a quad
        static unsigned int VAO, VBO;

        // initial setup of the rendering data
        static void initTextRenderingData();

        // shader used for text rendering
        static Shader textShader;

        // private constructor       
        TextRenderer() {}

        // used to clean up resources
        static void clear();

        //! Currently EXPERIMENTAL, may cause exceptions or segfaults
        // private boolean to track automatic clear()
        static bool isAutoClearSet;
        // set up automatic de-allocation of loaded resources
        static void setUpAutoClear();
};

#endif