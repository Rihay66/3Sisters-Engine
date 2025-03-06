#include <resourceSystems/managers/resource_manager.hpp>

#include <cstring>
#include <array>
#include <iostream>
#include <sstream>
#include <fstream>

// check platform and then grab the Freetype library
#ifdef __unix__ // Linux/Unix platform
    #include <freetype2/ft2build.h>
    #include FT_FREETYPE_H
#elif defined(_WIN32) || defined(WIN32) // Windows platform
    #include <ft2build.h>
    #include FT_FREETYPE_H
#endif

#include <stb/stb_image.h>

// instantiate static variables

std::map<std::string, Shader>                                               ResourceManager::Shaders;
std::map<std::string, Texture>                                              ResourceManager::Textures;
std::map<std::string, std::map<char, Character>>                            ResourceManager::Fonts;
std::map<std::string, SubTexture>                                           ResourceManager::SubTextures;
std::vector<unsigned int>                                                   ResourceManager::texIDList;
bool                                                                        ResourceManager::doesWhiteTexExist = false;
bool                                                                        ResourceManager::isAutoClearSet = false;


Shader& ResourceManager::LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name){
    // set up automatic clear()
    setUpAutoClear();
    // load the shader from the given file
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Texture& ResourceManager::LoadTexture(const char *file, std::string name, bool isLinear){
    // set up automatic clear()
    setUpAutoClear();
    // determine the alpha of the file by checking image file extension
    size_t fileNameLength = strlen(file);
    size_t fileExtension = 0;
    char fileImageformat[5];
    // find the position of the '.' in file name
    for(int i = fileNameLength - 1; i >= 0; i--){
        // check every position until finding '.'
        if(file[i] == '.'){
            fileExtension = fileNameLength - (i + 1);
            // check if the file extension is within commmon length
            if(fileExtension > (size_t)(sizeof(fileExtension)/sizeof(char))){
                std::cout << "ERROR: Unknown image format extension: " << file << "\n";
                break;
            }
            // get the file extension name
            for(int i = 0 ; i < fileExtension; i++){
                fileImageformat[i] = file[(fileNameLength - fileExtension) + i];
            }
            break;
        }
    }

    //TODO: Refactor to have a more readable way to check for certain image format that have alpha compositing

    // check file extension of the following to set alpha to be true, otherwise alpha is false
    if(strcmp(fileImageformat, "png") == 0 || strcmp(fileImageformat, "jpeg") == 0){
        // load texture with alpha
        Textures.insert({name ,loadTextureFromFile(file, true, isLinear)});
    }else{
        // load texture with no alpha
        Textures.insert({name ,loadTextureFromFile(file, false, isLinear)});
    }

    // add texture ID to list
    texIDList.push_back(Textures[name].GetID());

    //? bind the textures
    BindTextures();

    return Textures[name];
}

std::map<char, Character>& ResourceManager::LoadFontTexture(const char* filename, unsigned int fontsize, std::string name, bool isLinear){
    // set up automatic clear()
    setUpAutoClear();

    // use free type to load font and set font size

    // initialize the FreeType2 library
    FT_Library ft;
    // grab and check for errors
    FT_Error error = FT_Init_FreeType(&ft);
	if (error){
        std::cout << "ERROR: Couldn't load Freetype | Error: " << error << std::endl;
	}

    // load the font
    FT_Face face;
    // grab and check for errors
    error = FT_New_Face(ft, filename, 0, &face);
	if (error){
		std::cout << "ERROR: Couldn't load Font | Error: " << error << std::endl;
	}

    // set font size
    FT_Set_Pixel_Sizes(face, 0, fontsize);
    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // create font map
    std::map<char, Character> Characters;

    // load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++){
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)){
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph\n";
            continue;
        }

        unsigned int texture;
        // check opengl version
        if(GLAD_GL_VERSION_4_5){
            // generate texture
            glCreateTextures(GL_TEXTURE_2D, 1, &texture);
            
            glTextureStorage2D(texture, 1, GL_R8, face->glyph->bitmap.width, face->glyph->bitmap.rows);
            glTextureSubImage2D(texture, 0, 0, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
            
            // set texture options
            glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
            //Check for linear flag then set the texture filter
            if(isLinear){
                glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }else{
                glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            }
        }else{
            // generate texture
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );

            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            
            //Check for linear flag then set the texture filter
            if(isLinear){
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }else{
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            }
            
            // unbind the texture
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)};
        Characters.insert(std::pair<char, Character>(c, character));

        // check for OpenGL errors
        // check OpenGL errors
        int errorCode = glGetError();
        if(errorCode != GL_NO_ERROR && error == GL_INVALID_VALUE){
            std::cout << "ERROR: An error occured during binding texures | ERROR Code: " << errorCode << std::endl;
            std::cout << "ERROR: Texture ID: " << texture << " | Failed to be created\n";
            // skip iteration
            continue;
        }
    }

    // free Freetype resources
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // add characters to resources
    Fonts[name] = Characters;

    // rebind non-font textures
    BindTextures();

    //TODO: Create debug options for the ResourceManager class to display a console to show any errors or messages
    //Succesfully managed to load the font
    //std::cout << "MSG: Text Font loaded succesfully!\n";
    return Fonts[name];
}

