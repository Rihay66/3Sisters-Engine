#include <engine/sprite_renderer.hpp>

// standard library for debug outputs
#include <iostream>

// include additional GLM library
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

// initialize static variables
const glm::vec4                     SpriteRenderer::quadVertexPositions[4] = {
    {-0.5f, -0.5f, 0.0f, 1.0f},
    {0.5f, -0.5f, 0.0f, 1.0f},
    {0.5f, 0.5f, 0.0f, 1.0f},
    {-0.5f, 0.5f, 0.0f, 1.0f}};
const std::array<glm::vec2, 4>      SpriteRenderer::textureCoordinates = {{
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f}
    }};;
// initialize quad buffer
SpriteRenderer::QuadVertex*             SpriteRenderer::quadBuffer = nullptr;
SpriteRenderer::QuadVertex*             SpriteRenderer::quadBufferPtr = nullptr;
// initialize line buffer
SpriteRenderer::LineVertex*             SpriteRenderer::lineBuffer = nullptr;
SpriteRenderer::LineVertex*             SpriteRenderer::lineBufferPtr = nullptr;
// initialize quad graphics data
unsigned int                        SpriteRenderer::quadVAO;
unsigned int                        SpriteRenderer::quadVBO;
unsigned int                        SpriteRenderer::quadEBO;
unsigned int                        SpriteRenderer::quadIndexCount;
// initialize line graphics data
unsigned int                        SpriteRenderer::lineVAO;
unsigned int                        SpriteRenderer::lineVBO;
unsigned int                        SpriteRenderer::lineVertexCount;
unsigned int                        SpriteRenderer::lineWidth = 1.0f;
// initialize changeable shader
Shader                              SpriteRenderer::quadShader;
Shader                              SpriteRenderer::lineShader;
// initialize "Universal" pixel size for all render primatives under this renderer
glm::uvec2                          SpriteRenderer::quadPixelSize;
glm::uvec2                          SpriteRenderer::linePixelSize;
// initialize auto clear var
bool                                SpriteRenderer::QuadSet = false;
bool                                SpriteRenderer::LineSet = false;
bool                                SpriteRenderer::isAutoClearSet = false;

void SpriteRenderer::InitQuad(Shader& s, glm::uvec2 p){
    // when auto clear is set, stop re-initializing rendering data
    if(QuadSet){
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

    // set up sprite model view
    quadPixelSize = p;

    // set up rendering of quads
    initQuadRenderData(); 
}

void SpriteRenderer::Init(Shader& qS, Shader& lS, glm::uvec2 qP, glm::uvec2 lP){
    // when auto clear is set, stop re-initializing rendering data
    if(QuadSet && LineSet){
        std::cout << "Warning: Initialization of Sprite Renderer being called more than once!\n";
        return;
    }

    // set up automatic clear()
    setUpAutoClear();

    // set the quad shader reference
    quadShader = qS;

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

    // set the line shader renference
    lineShader = lS;

    // set up model pixel size
    quadPixelSize = qP;
    linePixelSize = lP;

    // set up rendering of quads
    initQuadRenderData(); 

    // set up rendering of lines
    initLineRenderData();
}

void SpriteRenderer::InitLine(Shader& l, glm::uvec2 lP){
    // when auto clear is set, stop re-initializing rendering data
    if(LineSet){
        std::cout << "Warning: Initialization of Sprite Renderer being called more than once!\n";
        return;
    }

    // set up automatic clear()
    setUpAutoClear();

    // set the line shader renference
    lineShader = l;

    // set up model pixel size
    linePixelSize = lP;

    // set up rendering of lines
    initLineRenderData();
}

void SpriteRenderer::DrawQuad(int texIndex, glm::vec2 pos, glm::vec2 size, float rot, glm::vec4 color,const std::array<glm::vec2, 4> texCoords ,const glm::vec4 vertexPositions[]){
    //? check if buffer hasn't been set up
    if(quadBuffer == nullptr){
        //! Display error
        std::cout << "ERROR: Missing render quad buffer initialization!\n";
        return; // stop function
    }

    // init the buffer
    beginQuadBatch();

    // create the quad to render
    createQuad(pos, size, rot, texIndex, color, texCoords, vertexPositions);

    // render
    FlushQuads();
}

void SpriteRenderer::DrawLine(glm::vec2 p0, glm::vec2 p1, glm::vec4 color){
    //? check if buffer hasn't been set up
    if(lineBuffer == nullptr){
        //! Display error
        std::cout << "ERROR: Missing render line buffer initialization!\n";
        return; // stop function
    }

    // init the buffer
    beginLineBatch();

    // create the line to render
    createLine(p0, p1, color);

    // render
    FlushLines();
}

void SpriteRenderer::DrawQuadWire(glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color){
    //? check if buffer hasn't been set up
    if(lineBuffer == nullptr){
        //! Display error
        std::cout << "ERROR: Missing render line buffer initialization!\n";
        return; // stop function
    }

    // init the buffer
    beginLineBatch();

    // create the lines that make up a rectangle to render
    
    // create array of vertices
    glm::vec2 lineVertices[4];

    // create the transform
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) 
    * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) 
    * glm::scale(glm::mat4(1.0f), {size.x, size.y, 0.0f});

    // set the vertices
    for(int i = 0; i < 4; i++){
        lineVertices[i] = transform * quadVertexPositions[i];
    }
    
    //draw the lines to make up the rectangle

    createLine(lineVertices[0], lineVertices[1], color);
    createLine(lineVertices[1], lineVertices[2], color);
    createLine(lineVertices[2], lineVertices[3], color);
    createLine(lineVertices[3], lineVertices[0], color);

    // render
    FlushLines();    
}

