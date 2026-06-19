#include "../inc/test_window.hpp"

// include resource managers
#include <resourceSystems/managers/shader_manager.hpp>
#include <resourceSystems/managers/texture_manager.hpp>

// include camera
#include <cameras/ortho_camera.hpp>

// include renderers
#include <engine/quad_renderer.hpp>

// create static variables
static float rotation = 0.0f;

void TestWindow::init(){  
    // add quad shader
    Shader quadShader = ShaderManager::LoadShader("shaders/quad.vert", "shaders/quad.frag", nullptr, "quad");

    // load white texture
    TextureManager::GenerateWhiteTexture();

    // load logo texture
    TextureManager::LoadTexture("textures/sisters.png", "sisters");

    // create temp camera (only for calculating projection)
    OrthoCamera camera;
    camera.setDimensions(getWidth(), getHeight());
    camera.calculateProjectionView(quadShader);

    // setup renderers
    QuadRenderer::Init(quadShader);
}

void TestWindow::stepUpdate(double ts){
   // update rotation
   rotation += 0.5f;
}

void TestWindow::update(){

}

void TestWindow::render(double alpha){
    // render background
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    // render red quad
    QuadRenderer::StackQuad(
        TextureManager::GetTextureIndex("default"),
        glm::vec2(0.0f),
        glm::vec2(1.0f),
        rotation,
        glm::vec4(1.0f, 0.1f, 0.1f, 1.0f)
    );

    QuadRenderer::StackQuad(
        TextureManager::GetTextureIndex("sisters"),
        glm::vec2(0.0f),
        glm::vec2(0.7f),
        0.0f
    );

    // draw quads
    QuadRenderer::FlushQuads();
}
