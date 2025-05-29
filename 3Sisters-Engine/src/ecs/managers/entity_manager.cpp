#include "ecs/types/component.hpp"
#include "ecs/types/signature.hpp"
#include <ecs/managers/entity_manager.hpp>

//? include standard library for debug prints
#include <iostream>

EntityManager::EntityManager(){
    // initialize the queue with all possible entity IDs
    for(Entity entity = 0; entity < MAX_ENTITIES; entity++){
        availableEntities.push(entity);
    }
}

Entity EntityManager::CreateEntity(){
    if(livingEntityCount > MAX_ENTITIES){
        //? display error
        std::cout << "ERROR: Too many entities in existance!\n"; 
        return 0;
    }

    // take an ID from the front of the queue
    Entity id = availableEntities.front();
    availableEntities.pop();
    livingEntityCount++;

    return id;
}

void EntityManager::DestroyEntity(Entity entity){
    if(entity > MAX_ENTITIES){
        //? display error
        std::cout << "ERROR: Entity out of range!\n"; 
        return;
    }

    // invalidate the destroyed entity's signature
    signatures[entity].reset();

    // put the destroyed ID at the back of the queue
    availableEntities.push(entity);
    livingEntityCount--;
}

void EntityManager::SetSignature(Entity entity, Signature signature){
    if(entity > MAX_ENTITIES){
        //? display error
        std::cout << "ERROR: Entity out of range!\n"; 
        return;
    }

    // put this entity's signature into the array
    signatures[entity] = signature;
}

Signature EntityManager::GetSignature(Entity entity){
    if(entity > MAX_ENTITIES){
        //? display error
        std::cout << "ERROR: Entity out of range!\n"; 
        return std::bitset<MAX_COMPONENTS>();
    }

    // get this entity's signature from the array
    return signatures[entity];
}