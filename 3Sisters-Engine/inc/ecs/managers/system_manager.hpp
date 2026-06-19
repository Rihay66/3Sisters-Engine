#pragma once

#ifndef SYSTEM_MANAGER_HPP
#define SYSTEM_MANAGER_HPP

#include <memory>
#include <unordered_map>
#include <type_traits>
#include <iostream>

#include <ecs/types/system.hpp>
#include <ecs/types/signature.hpp>

/* System Manager manages record of registered
systems and their signatures. Each system needs
a signature in order for the manager to add the
appropriate entities.
*/
class SystemManager{
    private:
        // map from system type string pointer to a signature
        std::unordered_map<const char*, Signature> signatures{};

        // map from system type string pointer to a system pointer
        std::unordered_map<const char*, std::shared_ptr<System>> systems{};

        // private storage of the debug option
        char debugOption;

    public:
        // public constructor
        SystemManager(char option = 'd'){
            debugOption = option;
        }

        // register a system for use, returns a reference of the system for external use
        template<typename T>
        std::shared_ptr<T> RegisterSystem(){
            static_assert(std::is_base_of<System, T>::value, "ERROR: given type to register system does not inherit from System");

            const char* typeName = typeid(T).name();

            if(systems.find(typeName) != systems.end()){
                std::cout << "ERROR: " << typeName << " is already registered, cannot have duplicate registrations\n";
                return nullptr;
            }

            // create a pointer to the system and return it so it can be used externally
            auto system = std::make_shared<T>();
            systems.insert({typeName, system});
            return system;
        }

        // get registered system and return reference of such system for external use
        template<typename T>
        std::shared_ptr<T> GetSystem(){
            static_assert(std::is_base_of<System, T>::value, "ERROR: given type to register system does not inherit from System");

            const char* typeName = typeid(T).name();

            if(systems.find(typeName) == systems.end()){
                std::cout << "ERROR: " << typeName << " is not registered, given system type can't be found\n";
                // no registered system was found, return nothing
                return nullptr;
            }
            
            return std::dynamic_pointer_cast<T>(systems[typeName]);
        }

        // set the signature to specified system
        template<typename T>
        void SetSignature(Signature signature){
            const char* typeName = typeid(T).name();

            if(debugOption == 'd' && systems.find(typeName) == systems.end()){
                std::cout << "ERROR: System: " << typeName << " already contains a signature\n";
                return;
            }

            // set the signature for this system
            signatures.insert({typeName, signature});
        }

        // remove destroyed entity from all systems
        void EntityDestroyed(Entity entity){
            for(auto const& pair : systems){
                pair.second->entities.erase(entity);
            }
        }

        // notify each system that a entity's signature has changed
        void EntitySignatureChange(Entity entity, Signature entitySignature){
            for(const auto& [key, value] : systems){
                auto const& type = key;
                auto const& system = value;
                auto const& systemSignature = signatures[type];

                // check if the entity signature change needs to be added or removed
                if((entitySignature & systemSignature) == systemSignature){
                    // insert entity into system
                    system->entities.insert(entity);
                }else{
                    // remove entity from system
                    system->entities.erase(entity);
                }
            }
        }
};

#endif