void SpriteRenderer::StackQuad(int texIndex, glm::vec2 pos, glm::vec2 size, float rot, glm::vec4 color, const std::array<glm::vec2, 4> texCoords, const glm::vec4 vertexPositions[]){
    //? check if buffer hasn't been set up
    if(quadBuffer == nullptr){
        //! Display error
        std::cout << "ERROR: Missing render buffer initialization!\n";
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

void SpriteRenderer::StackLine(glm::vec2 p0, glm::vec2 p1, glm::vec4 color){
    //? check if buffer hasn't been set up
    if(lineBuffer == nullptr){
        //! Display error
        std::cout << "ERROR: Missing render line buffer initialization!\n";
        return; // stop function
    }

    // check if the buffer pointer hasn't been set up
    if(lineBufferPtr == nullptr){
        // reset render stats
    
        // the initialize the batch
        beginLineBatch();
    }

    // if not then add a line to the buffer pointer

    // create the line to render
    createLine(p0, p1, color);
}

void SpriteRenderer::FlushQuads(){
    //? check if buffer hasn't been set up
    if(quadBuffer == nullptr){
        //! Display error
        std::cout << "ERROR: Missing render buffer initialization!\n";
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

void SpriteRenderer::FlushLines(){
    //? check if buffer hasn't been set up
    if(lineBuffer == nullptr){
        //! Display error
        std::cout << "ERROR: Missing render line buffer initialization!\n";
        return; // stop function
    }

    // set up vertex dynamic buffer
    if(!endLineBatch()){
        // there are no lines to render
        //! Display Warning
        //TODO: Make a debug option to show this warning
        //std::cout << "WARNING: No line added to draw!\n";
        return; // stop function
    }

    // draw the line\s
    glBindVertexArray(lineVAO);
    glDrawArrays(GL_LINES, 0, lineVertexCount);

    // reset buffer pointer
    lineBufferPtr = nullptr;

    // reset vertex count
    lineVertexCount = 0;
}

void SpriteRenderer::SetLineWidth(float width){
    lineWidth = width;
    glLineWidth(width);
}

void SpriteRenderer::SetQuadPixelSize(glm::uvec2 pixelSize){
    quadPixelSize = pixelSize;
}

void SpriteRenderer::SetLinesPixelSize(glm::uvec2 pixelSize){
    linePixelSize = pixelSize;
}

float SpriteRenderer::GetLineWidth(){
    return lineWidth;
}

void SpriteRenderer::createQuad(glm::vec2& pos, glm::vec2& size, float& rotation, int& texIndex, glm::vec4& color, const std::array<glm::vec2, 4> texCoords,const glm::vec4 vertexPositions[]){
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

    quadBufferPtr->position =
        (transform * vertexPositions[0]) *
        glm::scale(glm::mat4(1.0f),
                   glm::vec3(quadPixelSize.x, quadPixelSize.y, 1.0f));
    quadBufferPtr->texCoords = texCoords[0];
    quadBufferPtr->texIndex = texIndex;
    quadBufferPtr->color = color;
    quadBufferPtr++;

    quadBufferPtr->position =
        (transform * vertexPositions[1]) *
        glm::scale(glm::mat4(1.0f),
                   glm::vec3(quadPixelSize.x, quadPixelSize.y, 1.0f));
    quadBufferPtr->texCoords = texCoords[1];
    quadBufferPtr->texIndex = texIndex;
    quadBufferPtr->color = color;
    quadBufferPtr++;

    quadBufferPtr->position =
        (transform * vertexPositions[2]) *
        glm::scale(glm::mat4(1.0f),
                   glm::vec3(quadPixelSize.x, quadPixelSize.y, 1.0f));
    quadBufferPtr->texCoords = texCoords[2];
    quadBufferPtr->texIndex = texIndex;
    quadBufferPtr->color = color;
    quadBufferPtr++;

    quadBufferPtr->position =
        (transform * vertexPositions[3]) *
        glm::scale(glm::mat4(1.0f),
                   glm::vec3(quadPixelSize.x, quadPixelSize.y, 1.0f));
    quadBufferPtr->texCoords = texCoords[3];
    quadBufferPtr->texIndex = texIndex;
    quadBufferPtr->color = color;
    quadBufferPtr++;

    quadIndexCount += 6;
}

void SpriteRenderer::createLine(glm::vec2& p0, glm::vec2& p1, glm::vec4& color){
    if(lineVertexCount >= maxLineVertexCount){
        // flush what's left and start another batch
        FlushLines();
        beginLineBatch();
    }

    lineBufferPtr->position = p0 * glm::vec2(linePixelSize.x, linePixelSize.y);
    lineBufferPtr->color = color;
    lineBufferPtr++;

    lineBufferPtr->position = p1 * glm::vec2(linePixelSize.x, linePixelSize.y);
    lineBufferPtr->color = color;
    lineBufferPtr++;

    lineVertexCount += 2;
}

// Set up the quad rendering
void SpriteRenderer::initQuadRenderData(){
    // check if quad buffer had already been initialized
    if (quadBuffer != nullptr)
        exit(-1); // avoid re-initalize the render data

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

    // set quad set flag
    QuadSet = true;
}

void SpriteRenderer::initLineRenderData(){
    // check if line buffer had already been initialized
    if(lineBuffer != nullptr)
        exit(-1); // avoid re-initalize the render data

    // configure the buffer
    lineBuffer = new LineVertex[maxLineVertexCount];

    // check opengl version
    if(GLAD_GL_VERSION_4_5){
        // configure VAO/VBO
        glCreateVertexArrays(1, &lineVAO);
        glCreateBuffers(1, &lineVBO);
        
        glNamedBufferData(lineVBO, sizeof(LineVertex) * maxLineVertexCount, nullptr, GL_DYNAMIC_DRAW);
        
        glVertexArrayVertexBuffer(lineVAO, 0, lineVBO, 0, sizeof(LineVertex));
        
        // vertex attribute
        glEnableVertexArrayAttrib(lineVAO, 0);
        glVertexArrayAttribBinding(lineVAO, 0, 0);
        glVertexArrayAttribFormat(lineVAO, 0, 2, GL_FLOAT, GL_FALSE, offsetof(LineVertex, position));
        
        // color attribute
        glEnableVertexArrayAttrib(lineVAO, 1);
        glVertexArrayAttribBinding(lineVAO, 1, 0);
        glVertexArrayAttribFormat(lineVAO, 1, 4, GL_FLOAT, GL_FALSE, offsetof(LineVertex, color));
    }else{
        // configure VAO/VBO
        glGenVertexArrays(1, &lineVAO);
        glGenBuffers(1, &lineVBO);

        glBindVertexArray(lineVAO);

        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(LineVertex) * maxLineVertexCount, nullptr, GL_DYNAMIC_DRAW);

        // vertex attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (const void*)offsetof(LineVertex, position));

        // color attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (const void*)offsetof(LineVertex, color));
    }

    // set line set flag
    LineSet = true;
}

void SpriteRenderer::beginQuadBatch(){
    // set buffer pointer
    quadBufferPtr = quadBuffer;

    // call shader usage
    quadShader.Use();
}

void SpriteRenderer::beginLineBatch(){
    // set buffer pointer
    lineBufferPtr = lineBuffer;

    // call shader usage
    lineShader.Use();
}

bool SpriteRenderer::endQuadBatch(){
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

bool SpriteRenderer::endLineBatch(){
    // calculate amount of lines to render
    GLsizeiptr size = (uint8_t*)lineBufferPtr - (uint8_t*)lineBuffer;
    if(size < 0){
        // no quads available
        return false;
    }

    // check opengl version
    if(GLAD_GL_VERSION_4_5){
        // set up dynamic buffer
        glNamedBufferSubData(lineVBO, 0, size, lineBuffer);
    }else{
        // set up dynamic buffer
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, lineBuffer);
    }
    return true;
}

void SpriteRenderer::clear(){
    // delete all quad buffers
    delete[] quadBuffer;
    quadBufferPtr = nullptr;
    delete quadBufferPtr;

    // delete all line buffers
    delete[] lineBuffer;
    lineBufferPtr = nullptr;
    delete lineBufferPtr;

    // delete quad buffer data
    if(QuadSet){
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
        glDeleteBuffers(1, &quadEBO);
    }
    // delete line buffer data
    if(LineSet){
        glDeleteVertexArrays(1, &lineVAO);
        glDeleteBuffers(1, &lineVBO);
    }
}

void SpriteRenderer::setUpAutoClear(){
    // set up on exit to call the Clear()
    if(!isAutoClearSet && std::atexit(clear) == 0){
        isAutoClearSet = true; // disable calling this function again
    }
}