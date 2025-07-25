#include <engine/text_renderer.hpp>

// additional GLM libraries
#include <glm/gtc/matrix_transform.hpp>

// standard library for debug outputs
#include <iostream>

// include TextureManager
#include <resourceSystems/managers/texture_manager.hpp>


#include <cstddef>

// initialize static variables
const int TextRenderer::order[6] = { 0, 1, 2, 0, 2, 3 }; 

// init character vertex buffers
TextRenderer::CharacterVertex*      TextRenderer::characterVertexBuffer = nullptr;
TextRenderer::CharacterVertex*      TextRenderer::characterVertexBufferPtr = nullptr;

// init character graphics data
unsigned int    TextRenderer::VAO;
unsigned int    TextRenderer::VBO;
unsigned int    TextRenderer::charVertexCount = 0;
unsigned int    TextRenderer::window_height = 0;

// initialize changeable shader
Shader          TextRenderer::textShader;

// initialize auto clear var
bool            TextRenderer::isAutoClearSet = false;

//TODO: Refactor text renderer to use a batch rendering solution

void TextRenderer::Init(Shader& shader, unsigned int height){
    // when auto clear is set, stop re-initializing rendering data
    if(isAutoClearSet){
        std::cout << "Warning: Initialization of Text Renderer being called more than once!\n";
        return;
    }

    // set up auto clear
    setUpAutoClear();
    
    // set window height var
    window_height = height;

    // set shader
    textShader = shader;
    
    // set expplicit texture slot in shader
    textShader.Use();
    int uniformLoc = glGetUniformLocation(textShader.getID(), "text");
    glUniform1i(uniformLoc, 0);

    // configure VAO/VBO for positioning and texturing
    initTextRenderingData();
}

void TextRenderer::DrawCharacters(CharacterSet& set, std::string text, glm::vec3 position, float rotation, float size, glm::vec4 color){
    //? check if buffer hasn't been set up
    if(characterVertexBuffer == nullptr){
        //! Display error
        std::cout << "ERROR: Missing render buffer initialization!\n";
        return; // stop function
    }
    
    // check opengl version
    if(GLAD_GL_VERSION_4_5){
        // bind texture
        glBindTextureUnit(0, set.texID);
    }else{
        // bind texture
        glBindTexture(GL_TEXTURE_2D, set.texID);
        glActiveTexture(GL_TEXTURE0);
    }

    // init the buffer
    beginCharacterBatch();
    
    // create the text to render
    createCharacter(set, text, position, rotation, size, color);
    
    // render
    FlushText();
}

void TextRenderer::StackCharacters(CharacterSet &set, std::string text, glm::vec3 position, float rotation, float size, glm::vec4 color){
    //? check if buffer hasn't been set up
    if(characterVertexBuffer == nullptr){
        //! Display error
        std::cout << "ERROR: Missing render buffer initialization!\n";
        return; // stop function
    }
    
    // check if the buffer pointer hasn't been set up
    if(characterVertexBufferPtr == nullptr){
        // then initialize the batch
        beginCharacterBatch();
    }
    
    // check opengl version
    if(GLAD_GL_VERSION_4_5){
        // bind texture
        glBindTextureUnit(0, set.texID);
    }else{
        // bind texture
        glBindTexture(GL_TEXTURE_2D, set.texID);
        glActiveTexture(GL_TEXTURE0);
    }
    
    // if not then add characters to the buffer pointer
    
    // create the text to render
    createCharacter(set, text, position, rotation, size, color);
}

void TextRenderer::SetWindowHeight(unsigned int height){
    window_height = height;
}

void TextRenderer::FlushText(){
    //? check if buffer hasn't been set up
    if(characterVertexBuffer == nullptr){
        //! Display error
        std::cout << "ERROR: Missing render buffer initialization!\n";
        return; // stop function
    }
    
    // set up vertex dynamic buffer
    if(!endCharacterBatch()){
        // there are no characters to render
        //! Display Warning
        //TODO: Make a debug option to show this warning
        //std::cout << "WARNING: No characters added to draw!\n";
        return; // stop function
    }
    
    // ensure shader usage
    textShader.Use();
    
    // draw the character/s
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, charVertexCount);
    
    // reset buffer pointer
    characterVertexBufferPtr = nullptr;
    
    // reset vertex count
    charVertexCount = 0;
    
    // rebind non-font textures
    TextureManager::BindTextures();
}

