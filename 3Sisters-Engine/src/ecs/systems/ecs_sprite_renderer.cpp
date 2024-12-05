#include <ecs/systems/ecs_sprite_renderer.hpp>

// include standard components
#include <ecs/default_components.hpp>

void ECS_SpriteRenderer::render(double alpha){
    // loop through all entities
    for(auto const& entity : entities){
        // grab entity components
        auto& transform = ECS::GetComponent<Transform2D>(entity);
        auto& material = ECS::GetComponent<Material2D>(entity);

        // check if entity has interpolation
        if((ECS::GetComponentType<Interpolation>() != 255) && ECS::CheckComponent<Interpolation>(entity)){
            // grab entity component
            auto& inter = ECS::GetComponent<Interpolation>(entity);

            // add to stack
            SpriteRenderer::StackQuad(material.texIndex, inter, transform.size, transform.rotation, alpha, material.color, material.texCoords);

            // skip iteration
            continue;
        }

        // add to stack
        SpriteRenderer::StackQuad(material.texIndex, transform.position, transform.size, transform.rotation, material.color, material.texCoords);
    }

    // flush
    SpriteRenderer::FlushQuads();
}
