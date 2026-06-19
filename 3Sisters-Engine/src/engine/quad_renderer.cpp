#include "resourceSystems/resource_shader.hpp"
#include <engine/quad_renderer.hpp>

// standard library for debug outputs
#include <exception>
#include <iostream>

// include additional GLM library
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

// initialize static variables
const glm::vec4                     QuadRenderer::quadVertexPositions[4] = {
    {-0.5f, -0.5f, 0.0f, 1.0f},
    {0.5f, -0.5f, 0.0f, 1.0f},
    {0.5f, 0.5f, 0.0f, 1.0f},
    {-0.5f, 0.5f, 0.0f, 1.0f}};
const std::array<glm::vec2, 4>      QuadRenderer::textureCoordinates = {{
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f}
    }};;
// initialize quad graphics data
unsigned int                        QuadRenderer::quadVAO;
unsigned int                        QuadRenderer::quadVBO;
unsigned int                        QuadRenderer::quadEBO;
// initialize buffer stack
std::map<unsigned int, QuadRenderer::BufferStack> QuadRenderer::bufferStacks;

// initialize changeable shader
Shader                              QuadRenderer::quadShader;
// initialize auto clear var
bool                                QuadRenderer::isAutoClearSet = false;

void QuadRenderer::Init(Shader& s){
    // when auto clear is set, stop re-initializing rendering data
    if(isAutoClearSet){
        std::cout << "Warning: Initialization of Sprite Renderer being called more than once!\n";
        return;
    }

    // set up automatic clear()
    setUpAutoClear();
    
    // set the shader reference
    quadShader = s;

    // setup texture indexes
    setupTextureIndexing(quadShader);

    // set up rendering of quads
    initQuadRenderData(); 
}

void QuadRenderer::setupTextureIndexing(Shader& shader){
    // set up shader samples for the quad textures
    shader.Use();
    
    // grab the uniform location of 'image' in the shader, the name 'image' is explicit
    auto loc = glGetUniformLocation(shader.getID(), "image");

    // set up array to the size of the max number of textures
    int samplers[maxTextureSlots];

    // set up samplers array
    for (int i = 0; i < maxTextureSlots; i++)
    {
        samplers[i] = i;
    }

    // set up the index of the shader's texture array
    glUniform1iv(loc, maxTextureSlots, samplers);
}

void QuadRenderer::DrawQuad(int texIndex, glm::vec2 pos, glm::vec2 size, float rot, glm::vec4 color,const std::array<glm::vec2, 4> texCoords ,const glm::vec4 vertexPositions[]){
    //? check if buffer hasn't been set up
    if(bufferStacks[0].buffer == nullptr){
        //! Display error
        std::cout << "ERROR: Missing quad render buffer initialization!\n";
        return; // stop function
    }

    // init the buffer
    beginQuadBatch(0);

    // create the quad to render, use default buffer stack and shader
    createQuad(0, quadShader,pos, size, rot, texIndex, color, texCoords, vertexPositions);

    // render
    FlushQuads();
}

void QuadRenderer::DrawQuad(Shader& shader, int texIndex, glm::vec2 pos, glm::vec2 size, float rot, glm::vec4 color,const std::array<glm::vec2, 4> texCoords ,const glm::vec4 vertexPositions[]){
    //? check if buffer hasn't been set up
    if(bufferStacks[0].buffer == nullptr){
        //! Display error
        std::cout << "ERROR: Missing quad render buffer initialization!\n";
        return; // stop function
    }

    // init the buffer
    beginQuadBatch(0);

    // create the quad to render, use default buffer stack and given shader
    createQuad(0, shader,pos, size, rot, texIndex, color, texCoords, vertexPositions);

    // render
    FlushQuads(shader);
}   

