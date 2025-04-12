#include "../inc/test_window.hpp"
#include "engine/text_renderer.hpp"
#include "input/sisters_glfw_keyboard.hpp"
#include "input/sisters_glfw_gamepad.hpp"
#include <resourceSystems/managers/texture_manager.hpp>
#include <resourceSystems/managers/shader_manager.hpp>
#include <engine/quad_renderer.hpp>

#include <iostream>

TestWindow::TestWindow() : Window(){

}

TestWindow::~TestWindow() {
    
}

void TestWindow::init(){  
    // load a quad shader
    ShaderManager::LoadShader("shaders/quad.vert", "shaders/quad.frag", nullptr, "quad");
    // load a line shader
    ShaderManager::LoadShader("shaders/line.vert", "shaders/line.frag", nullptr, "line");
    // load a text shader
    ShaderManager::LoadShader("shaders/text.vert", "shaders/text.frag", nullptr, "text");
    
    // generate white texture
    TextureManager::GenerateWhiteTexture();
    // load a texture
    TextureManager::LoadTexture("textures/sisters.png", "sisters");
    // load a font
    TextureManager::LoadFontTexture("fonts/November.ttf", "font", 512,  512, 64.0f, false);
    
    // set up camera
    camera.setDimensions(getWidth(), getHeight());
    camera.calculateProjectionView(ShaderManager::GetShader("quad"));
    camera.calculateProjectionView(ShaderManager::GetShader("line"));
    camera.calculateProjectionView(ShaderManager::GetShader("text"));

    // set up renderers
    QuadRenderer::Init(ShaderManager::GetShader("quad"));
    TextRenderer::Init(ShaderManager::GetShader("text"), getHeight());
    
    // set gamepad
    setGamepad(pad);
}

void TestWindow::stepUpdate(double ts){
    //* Update appropriately the camera projection
    camera.setDimensions(getWidth(), getHeight());
    
    //* test input
    if(GLFW::getKeyInput(SISTER_KEY_E)){
        std::cout << "E was pressed!\n";
    }
    
    if(GLFW::getButtonInput(pad, SISTER_BUTTON_SOUTH)){
        std::cout << "South button pressed!\n";
    }
}

void TestWindow::update(){

}

void TestWindow::render(double alpha){
    // recalculate camera projection
    camera.calculateProjectionView(ShaderManager::GetShader("quad"));
    camera.calculateProjectionView(ShaderManager::GetShader("line"));
    camera.calculateProjectionView(ShaderManager::GetShader("text"));

    // render background
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    // test quad renderer
    QuadRenderer::StackQuad(TextureManager::GetTextureIndex("default"), glm::vec2(0.0f), glm::vec2(0.5f), 0.0f, glm::vec4(0.0f, 0.5f, 0.5f, 1.0f));
    QuadRenderer::StackQuad(TextureManager::GetTextureIndex("sisters"), glm::vec2(-1.2f, 0.0f), glm::vec2(0.75f), 45.0f);

    QuadRenderer::FlushQuads();
    
    // test text renderer
    TextRenderer::StackCharacters(TextureManager::GetFontTexture("font"), "Hello World", glm::vec3(0.0f, 0.5f, 0.0f), 45.0f, 1.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    TextRenderer::StackCharacters(TextureManager::GetFontTexture("font"), "Hi\nBye", glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 1.0f);
    TextRenderer::FlushText();
}