void TextRenderer::createCharacter(CharacterSet& set, std::string text, glm::vec3 position, float rotation, float size, glm::vec4 color){     
    // calculate pixel scale
    float pixelScale = 2.0f / window_height;
    
    // local storage of the position for each glyph
    glm::vec3 localPosition = position;
    
    for(char ch : text){
        // cehck if the charecter glyph is in the font atlas
        if(ch >= codePointOfFirstChar && ch <= codePointOfFirstChar + charsToIncludeInFontAtlas){
            if(charVertexCount >= maxQuadVertexCount){
                // flush what's left and start another batch
                FlushText();
                beginCharacterBatch();
            }
            
            // Retrive the data that is used to render a glyph of charecter 'ch'
            stbtt_packedchar* packedChar = &set.packedChars[ch - codePointOfFirstChar]; 
            stbtt_aligned_quad* alignedQuad = &set.alignedQuads[ch - codePointOfFirstChar];
            
            // The units of the fields of the above structs are in pixels, 
            // convert them to a unit of what we want be multilplying to pixelScale  
            glm::vec2 glyphSize = {
                (packedChar->x1 - packedChar->x0) * pixelScale * size,
                (packedChar->y1 - packedChar->y0) * pixelScale * size
            };

            glm::vec2 glyphBoundingBoxBottomLeft = {
                localPosition.x + (packedChar->xoff * pixelScale * size),
                localPosition.y - (packedChar->yoff + packedChar->y1 - packedChar->y0) * pixelScale * size
            };

            // The order of vertices of a quad goes top-right, top-left, bottom-left, bottom-right
            glm::vec2 glyphVertices[4] = {
                { glyphBoundingBoxBottomLeft.x + glyphSize.x, glyphBoundingBoxBottomLeft.y + glyphSize.y },
                { glyphBoundingBoxBottomLeft.x, glyphBoundingBoxBottomLeft.y + glyphSize.y },
                { glyphBoundingBoxBottomLeft.x, glyphBoundingBoxBottomLeft.y },
                { glyphBoundingBoxBottomLeft.x + glyphSize.x, glyphBoundingBoxBottomLeft.y },
            };

            glm::vec2 glyphTextureCoords[4] = {
                { alignedQuad->s1, alignedQuad->t0 },
                { alignedQuad->s0, alignedQuad->t0 },
                { alignedQuad->s0, alignedQuad->t1 },
                { alignedQuad->s1, alignedQuad->t1 },
            };

            // create a transform to allow for rotation
            glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            
            // We need to fill the vertex buffer by 6 vertices to render a quad as we are rendering a quad as 2 triangles
            // The order used is in the 'order' array
            // order = [0, 1, 2, 0, 2, 3] is meant to represent 2 triangles: 
            // one by glyphVertices[0], glyphVertices[1], glyphVertices[2] and one by glyphVertices[0], glyphVertices[2], glyphVertices[3]
            
            characterVertexBufferPtr->position = glm::vec4(glyphVertices[order[0]], position.z, 0.0f) * transform;
            characterVertexBufferPtr->color = color;
            characterVertexBufferPtr->texCoords = glyphTextureCoords[order[0]];
            characterVertexBufferPtr++;
            
            characterVertexBufferPtr->position = glm::vec4(glyphVertices[order[1]], position.z, 0.0f) * transform;
            characterVertexBufferPtr->color = color;
            characterVertexBufferPtr->texCoords = glyphTextureCoords[order[1]];
            characterVertexBufferPtr++;
            
            characterVertexBufferPtr->position = glm::vec4(glyphVertices[order[2]], position.z, 0.0f) * transform; 
            characterVertexBufferPtr->color = color;
            characterVertexBufferPtr->texCoords = glyphTextureCoords[order[2]];
            characterVertexBufferPtr++;
            
            characterVertexBufferPtr->position = glm::vec4(glyphVertices[order[3]], position.z, 0.0f) * transform;
            characterVertexBufferPtr->color = color;
            characterVertexBufferPtr->texCoords = glyphTextureCoords[order[3]];
            characterVertexBufferPtr++;
            
            characterVertexBufferPtr->position = glm::vec4(glyphVertices[order[4]], position.z, 0.0f) * transform;
            characterVertexBufferPtr->color = color;
            characterVertexBufferPtr->texCoords = glyphTextureCoords[order[4]];
            characterVertexBufferPtr++;
            
            characterVertexBufferPtr->position = glm::vec4(glyphVertices[order[5]], position.z, 0.0f) * transform;
            characterVertexBufferPtr->color = color;
            characterVertexBufferPtr->texCoords = glyphTextureCoords[order[5]];
            characterVertexBufferPtr++;

            charVertexCount += 6;

            // Update the position to render the next glyph specified by packedChar->xadvance.
            localPosition.x += packedChar->xadvance * pixelScale * size;
        }else if(ch == '\n'){ // handle newlines seperately
            // advance y by fontSize, reset x-coordinate
            localPosition.y -= set.fontSize * pixelScale * size;
            localPosition.x = position.x;
        }
    }
}

