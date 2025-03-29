#include <ecs/systems/ecs_quad_renderer.hpp>

// include standard components
#include <ecs/default_components.hpp>

void ECS_QuadRenderer::render(double alpha){
    // loop through all entities
    for(auto const& entity : entities){
        // grab entity components
        auto& transform = ECS::GetComponent<Transform2D>(entity);
        auto& material = ECS::GetComponent<Material2D>(entity);

        // add to stack
        QuadRenderer::StackQuad(material.texIndex, transform.position, transform.size, transform.rotation, material.color, material.texCoords);
    }

    // flush
    QuadRenderer::FlushQuads();
}
