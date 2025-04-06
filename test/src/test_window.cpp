#include "../inc/test_window.hpp"
#include "ecs/systems/ecs_quad_renderer.hpp"
#include "engine/text_renderer.hpp"
#include <engine/components/interpolation.hpp>
#include <resourceSystems/managers/texture_manager.hpp>
#include <resourceSystems/managers/shader_manager.hpp>
#include <engine/quad_renderer.hpp>
#include <ecs/ecs.hpp>
#include <ecs/components/material.hpp>
#include <ecs/components/transform.hpp>

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
    
    // initialize gamepad
    GamepadManager::InitializeQuery(getEventState());

    // init ECS
    ECS::Init();

    // register components
    ECS::RegisterComponent<Transform2D>();
    ECS::RegisterComponent<Material2D>();
    ECS::RegisterComponent<Gamepad>();

    // register systems
    ECS::RegisterSystem<ECS_QuadRenderer>();

    // get reference of the system
    renderer = ECS::GetSystem<ECS_QuadRenderer>();

    // set signatures to system
    ECS::SetSystemSignature<ECS_QuadRenderer>(
        ECS::GetComponentType<Transform2D>(),
        ECS::GetComponentType<Material2D>()
    );

    // create entity
    greenEntity = ECS::CreateEntity();
    // add components to entity
    ECS::AddComponent(greenEntity, 
    Transform2D{
        .position = {20.0f, 20.0f},
        .rotation = 0.0f,
        .size = {1.5f, 1.5f}
    },
    Material2D{
        .texIndex = TextureManager::GetTextureIndex("default"),
        .color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
    },
    Gamepad{

    }
    );

    // create another entity
    redEntity = ECS::CreateEntity();
    // add components to entity
    ECS::AddComponent(redEntity, 
    Transform2D{
        .position = {10.0f, 20.0f},
        .rotation = 0.0f,
        .size = {1.5f, 1.5f}
    },
    Material2D{
        .texIndex = TextureManager::GetTextureIndex("default"),
        .color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
    },
    Gamepad{

    }
    );

    // put gamepad into gamepad manager
    GamepadManager::SetGamepad(ECS::GetComponent<Gamepad>(greenEntity), 0);
    GamepadManager::SetGamepad(ECS::GetComponent<Gamepad>(redEntity), 1);
}

void TestWindow::stepUpdate(double ts){
    // make entity spin
    auto& redpos = ECS::GetComponent<Transform2D>(redEntity);
    redpos.rotation += 2.0f;

    // make entity move according to the controller input
    auto& redgamepad = ECS::GetComponent<Gamepad>(redEntity);

    // check for input
    if(getButtonInput(redgamepad, PLAYSTATION_BUTTON_CROSS)){
        // move entity upwards
        redpos.position.y += 1.0f * ts;
    }

    // make entity spin
    auto& pos = ECS::GetComponent<Transform2D>(greenEntity);
    pos.rotation += 2.0f;

    // make entity move according to the controller input
    auto& gamepad = ECS::GetComponent<Gamepad>(greenEntity);

    // check for input
    if(getButtonInput(gamepad, PLAYSTATION_BUTTON_CROSS)){
        // move entity upwards
        pos.position.y += 1.0f * ts;
    }

    //* Update appropriately the camera projection
    camera.setDimensions(getWidth(), getHeight());
}

void TestWindow::update(){
    // poll gamepad inputs
    GamepadManager::PollIO();
}

void TestWindow::render(double alpha){
    // recalculate camera projection
    camera.calculateProjectionView(ShaderManager::GetShader("quad"));
    camera.calculateProjectionView(ShaderManager::GetShader("line"));
    camera.calculateProjectionView(ShaderManager::GetShader("text"));

    // render background
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    // test ECS quad renderer system
    //renderer->render(alpha); 

    // test quad renderer
    QuadRenderer::StackQuad(TextureManager::GetTextureIndex("default"), glm::vec2(0.0f), glm::vec2(0.5f), 0.0f, glm::vec4(0.0f, 0.5f, 0.5f, 1.0f));
    QuadRenderer::StackQuad(TextureManager::GetTextureIndex("sisters"), glm::vec2(-1.2f, 0.0f), glm::vec2(0.75f), 45.0f);

    QuadRenderer::FlushQuads();
    
    // test text renderer
    TextRenderer::StackCharacters(TextureManager::GetFontTexture("font"), "Hello World", glm::vec3(0.0f, 0.5f, 0.0f), 45.0f, 1.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    TextRenderer::StackCharacters(TextureManager::GetFontTexture("font"), "Hi\nBye", glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 1.0f);
    TextRenderer::FlushText();
}