void TextRenderer::initTextRenderingData(){
    // check if character vertex buffer had already been initialized
    if(characterVertexBuffer != nullptr){
        exit(-1); // avoid re-initialize of the render data
    }
    
    // configure buffer
    characterVertexBuffer = new CharacterVertex[maxQuadVertexCount];
    
    // check opengl version
    if(GLAD_GL_VERSION_4_5){
        // configure VAO/VBO for text
        glCreateVertexArrays(1, &VAO);
        glCreateBuffers(1, &VBO);
        
        glNamedBufferData(VBO, sizeof(CharacterVertex) * maxQuadVertexCount, nullptr, GL_DYNAMIC_DRAW);
        
        glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(CharacterVertex));
        
        // vertex attribute
        glEnableVertexArrayAttrib(VAO, 0);
        glVertexArrayAttribBinding(VAO, 0, 0);
        glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(CharacterVertex, position));
        
        // texture coordinates attribute
        glEnableVertexArrayAttrib(VAO, 1);
        glVertexArrayAttribBinding(VAO, 1, 0);
        glVertexArrayAttribFormat(VAO,1, 2, GL_FLOAT, GL_FALSE, offsetof(CharacterVertex, texCoords));
        
        // color attribute
        glEnableVertexArrayAttrib(VAO, 2);
        glVertexArrayAttribBinding(VAO, 2, 0);
        glVertexArrayAttribFormat(VAO,2, 4, GL_FLOAT, GL_FALSE, offsetof(CharacterVertex, color));
    }else{
        // configure VAO/VBO for text
        glGenVertexArrays(1, &VAO);        
        glGenBuffers(1, &VBO);
        
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(CharacterVertex) * maxQuadVertexCount, NULL, GL_DYNAMIC_DRAW);
        
        // vertex attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CharacterVertex), (const void *)offsetof(CharacterVertex, position));
        
        // texture cooridnates attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CharacterVertex), (const void *)offsetof(CharacterVertex, texCoords));
        
        // color attribute
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(CharacterVertex), (const void *)offsetof(CharacterVertex, color));
        
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void TextRenderer::beginCharacterBatch(){
    // set buffer pointer
    characterVertexBufferPtr = characterVertexBuffer;
    
    // call shader usage
    textShader.Use(); 
}

bool TextRenderer::endCharacterBatch(){
    // calculate amount of quads to render
    GLsizeiptr size = (uint8_t*)characterVertexBufferPtr - (uint8_t*)characterVertexBuffer;
    if(size < 0){
        // no quads available
        return false;
    }

    // check opengl version
    if(GLAD_GL_VERSION_4_5){
        // set up dynamic buffer
        glNamedBufferSubData(VBO, 0, size, characterVertexBuffer);
    }else{
        // set up dynamic buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, characterVertexBuffer);
    }

    // batch is fully set up
    return true;
}

void TextRenderer::clear(){
    // delete all character vertex buffers
    delete[] characterVertexBuffer;
    characterVertexBufferPtr = nullptr;
    delete characterVertexBufferPtr;
    
    // delete quad buffer data 
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void TextRenderer::setUpAutoClear(){
    // set up on exit to call the Clear()
    if(!isAutoClearSet && std::atexit(clear) == 0){
        isAutoClearSet = true; // disable calling this function again
    }
}