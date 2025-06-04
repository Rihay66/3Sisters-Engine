#pragma once

#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include <queue>
#include <array>

#include <ecs/types/entity.hpp>
#include <ecs/types/signature.hpp>

/* Entity Manager distribtutes entity IDs and
keeps record of which entities are in use and
which are not.
*/
class EntityManager{
    private:
        // queue of unused entity IDs
        std::queue<Entity> availableEntities{};

        // array of signatures where the index corresponds to the entity ID
        std::array<Signature, MAX_ENTITIES> signatures{};

        // total living entities, used to keep limits to how many exist
        uint32_t livingEntityCount{};

    public:
        // constructor
        EntityManager();

        // create an entity
        Entity CreateEntity();

        // destroy an entity
        void DestroyEntity(Entity entity);

        // set an existing entity their signature
        void SetSignature(Entity entity, Signature signature);

        // get an exiting entity's signature
        Signature GetSignature(Entity entity);
};

#endif