void QuadRenderer::StackQuad(int texIndex, glm::vec2 pos, glm::vec2 size, float rot, glm::vec4 color, const std::array<glm::vec2, 4> texCoords, const glm::vec4 vertexPositions[]){
    //? check if buffer hasn't been set up
    if(bufferStacks[0].buffer == nullptr){
        //! Display error
        std::cout << "ERROR: Missing quad render buffer initialization!\n";
        return; // stop function
    }

    // check if the buffer pointer hasn't been set up
    if(bufferStacks[0].bufferPtr == nullptr){
        // then initialize the batch, use default selection
        beginQuadBatch(0);
    }

    // if not then add a quad to the buffer pointer

    // create the quad to render, use default buffer stack and shader
    createQuad(0, quadShader,pos, size, rot, texIndex, color, texCoords, vertexPositions);
}

void QuadRenderer::StackQuad(unsigned int bufferSelectionID, int texIndex, glm::vec2 pos, glm::vec2 size, float rot, glm::vec4 color, const std::array<glm::vec2, 4> texCoords, const glm::vec4 vertexPositions[]){
    //? check if stack already exists
    try{   
        // attempt to access buffer
        if(bufferStacks.at(bufferSelectionID).buffer == nullptr){
            //std::cout << "MSG: new buffer stack made\n";
            //? initialize buffer stack
            bufferStacks[bufferSelectionID].buffer = new QuadVertex[maxQuadVertexCount];

            //std::cout << "MSG: Buffer Stack size: " << bufferStacks.size() << "\n";
        }
    }catch(const std::out_of_range& e){
        // upon out of range exception then create new buffer selection
        bufferStacks.insert({bufferSelectionID, BufferStack{.buffer = new QuadVertex[maxQuadVertexCount]}});
        // debug
        //std::cout << "MSG: Created a new buffer stack with selection ID of: " << bufferSelectionID << "\n";
    }
    catch(...){
        // not a out of range exception
        //! print out error
        std::cout << "ERROR: Missing quad render buffer initialization!\n";

        return; // stop function
    }
    
    //? check if buffer hasn't been set up
    if(bufferStacks[bufferSelectionID].buffer == nullptr){
        //! Display error
        std::cout << "ERROR: Missing quad render buffer initialization!\n";
        return; // stop function
    }

    // check if the buffer pointer hasn't been set up
    if(bufferStacks[bufferSelectionID].bufferPtr == nullptr){
        // then initialize the batch, use default selection
        beginQuadBatch(bufferSelectionID);
    }

    // if not then add a quad to the buffer pointer

    // create the quad to render, use default buffer stack and shader
    createQuad(bufferSelectionID, quadShader,pos, size, rot, texIndex, color, texCoords, vertexPositions);
}


void QuadRenderer::StackQuad(unsigned int bufferSelectionID, Shader& shader, int texIndex, glm::vec2 pos, glm::vec2 size, float rot, glm::vec4 color, const std::array<glm::vec2, 4> texCoords, const glm::vec4 vertexPositions[]){
    //? check if stack already exists
    try{   
        // attempt to access buffer
        if(bufferStacks.at(bufferSelectionID).buffer == nullptr){
            //std::cout << "MSG: new buffer stack made\n";
            //? initialize buffer stack
            bufferStacks[bufferSelectionID].buffer = new QuadVertex[maxQuadVertexCount];

            //std::cout << "MSG: Buffer Stack size: " << bufferStacks.size() << "\n";
        }
    }catch(const std::out_of_range& e){
        // upon out of range exception then create new buffer selection
        bufferStacks.insert({bufferSelectionID, BufferStack{.buffer = new QuadVertex[maxQuadVertexCount]}});
        // debug
        //std::cout << "MSG: Created a new buffer stack with selection ID of: " << bufferSelectionID << "\n";
    }
    catch(...){
        // not a out of range exception
        //! print out error
        std::cout << "ERROR: Missing quad render buffer initialization!\n";

        return; // stop function
    }
    
    //? check if buffer hasn't been set up
    if(bufferStacks[bufferSelectionID].buffer == nullptr){
        //! Display error
        std::cout << "ERROR: Missing quad render buffer initialization!\n";
        return; // stop function
    }

    // check if the buffer pointer hasn't been set up
    if(bufferStacks[bufferSelectionID].bufferPtr == nullptr){
        // then initialize the batch, use default selection
        beginQuadBatch(bufferSelectionID);
    }

    // if not then add a quad to the buffer pointer

    // create the quad to render, use default buffer stack and shader
    createQuad(bufferSelectionID, shader,pos, size, rot, texIndex, color, texCoords, vertexPositions);
}

