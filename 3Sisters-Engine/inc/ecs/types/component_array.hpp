#pragma once

#ifndef COMPONENT_ARRAY_HPP
#define COMPONENT_ARRAY_HPP

#include <unordered_map>
#include <array>
#include <cstddef>
#include <iostream>
#include <stdexcept>

#include <ecs/types/entity.hpp>

/* an interface so that the Component Mananger can tell 
a generic ComponentArray that an Entity has been destroyed
and that it needs to update its array mappings
*/
class IComponentArray{
    public:
        virtual ~IComponentArray() = default;
        virtual void EntityDestroyed(Entity entity) = 0;
};

//TODO: remove the interface in favor of a event system that 
// allows EntityDestroyed to be called to all ComponentArrays

/* Component Array keeps track of entities that
contain a specified component.
*/
template<typename T>
class ComponentArray : public IComponentArray{
    private:
        /* packed array of components, set to a specific maximum amount
        that are mathcing the maximum number of entities allowed to exist
        simultaneously, so that each entity has a unique spot
        */
        std::array<T, MAX_ENTITIES> componentArray{};

        // map from an entity ID to an array index
        std::unordered_map<Entity, size_t> entityToIndexMap{};

        // map from an array index to an entity ID
        std::unordered_map<size_t, Entity> indexToEntityMap{};

        // total size of valid entries in the array
        size_t size{};

        // private storage of the debug option
        char debugOption;
        
    public:
        // public constructor
        ComponentArray(char option = 'd'){
            debugOption = option;
        }

        // give an entity a component
        void InsertData(Entity entity, T component){
            if(debugOption == 'd' && entityToIndexMap.find(entity) != entityToIndexMap.end()){
                std::cout << "ERROR: Entity already contains given component!\n";
                return;
            }

            // put a new entity at end and update the maps
            size_t newIndex = size;
            entityToIndexMap[entity] = newIndex;
            indexToEntityMap[newIndex] = entity;
            componentArray[newIndex] = component;
            ++size;
        }

        // remove an entity's component
        void RemoveData(Entity entity){
            if(debugOption == 'd' && entityToIndexMap.find(entity) == entityToIndexMap.end()){
                std::cout << "ERROR: Entity doesn't have such component to remove!\n";
                return;
            }

            // copy element at end into deleted element's place to maintain density
            size_t indexOfRemovedEntity = entityToIndexMap[entity];
            size_t indexOfLastElement = size - 1;
            componentArray[indexOfRemovedEntity] = componentArray[indexOfLastElement];

            // update the map to point to moved spot
            Entity entityOfLastElement = indexToEntityMap[indexOfLastElement];
            entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
            indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

            entityToIndexMap.erase(entity);
            indexToEntityMap.erase(indexOfLastElement);

            --size;
        }

        // return a reference of the entity's component
        T& GetData(Entity entity){
            if(debugOption == 'd' && entityToIndexMap.find(entity) == entityToIndexMap.end()){
                throw std::invalid_argument("ERROR: Can't find entity with a component!");
            }

            return componentArray[entityToIndexMap[entity]];
        }

        // check if entity contains given component
        bool CheckData(Entity entity){
            // return true if entity has been found with component, otherwise false
            return !(entityToIndexMap.find(entity) == entityToIndexMap.end());
        }

        // remove an entity's component when destroyed
        void EntityDestroyed(Entity entity) override{
            if (entityToIndexMap.find(entity) != entityToIndexMap.end()){
			    RemoveData(entity);
		    }
        }

};

#endif