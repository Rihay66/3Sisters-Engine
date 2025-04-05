#pragma once

#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

// include standard libraries
#include <map>
#include <string>
#include <vector>
#include <array>
#include <cstdint>

// include necessary classes such as texture and shader classes
#include <resourceSystems/resource_texture.hpp>

// include resource definitions
#include <resourceSystems/resource_types.hpp>

/* A static singleton Resource Texture Manager class that 
 hosts several functions to load Textures. Each loaded texture
 and/or shader is also stored for future reference by string
 handles. All functions and resources are static and no
 public constructor is defined.
*/
class TextureManager{
    public:
        //* loader functions

        /* loads (and generates) a texture from file along with a name and optional texture filter option
        * @NOTE: depending on the file type alpha is automatically set
        * @NOTE: by default the texture's filter is set to be nearest and it is optional to set it to true which sets to be linear
        */
        static Texture& LoadTexture(const char *file, std::string name, bool linearFilter = false);

        /* loads (and generates) a font from file with a font size along with a name and optional texture filter option
        * @NOTE: by default the texture's filter is set to be nearest and it is optional to set it to true which sets to be linear
        */
        static CharacterSet& LoadFontTexture(const char* file, std::string name, uint32_t fontAtlasWidth, uint32_t fontAtlasHeight,  float fontSize, bool linearFilter = false);
        
        // use a loaded texture to create a sub texture along with a name
        static std::array<glm::vec2, 4>& GenerateSubTexture(std::string name, Texture& texture, glm::uvec2 coordinates, glm::uvec2 cellSize, glm::uvec2 spriteSize = {1, 1});
        
        // create a white texture that is named "default"
        static void GenerateWhiteTexture();

        //* getter functions

        // retrieves a stored texture's ID index
        static int GetTextureIndex(std::string name);

        // retrieves a stored texture
        static Texture& GetTexture(std::string name);
        
        // retrieves a stored font texture
        static CharacterSet& GetFontTexture(std::string name);

        // retrieves a stored font texture map that contains characters and associated font character
        
        // retrieve a stored sub texture
        static std::array<glm::vec2, 4>& GetSubTexture(std::string name);

        //* helper functions

        /* binds all textures (except not fonts) from the texture list to be used by OpenGL
        * @NOTE: is automatically called by LoadTexture()
        */
        static bool BindTextures();
        
    private:
        //TODO: Switch to use std::array
        static std::map<std::string, Texture> Textures;
        static std::map<std::string, CharacterSet> Fonts;
        static std::map<std::string, SubTexture> SubTextures;
        static std::vector<unsigned int> texIDList;
        // track (the caveman way) if the white texture has been generated 
        static bool doesWhiteTexExist;
        
        // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
        TextureManager() {}
        // loads a single texture from file
        static Texture loadTextureFromFile(const char *file, bool alpha, bool isLinear);
        // loads a single font from file
        static uint8_t* loadFontFromFile(CharacterSet* chars, const char* file, uint32_t fontAtlasWidth, uint32_t fontAtlasHeight,  float fontSize);
        // properly de-allocates all loaded resources
        static void clear();
        
        //! Currently EXPERIMENTAL, may cause exceptions or segfaults
        // private boolean to track automatic clear()
        static bool isAutoClearSet;
        // set up automatic de-allocation of loaded resources
        static void setUpAutoClear();
};

#endif
