#include <engine/quad_renderer.hpp>

// standard library for debug outputs
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
// initialize quad buffer
QuadRenderer::QuadVertex*             QuadRenderer::quadBuffer = nullptr;
QuadRenderer::QuadVertex*             QuadRenderer::quadBufferPtr = nullptr;

// initialize quad graphics data
unsigned int                        QuadRenderer::quadVAO;
unsigned int                        QuadRenderer::quadVBO;
unsigned int                        QuadRenderer::quadEBO;
unsigned int                        QuadRenderer::quadIndexCount;

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

    // set up shader samples for the quad textures
    quadShader.Use();
    
    // grab the uniform location of 'image' in the shader, the name 'image' is explicit
    auto loc = glGetUniformLocation(quadShader.getID(), "image");

    // set up array to the size of the max number of textures
    int samplers[maxTextureSlots];

    // set up samplers array
    for (int i = 0; i < maxTextureSlots; i++)
    {
        samplers[i] = i;
    }

    // set up the index of the shader's texture array
    glUniform1iv(loc, maxTextureSlots, samplers);

    // set up rendering of quads
    initQuadRenderData(); 
}

void QuadRenderer::DrawQuad(int texIndex, glm::vec2 pos, glm::vec2 size, float rot, glm::vec4 color,const std::array<glm::vec2, 4> texCoords ,const glm::vec4 vertexPositions[]){
    //? check if buffer hasn't been set up
    if(quadBuffer == nullptr){
        //! Display error
        std::cout << "ERROR: Missing quad render quad buffer initialization!\n";
        return; // stop function
    }

    // init the buffer
    beginQuadBatch();

    // create the quad to render
    createQuad(pos, size, rot, texIndex, color, texCoords, vertexPositions);

    // render
    FlushQuads();
}

void QuadRenderer::StackQuad(int texIndex, glm::vec2 pos, glm::vec2 size, float rot, glm::vec4 color, const std::array<glm::vec2, 4> texCoords, const glm::vec4 vertexPositions[]){
    //? check if buffer hasn't been set up
    if(quadBuffer == nullptr){
        //! Display error
        std::cout << "ERROR: Missing quad render buffer initialization!\n";
        return; // stop function
    }

    // check if the buffer pointer hasn't been set up
    if(quadBufferPtr == nullptr){
        // then initialize the batch
        beginQuadBatch();
    }

    // if not then add a quad to the buffer pointer

    // create the quad to render
    createQuad(pos, size, rot, texIndex, color, texCoords, vertexPositions);
}

void QuadRenderer::FlushQuads(){
    //? check if buffer hasn't been set up
    if(quadBuffer == nullptr){
        //! Display error
        std::cout << "ERROR: Missing quad render buffer initialization!\n";
        return; // stop function
    }

    // set up vertex dynamic buffer
    if(!endQuadBatch()){
        // there are no quads to render
        //! Display Warning
        //TODO: Make a debug option to show this warning
        //std::cout << "WARNING: No quad added to draw!\n";
        return; // stop function
    }

    // draw the quad/s
    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, quadIndexCount, GL_UNSIGNED_INT, nullptr);

    // reset buffer pointer
    quadBufferPtr = nullptr;

    // reset index count
    quadIndexCount = 0;
}

void QuadRenderer::createQuad(glm::vec2& pos, glm::vec2& size, float& rotation, int& texIndex, glm::vec4& color, const std::array<glm::vec2, 4> texCoords,const glm::vec4 vertexPositions[]){
    // check if not over the index count
    if (quadIndexCount >= maxQuadIndexCount){
        // flush what's left and start another batch
        FlushQuads();  
        beginQuadBatch();
    }

    // create model transform
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f)) 
    * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) 
    * glm::scale(glm::mat4(1.0f), {size.x, size.y, 0.0f});

    quadBufferPtr->position = (transform * vertexPositions[0]);
    quadBufferPtr->texCoords = texCoords[0];
    quadBufferPtr->texIndex = texIndex;
    quadBufferPtr->color = color;
    quadBufferPtr++;

    quadBufferPtr->position = (transform * vertexPositions[1]);
    quadBufferPtr->texCoords = texCoords[1];
    quadBufferPtr->texIndex = texIndex;
    quadBufferPtr->color = color;
    quadBufferPtr++;

    quadBufferPtr->position = (transform * vertexPositions[2]);
    quadBufferPtr->texCoords = texCoords[2];
    quadBufferPtr->texIndex = texIndex;
    quadBufferPtr->color = color;
    quadBufferPtr++;

    quadBufferPtr->position = (transform * vertexPositions[3]);
    quadBufferPtr->texCoords = texCoords[3];
    quadBufferPtr->texIndex = texIndex;
    quadBufferPtr->color = color;
    quadBufferPtr++;

    quadIndexCount += 6;
}

// Set up the quad rendering
void QuadRenderer::initQuadRenderData(){
    // check if quad buffer had already been initialized
    if (quadBuffer != nullptr)
        exit(-1); // avoid re-initalize of the render data

    // configure buffer
    quadBuffer = new QuadVertex[maxQuadVertexCount];

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

void QuadRenderer::beginQuadBatch(){
    // set buffer pointer
    quadBufferPtr = quadBuffer;

    // call shader usage
    quadShader.Use();
}

bool QuadRenderer::endQuadBatch(){
    // calculate amount of quads to render
    GLsizeiptr size = (uint8_t*)quadBufferPtr - (uint8_t*)quadBuffer;
    if(size < 0){
        // no quads available
        return false;
    }

    // check opengl version
    if(GLAD_GL_VERSION_4_5){
        // set up dynamic buffer
        glNamedBufferSubData(quadVBO, 0, size, quadBuffer);
    }else{
        // set up dynamic buffer
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, quadBuffer);
    }

    // batch is fully set up
    return true;
}

void QuadRenderer::clear(){
    // delete all quad buffers
    delete[] quadBuffer;
    quadBufferPtr = nullptr;
    delete quadBufferPtr;

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