void QuadRenderer::FlushQuads(Shader& shader, unsigned int bufferSelectionID){
    //? check if buffer hasn't been set up
    if(bufferStacks[bufferSelectionID].buffer == nullptr){
        //! Display error
        //std::cout << "ERROR: Missing quad render buffer initialization!\n";
        return; // stop function
    }

    // set up vertex dynamic buffer
    if(!endQuadBatch(bufferSelectionID)){
        // there are no quads to render
        //! Display Warning
        //TODO: Make a debug option to show this warning
        //std::cout << "WARNING: No quad added to draw!\n";
        return; // stop function
    }
    
    // ensure shader usage
    shader.Use();

    // draw the quad/s
    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, bufferStacks[bufferSelectionID].indexCount, GL_UNSIGNED_INT, nullptr);

    // reset buffer pointer
    bufferStacks[bufferSelectionID].bufferPtr = nullptr;

    // reset index count
    bufferStacks[bufferSelectionID].indexCount = 0;
}

void QuadRenderer::createQuad(unsigned int bufferSelectionID, Shader& shader, glm::vec2& pos, glm::vec2& size, float& rotation, int& texIndex, glm::vec4& color, const std::array<glm::vec2, 4> texCoords,const glm::vec4 vertexPositions[]){
    // obtain reference to buffer stack
    BufferStack* stack = &bufferStacks[bufferSelectionID];
    
    // check if given stack is valid
    if(stack == nullptr || stack->buffer == nullptr){
        //! print error
        std::cout << "ERORR: Given an NULL buffer stack for creating quads!\n" <<  std::endl;
        return; // stop function
    }

    // check if not over the index count
    if (stack->indexCount >= maxQuadIndexCount){
        // flush what's left and start another batch
        FlushQuads(shader, bufferSelectionID);  
        beginQuadBatch(bufferSelectionID);
    }

    // create model transform
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f)) 
    * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) 
    * glm::scale(glm::mat4(1.0f), {size.x, size.y, 0.0f});

    stack->bufferPtr->position = (transform * vertexPositions[0]);
    stack->bufferPtr->texCoords = texCoords[0];
    stack->bufferPtr->texIndex = texIndex;
    stack->bufferPtr->color = color;
    stack->bufferPtr++;

    stack->bufferPtr->position = (transform * vertexPositions[1]);
    stack->bufferPtr->texCoords = texCoords[1];
    stack->bufferPtr->texIndex = texIndex;
    stack->bufferPtr->color = color;
    stack->bufferPtr++;

    stack->bufferPtr->position = (transform * vertexPositions[2]);
    stack->bufferPtr->texCoords = texCoords[2];
    stack->bufferPtr->texIndex = texIndex;
    stack->bufferPtr->color = color;
    stack->bufferPtr++;

    stack->bufferPtr->position = (transform * vertexPositions[3]);
    stack->bufferPtr->texCoords = texCoords[3];
    stack->bufferPtr->texIndex = texIndex;
    stack->bufferPtr->color = color;
    stack->bufferPtr++;

    stack->indexCount += 6;
}

