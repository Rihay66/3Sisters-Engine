#include "glad/glad.h"
#include <cstdint>
#include <resourceSystems/managers/texture_manager.hpp>

#include <cstring>
#include <array>
#include <iostream>
#include <fstream>

// include STB headers
#include <stb/stb_truetype.h>
#include <stb/stb_image_write.h>
#include <stb/stb_image.h>

// instantiate static variables

std::map<std::string, Texture>                                              TextureManager::Textures;
std::map<std::string, CharacterSet>                                         TextureManager::Fonts;
std::map<std::string, SubTexture>                                           TextureManager::SubTextures;
std::vector<unsigned int>                                                   TextureManager::texIDList;
bool                                                                        TextureManager::doesWhiteTexExist = false;
bool                                                                        TextureManager::isAutoClearSet = false;

Texture& TextureManager::LoadTexture(const char *file, std::string name, bool isLinear){
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

CharacterSet& TextureManager::LoadFontTexture(const char *file, std::string name, uint32_t fontAtlasWidth, uint32_t fontAtlasHeight,  float fontSize, bool isLinear){
    // set up auto clear
    setUpAutoClear();
    
    // create characters container
    CharacterSet chars;
    
    // load font
    uint8_t* data = loadFontFromFile(&chars, file, fontAtlasWidth, fontAtlasHeight, fontSize);
    
    // check opengl version
    if(GLAD_GL_VERSION_4_5){
        // geenrate texture
        glCreateTextures(GL_TEXTURE_2D, 1, &chars.texID);
        
        // create texture for GPU
        glTextureStorage2D(chars.texID, 1, GL_R8, fontAtlasWidth, fontAtlasHeight);
        glTextureSubImage2D(chars.texID, 0, 0, 0, fontAtlasWidth, fontAtlasHeight, GL_RED, GL_UNSIGNED_BYTE, data);
        
        // set texture options
        glTextureParameteri(chars.texID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(chars.texID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        //Check for linear flag then set the texture filter
        if(isLinear){
            glTextureParameteri(chars.texID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(chars.texID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }else{
            glTextureParameteri(chars.texID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureParameteri(chars.texID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
    }else{
        // generate ID
        glGenTextures(1, &chars.texID);
        // bind texture
        glBindTexture(GL_TEXTURE_2D, chars.texID);
        // create texture for GPU
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, fontAtlasWidth, fontAtlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, data);
        
        // set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // check for filter option
        if(isLinear){
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }else{
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        
        // unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    // add texture to font textures
    Fonts[name] = chars;
    
    // free loaded font data 
    delete[] data;
    
    // rebind non-font textures
    BindTextures();
    
    return Fonts[name];
}

std::array<glm::vec2, 4>& TextureManager::GenerateSubTexture(std::string name, Texture& texture, glm::uvec2 coordinates, glm::uvec2 cellSize, glm::uvec2 spriteSize){
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

void TextureManager::GenerateWhiteTexture(){
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

int TextureManager::GetTextureIndex(std::string name){

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

Texture& TextureManager::GetTexture(std::string name){
    return Textures[name];
}

CharacterSet& TextureManager::GetFontTexture(std::string name){
    return Fonts[name];
}

std::array<glm::vec2, 4>& TextureManager::GetSubTexture(std::string name){
    return SubTextures[name].TexCoords;
}

bool TextureManager::BindTextures(){
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

Texture TextureManager::loadTextureFromFile(const char *file, bool alpha, bool isLinear){
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

uint8_t* TextureManager::loadFontFromFile(CharacterSet* chars, const char* file, uint32_t fontAtlasWidth, uint32_t fontAtlasHeight, float fontSize){
    // load font file
    std::ifstream inputFileStream(file, std::ios::binary);
    
    // find the size of the file to store memory dynamically
    inputFileStream.seekg(0, std::ios::end);
    auto&& size = inputFileStream.tellg();
    inputFileStream.seekg(0, std::ios::beg);

    // allocate the buffer
    uint8_t* fontDataBuf = new uint8_t[static_cast<size_t>(size)];

    // read the font data to the buffer
    inputFileStream.read((char*)fontDataBuf, size);
    
    //? debug, print out font file info
    uint32_t fontCount = stbtt_GetNumberOfFonts(fontDataBuf);
    std::cout << "MSG: Font File: " << file << " contains: " << fontCount << " fonts\n";
    
    // initialize stb
    stbtt_fontinfo fontInfo = {};
    
    if(!stbtt_InitFont(&fontInfo, fontDataBuf, 0)){
        std::cout << "ERROR: Failed to initialize stbtt_InitFont!\n";
        exit(-1);
    }
    
    // allocate the bitmap texture
    uint8_t* fontAtlasTextureData = new uint8_t[fontAtlasWidth * fontAtlasHeight];
    
    // append font data into the bitmap texture
    stbtt_pack_context ctx;
    
    stbtt_PackBegin(
        &ctx,                                     // stbtt_pack_context (this call will initialize it) 
        (unsigned char*)fontAtlasTextureData,     // Font Atlas texture data
        fontAtlasWidth,                           // Width of the font atlas texture
        fontAtlasHeight,                          // Height of the font atlas texture
        0,                                        // Stride in bytes
        1,                                        // Padding between the glyphs
        nullptr);

    stbtt_PackFontRange(
        &ctx,                                     // stbtt_pack_context
        fontDataBuf,                              // Font Atlas texture data
        0,                                        // Font Index                                 
        fontSize,                                 // Size of font in pixels. (Use STBTT_POINT_SIZE(fontSize) to use points) 
        codePointOfFirstChar,                     // Code point of the first charecter
        charsToIncludeInFontAtlas,                // No. of charecters to be included in the font atlas 
        chars->packedChars                    // stbtt_packedchar array, this struct will contain the data to render a glyph
    );
    stbtt_PackEnd(&ctx);
    
    // store font size on the character set
    chars->fontSize = fontSize;

    for (int i = 0; i < charsToIncludeInFontAtlas; i++){
        // define reusable vars
        float unusedX, unusedY;
        
        stbtt_GetPackedQuad(
            chars->packedChars,              // Array of stbtt_packedchar
            fontAtlasWidth,                      // Width of the font atlas texture
            fontAtlasHeight,                     // Height of the font atlas texture
            i,                                   // Index of the glyph
            &unusedX, &unusedY,                  // current position of the glyph in screen pixel coordinates, (not required as we have a different corrdinate system)
            &chars->alignedQuads[i],         // stbtt_alligned_quad struct. (this struct mainly consists of the texture coordinates)
            0                                    // Allign X and Y position to a integer (doesn't matter because we are not using 'unusedX' and 'unusedY')
        );
    }

    // free any memory        
    delete[] fontDataBuf;
    
    //? debug, create an PNG image file of the font
    //stbi_write_png("fontAtlas.png", fontAtlasWidth, fontAtlasHeight, 1, fontAtlasTextureData, fontAtlasWidth);
    
    // return texture data
    return fontAtlasTextureData;
}

void TextureManager::clear(){
    // (properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.GetID());
    // (properly) delete all font textures
    for(auto iter : Fonts){
        glDeleteTextures(1, &iter.second.texID);
    }
}

void TextureManager::setUpAutoClear(){
    // set up on exit to call the Clear()
    if(!isAutoClearSet && std::atexit(clear) == 0){
        isAutoClearSet = true; // disable calling this function again
    }
}
