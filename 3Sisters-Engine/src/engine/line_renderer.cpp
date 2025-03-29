#include <engine/line_renderer.hpp>

// standard library for debug outputs
#include <iostream>

// include additional GLM library
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>


// initialize static variables
const glm::vec4                     LineRenderer::quadVertexPositions[4] = {
    {-0.5f, -0.5f, 0.0f, 1.0f},
    {0.5f, -0.5f, 0.0f, 1.0f},
    {0.5f, 0.5f, 0.0f, 1.0f},
    {-0.5f, 0.5f, 0.0f, 1.0f}};
// initialize line buffer
LineRenderer::LineVertex*             LineRenderer::lineBuffer = nullptr;
LineRenderer::LineVertex*             LineRenderer::lineBufferPtr = nullptr;
// initialize line graphics data
unsigned int                        LineRenderer::lineVAO;
unsigned int                        LineRenderer::lineVBO;
unsigned int                        LineRenderer::lineVertexCount;
unsigned int                        LineRenderer::lineWidth = 1.0f;
// initialize changeable shader
Shader                              LineRenderer::lineShader;
// initialize auto clear var
bool                                LineRenderer::isAutoClearSet = false;


void LineRenderer::Init(Shader& l){
    // when auto clear is set, stop re-initializing rendering data
    if(isAutoClearSet){
        std::cout << "Warning: Initialization of Sprite Renderer being called more than once!\n";
        return;
    }

    // set up automatic clear()
    setUpAutoClear();

    // set the line shader renference
    lineShader = l;

    // set up rendering of lines
    initLineRenderData();
}

void LineRenderer::DrawLine(glm::vec2 p0, glm::vec2 p1, glm::vec4 color){
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

void LineRenderer::DrawQuadWire(glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color){
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

void LineRenderer::StackLine(glm::vec2 p0, glm::vec2 p1, glm::vec4 color){
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

void LineRenderer::FlushLines(){
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

void LineRenderer::SetLineWidth(float width){
    lineWidth = width;
    glLineWidth(width);
}

float LineRenderer::GetLineWidth(){
    return lineWidth;
}

void LineRenderer::createLine(glm::vec2& p0, glm::vec2& p1, glm::vec4& color){
    if(lineVertexCount >= maxLineVertexCount){
        // flush what's left and start another batch
        FlushLines();
        beginLineBatch();
    }

    lineBufferPtr->position = p0;
    lineBufferPtr->color = color;
    lineBufferPtr++;

    lineBufferPtr->position = p1;
    lineBufferPtr->color = color;
    lineBufferPtr++;

    lineVertexCount += 2;
}

void LineRenderer::initLineRenderData(){
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
}

void LineRenderer::beginLineBatch(){
    // set buffer pointer
    lineBufferPtr = lineBuffer;

    // call shader usage
    lineShader.Use();
}

bool LineRenderer::endLineBatch(){
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

void LineRenderer::clear(){
    // delete all line buffers
    delete[] lineBuffer;
    lineBufferPtr = nullptr;
    delete lineBufferPtr;

    // delete line buffer data
    glDeleteVertexArrays(1, &lineVAO);
    glDeleteBuffers(1, &lineVBO);
}

void LineRenderer::setUpAutoClear(){
    // set up on exit to call the Clear()
    if(!isAutoClearSet && std::atexit(clear) == 0){
        isAutoClearSet = true; // disable calling this function again
    }
}