// Set up the quad rendering
void QuadRenderer::initQuadRenderData(){
    // create default buffer stack
    bufferStacks.insert({0, BufferStack{.buffer = new QuadVertex[maxQuadVertexCount]}});

    // indices buffer data
    unsigned int indices[maxQuadIndexCount];
    unsigned int offset = 0;
    for (size_t i = 0; i < maxQuadIndexCount; i += 6){
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;

        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    }

    // check opengl version
    if(GLAD_GL_VERSION_4_5){
        // configure VAO/VBO/EBO
        glCreateVertexArrays(1, &quadVAO);
        glCreateBuffers(1, &quadVBO);
        glCreateBuffers(1, &quadEBO);

        glNamedBufferData(quadVBO, sizeof(QuadVertex) * maxQuadVertexCount, nullptr, GL_DYNAMIC_DRAW);
        glNamedBufferData(quadEBO, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexArrayVertexBuffer(quadVAO, 0, quadVBO, 0, sizeof(QuadVertex));
        glVertexArrayElementBuffer(quadVAO, quadEBO);

        // vertex attribute
        glEnableVertexArrayAttrib(quadVAO, 0);
        glVertexArrayAttribBinding(quadVAO, 0, 0);
        glVertexArrayAttribFormat(quadVAO, 0, 2, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, position));

        // texture coordinates attribute
        glEnableVertexArrayAttrib(quadVAO, 1);
        glVertexArrayAttribBinding(quadVAO, 1, 0);
        glVertexArrayAttribFormat(quadVAO,1, 2, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, texCoords));

        // texture index attribute
        glEnableVertexArrayAttrib(quadVAO, 2);
        glVertexArrayAttribBinding(quadVAO, 2, 0);
        glVertexArrayAttribFormat(quadVAO, 2, 1, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, texIndex));

        // color attribute
        glEnableVertexArrayAttrib(quadVAO, 3);
        glVertexArrayAttribBinding(quadVAO, 3, 0);
        glVertexArrayAttribFormat(quadVAO, 3, 4, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, color));
    }else{
        // configure VAO/VBO/EBO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);

        glBindVertexArray(quadVAO);

        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertex) * maxQuadVertexCount, nullptr, GL_DYNAMIC_DRAW);

        // vertex attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void *)offsetof(QuadVertex, position));

        // texture coordinates attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void *)offsetof(QuadVertex, texCoords));

        // texture index attribute
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void *)offsetof(QuadVertex, texIndex));

        // color attribute
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void *)offsetof(QuadVertex, color));
        
        glGenBuffers(1, &quadEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }
}

void QuadRenderer::beginQuadBatch(unsigned int bufferSelectionID){
    // set buffer pointer for a buffer stack
    bufferStacks[bufferSelectionID].bufferPtr = bufferStacks[bufferSelectionID].buffer;
}

bool QuadRenderer::endQuadBatch(unsigned int bufferSelectionID){
    // calculate amount of quads to render
    GLsizeiptr size = (uint8_t*)bufferStacks[bufferSelectionID].bufferPtr 
                            - (uint8_t*)bufferStacks[bufferSelectionID].buffer;
    if(size < 0){
        // no quads available
        return false;
    }

    // check opengl version
    if(GLAD_GL_VERSION_4_5){
        // set up dynamic buffer
        glNamedBufferSubData(quadVBO, 0, size, (uint8_t*)bufferStacks[bufferSelectionID].buffer);
    }else{
        // set up dynamic buffer
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, (uint8_t*)bufferStacks[bufferSelectionID].buffer);
    }

    // batch is fully set up
    return true;
}

void QuadRenderer::clear(){
    // delete all quad buffer stacks
    for(auto& iter : bufferStacks){
        delete [] iter.second.buffer;
        iter.second.bufferPtr = nullptr;
        delete iter.second.bufferPtr;
    }

    // delete quad buffer data
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteBuffers(1, &quadEBO);
}

void QuadRenderer::setUpAutoClear(){
    // set up on exit to call the Clear()
    if(!isAutoClearSet && std::atexit(clear) == 0){
        isAutoClearSet = true; // disable calling this function again
    }
}