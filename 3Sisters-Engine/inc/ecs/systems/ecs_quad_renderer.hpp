#pragma once

#ifndef ECS_QUAD_RENDERER_HPP
#define ECS_QUAD_RENDERER_HPP

// include ECS
#include <ecs/ecs.hpp>

// include Engine Renderer
#include <engine/quad_renderer.hpp>

/* ECS_QuadRenderer class is a ECS system that
 allows entities with the necessary components to
 be rendered using the QuadRenderer static class
*/
class ECS_QuadRenderer : public System{
    public:
        // render all entities added
        void render(double alpha);
};

// create an alias to easily refer to this system
using ECS_QuadRendererPtr = std::shared_ptr<ECS_QuadRenderer>;

#endif