std::array<glm::vec2, 4>& ResourceManager::GenerateSubTexture(std::string name, Texture& texture, glm::uvec2 coordinates, glm::uvec2 cellSize, glm::uvec2 spriteSize){
    //create sub texture
    SubTexture st;

    // calculate the offset coordinate of the sub texture
    glm::vec2 min = { (coordinates.x * cellSize.x) / (float)texture.GetWidth(), (coordinates.y * cellSize.y) / (float)texture.GetHeight()};
    glm::vec2 max = { ((coordinates.x + spriteSize.x) * cellSize.x) / (float)texture.GetWidth(), ((coordinates.y + spriteSize.y) * cellSize.y) / (float)texture.GetHeight()};

    // set each tex coord the corresponding coordinates
    st.TexCoords[0] = {min.x, min.y};
    st.TexCoords[1] = {max.x, min.y};
    st.TexCoords[2] = {max.x, max.y};
    st.TexCoords[3] = {min.x, max.y};

    // store sub texture
    SubTextures[name] = st;
    return SubTextures[name].TexCoords;
}

void ResourceManager::GenerateWhiteTexture(){
    if(!doesWhiteTexExist){
        // create white texture
        Texture whiteTexture;
        
        // create data for a white texture
        auto data = new unsigned char[16 * 16 * 4];;
        for(int i = 0; i < (int)(16 * 16 * 4); i++){
            data[i] = 255;
        }
        
        // check opengl version
        if(GLAD_GL_VERSION_4_5){
            whiteTexture.SetTextureInternalFormat(GL_RGBA8);
            whiteTexture.SetTextureImageFormat(GL_RGBA);
        }else{       
            whiteTexture.SetTextureInternalFormat(GL_RGBA);
            whiteTexture.SetTextureImageFormat(GL_RGBA);
        }
        
        // generate the texture
        whiteTexture.Generate(16, 16, data);

        //? delete generated data
        delete[] data;
        
        // add texture to storage with name "default"
        Textures.insert({"default", whiteTexture});
        
        // add texture ID to list
        texIDList.push_back(Textures["default"].GetID());

        //? bind the textures
        BindTextures();
        
        // turn off ability to call this function again
        doesWhiteTexExist = true;
    }else{
        //print a warning when texture already exists
        std::cout << "Warning: White Texture already generated!\n";
    }
}

Shader& ResourceManager::GetShader(std::string name){
    return Shaders[name];
}

int ResourceManager::GetTextureIndex(std::string name){

    //*NOTE: The check is used to prevent using this function when no texture was binded to OpenGL
    if(texIDList.size() <= 0){
        std::cout << "ERROR: No textures were binded to OpenGL!" << std::endl;
        return -1;
    }

    unsigned int id = Textures[name].GetID();

    for(int i = 0; i < texIDList.size(); i++){
        //check for id on the list and return it's location by iteration
        if(texIDList[i] == id)
            return i;//exit out and return texture index
    }

    // error the texture couldn't be found
    std::cout << "ERROR: Couldn't find texture: " << name << ", in storage!" << std::endl;
    return -1;
}

