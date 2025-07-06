#pragma once

#ifndef SHADER_MANAGER_HPP
#define SHADER_MANAGER_HPP

// include standard libraries
#include <map>
#include <string>

// include necessary classes such as texture and shader classes
#include <resourceSystems/resource_shader.hpp>

// include resource definitions
#include <resourceSystems/resource_types.hpp>

/* A static singleton Resource Shader Manager class that 
 hosts several functions to load Shaders. Each loaded texture
 and/or shader is also stored for future reference by string
 handles. All functions and resources are static and no
 public constructor is defined.
*/
class ShaderManager{
    public:
        //* loader functions

        /*  loads (and generates) a shader program from file along with a name, loads the files in this order of vertex, fragment (and geometry) and extracts the shader's source code
        * @NOTE: it is optional to load a geometry shader file, if there is no geometry shader then set as nullptr
        */
        static Shader& LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name);

        //* getter functions

        // retrieves a stored shader
        static Shader& GetShader(std::string name);
        
    private:
        // private resource storage
        static std::map<std::string, Shader> Shaders;
        
        // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
        ShaderManager() {}
        // loads and generates a shader from file
        static Shader loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
        // properly de-allocates all loaded resources
        static void clear();
        
        //! Currently EXPERIMENTAL, may cause exceptions or segfaults
        // private boolean to track automatic clear()
        static bool isAutoClearSet;
        // set up automatic de-allocation of loaded resources
        static void setUpAutoClear();
};

#endif
