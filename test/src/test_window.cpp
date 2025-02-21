#include "../inc/test_window.hpp"
#include "ecs/systems/ecs_sprite_renderer.hpp"
#include <engine/components/interpolation.hpp>
#include <resourceSystems/managers/resource_manager.hpp>
#include <engine/sprite_renderer.hpp>
#include <engine/text_renderer.hpp>
#include <ecs/ecs.hpp>
#include <ecs/components/material.hpp>
#include <ecs/components/transform.hpp>

TestWindow::TestWindow() : Window(){

}

TestWindow::~TestWindow() {
    
}

void TestWindow::init(){  
    // load a quad shader
    ResourceManager::LoadShader("shaders/quad_es.vs", "shaders/quad_es.frag", nullptr, "quad");
    // load a line shader
    ResourceManager::LoadShader("shaders/line_es.vs", "shaders/line_es.frag", nullptr, "line");
    // load a text shader
    ResourceManager::LoadShader("shaders/text_es.vs", "shaders/text_es.frag", nullptr, "text");
    
    // generate white texture
    ResourceManager::GenerateWhiteTexture();
    // load a texture
    ResourceManager::LoadTexture("textures/sisters.png", "sisters");
    // load a font
    ResourceManager::LoadFontTexture("fonts/November.ttf", 36, "font");
    
    // set up camera
    camera.setDimensions(getWidth(), getHeight());
    camera.calculateProjectionView(ResourceManager::GetShader("quad"));
    camera.calculateProjectionView(ResourceManager::GetShader("line"));
    camera.calculateProjectionView(ResourceManager::GetShader("text"));

    // set up renderers
    SpriteRenderer::Init(ResourceManager::GetShader("quad"), ResourceManager::GetShader("line"), glm::vec2(15.0f), glm::vec2(15.0f));
    TextRenderer::Init(ResourceManager::GetShader("text"));

    // init ECS
    ECS::Init();

    // register components
    ECS::RegisterComponent<Transform2D>();
    ECS::RegisterComponent<Material2D>();
    ECS::RegisterComponent<Interpolation>();

    // register systems
    ECS::RegisterSystem<ECS_SpriteRenderer>();

    // get reference of the system
    renderer = ECS::GetSystem<ECS_SpriteRenderer>();

    // set signatures to system
    ECS::SetSystemSignature<ECS_SpriteRenderer>(
        ECS::GetComponentType<Transform2D>(),
        ECS::GetComponentType<Material2D>()
    );

    // create entity
    entity = ECS::CreateEntity();
    // add components to entity
    ECS::AddComponent(entity, 
    Transform2D{
        .position = {20.0f, 20.0f},
        .rotation = 0.0f,
        .size = {1.5f, 1.5f}
    },
    Material2D{
        .texIndex = ResourceManager::GetTextureIndex("default"),
        .color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
    }    
    );
}

void TestWindow::stepUpdate(double ts){
    // make entity spin
    auto& pos = ECS::GetComponent<Transform2D>(entity);
    pos.rotation += 2.0f;
}

void TestWindow::update(){

}

void TestWindow::render(double alpha){
    // render background
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    // test ECS quad renderer system
    renderer->render(alpha); 

    // test quad renderer
    SpriteRenderer::StackQuad(ResourceManager::GetTextureIndex("default"), glm::vec2(15.0f), glm::vec2(2.0f), 0.0f, glm::vec4(0.0f, 0.5f, 0.5f, 1.0f));
    SpriteRenderer::StackQuad(ResourceManager::GetTextureIndex("sisters"), glm::vec2(5.0f), glm::vec2(5.0f), 45.0f);

    SpriteRenderer::FlushQuads();

    SpriteRenderer::DrawQuad(ResourceManager::GetTextureIndex("default"), glm::vec2(40.0f, 20.0f), glm::vec2(10.0f), 0.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

    // test line renderer
    SpriteRenderer::DrawLine(glm::vec2(1.0f), glm::vec2(10.0f));

    // render text
    TextRenderer::DrawText(ResourceManager::GetFontTexture("font"), "HELLO world!", glm::vec2(100.0f, 200.0f), glm::vec2(1.0f));
}