Texture& ResourceManager::GetTexture(std::string name){
    // check the name for any special characters
    return Textures[name];
}

std::map<char, Character>& ResourceManager::GetFontTexture(std::string name){
    return Fonts[name];
}

std::array<glm::vec2, 4>& ResourceManager::GetSubTexture(std::string name){
    return SubTextures[name].TexCoords;
}

bool ResourceManager::BindTextures(){

    // check if the texure list is not zero
    if(texIDList.size() <= 0){
        std::cout << "ERROR: No textures were loaded!" << std::endl;
        return false;
    }

    // bind all the textures from first to last
    for(int i = 0; i < texIDList.size(); i++){
        // check opengl version
        if(GLAD_GL_VERSION_4_5){
            // call to bind texture by their ID to an index 
            glBindTextureUnit(i, texIDList[i]);
        }else{
            //? this may cause issues with multiple textures
            // call to bind texture by their ID to an index
            glActiveTexture(GL_TEXTURE0 + i);;
            glBindTexture(GL_TEXTURE_2D, texIDList[i]);
        }
        // check OpenGL errors
        int errorCode = glGetError();
        if(errorCode != GL_NO_ERROR){
            std::cout << "ERROR: An error occured during binding texures, ERROR Code: " << errorCode << std::endl;
            std::cout << "ERROR: Texture ID: " << texIDList[i] << " | Failed to be binded to index: " << i << "\n";
            return false; 
        }
    }

    //TODO: Make a flag that allows to display warnings such as this

    return true;
}

Shader ResourceManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile){
    // retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    // create interface objs to get file
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;
    try
    {
        // open files
        vertexShaderFile.open(vShaderFile);
        fragmentShaderFile.open(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            // ensure the ifstream can throw exceptions
            geometryShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files: " << e.what() << std::endl;
        // exit with error
        exit(-1);
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();
    // now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture ResourceManager::loadTextureFromFile(const char *file, bool alpha, bool isLinear){
    // create texture object
    Texture texture;
    // set alpha
    if (alpha){
        // check opengl version
        if(GLAD_GL_VERSION_4_5){
            texture.SetTextureInternalFormat(GL_RGBA8);
            texture.SetTextureImageFormat(GL_RGBA);
        }else{        
            texture.SetTextureInternalFormat(GL_RGBA);
            texture.SetTextureImageFormat(GL_RGBA);
        }

    }
    // set filter
    if(isLinear){
        texture.SetTextureFilterMin(GL_LINEAR);
        texture.SetTextureFilterMax(GL_LINEAR);
    }

    // load image
    int width, height, nrChannels;
    // tell stb_image to flip vertically the image
    stbi_set_flip_vertically_on_load(true);
    // load the texture file
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // check file if it has been found
    if(!data){
        std::cout << "ERROR: Failed to load texture file: " << file << " !\n";
        std::cout << "ERROR: " << stbi_failure_reason() << "\n";

        // give hint to user for possible fix
        std::cout << "HINT: Make sure file or folder name is all lowercase, or check if file exists in build folder, or file format is unsupported" << std::endl;
        exit(-1);
    }
    // now generate texture
    texture.Generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}

void ResourceManager::clear(){
    // (properly) delete all shaders
    for (auto iter : Shaders) {
        glDeleteProgram(iter.second.getID());
    }

    // (properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.GetID());
    // (properly) delete all font textures
    for(auto i : Fonts){
        for(const auto& x : i.second){
            glDeleteTextures(1, &x.second.TextureID);
        }
    }
}

void ResourceManager::setUpAutoClear(){
    // set up on exit to call the Clear()
    if(!isAutoClearSet && std::atexit(clear) == 0){
        isAutoClearSet = true; // disable calling this function again
    }
}
