#pragma once

#ifndef ECS_HPP
#define ECS_HPP

#include <type_traits>

// include all ecs managers
#include <ecs/managers/entity_manager.hpp>
#include <ecs/managers/component_manager.hpp>
#include <ecs/managers/system_manager.hpp>

/* A static singleton ECS (aka coordinator) class that hosts
 several function to create, manage, remove Entities, Components,
 and Systems. All functions and resources are static and no public 
 constructor is defined.  
*/
class ECS{
    public:
        /* initialize the entity, component, and system managers
            this function also contains debug options
            @ d - debug, debug outputs, error checking (default).
            @ r - release, skip most functional error checking and some debug outputs (use with caution).
        */
        static void Init(char debugOption = 'd');

        //* Entity Functions

        // create an Entity, returns reference of created Entity
        static Entity CreateEntity();

        // destroy given Entity and remove attached components
        static void DestroyEntity(Entity entity);

        // get an existing entity's component signature
        static Signature GetEntitySignature(Entity entity){
            return entityManager->GetSignature(entity);
        }
        
        //* Component Functions
        
        // register specified component for entity and system usage
        template<typename T>
        static void RegisterComponent(){
            componentManager->RegisterComponent<T>();
        }     

        // add a specified component to an existing entity 
        template<typename T>
        static void AddComponent(Entity entity, T component){
            //? check if component is not registered
            if(componentManager->GetComponentType<T>() == 255){
                std::cout << "ERROR: Failed to add component to entity: " <<  entity << " | component: " << typeid(T).name() << " isn't registered to ECS!" << "\n";
                return;
            }
            // else add component to existing entity

            componentManager->AddComponent<T>(entity,component);

            Signature signature = entityManager->GetSignature(entity); 
            signature.set(componentManager->GetComponentType<T>(), true);
            entityManager->SetSignature(entity, signature);

            systemManager->EntitySignatureChange(entity, signature);
        }

        // add a list of components to an existing entity
        template<typename... Args>
        static void AddComponent(Entity entity, Args... args){
            // check the amount of arguments to the max components
            static_assert(sizeof...(args) < MAX_COMPONENTS, "ERROR: Too many specified component, the max is 32");

            //? check if every component is not registered
            if(((componentManager->GetComponentType<Args>() == 255) ||  ...)){
                //TODO: Find a way to print the exact argument's name that isn't registered (for now this should still be helpful)
                std::cout << "ERROR: Failed to add component to entity: " <<  entity << " | a component isn't registered to ECS!" << "\n";
                return;
            }

            // else add each component to the exisiting entity

            (componentManager->AddComponent<Args>(entity, args), ...);

            Signature signature = entityManager->GetSignature(entity);
            (signature.set(componentManager->GetComponentType<Args>(), true), ...);
            entityManager->SetSignature(entity, signature);

            systemManager->EntitySignatureChange(entity, signature);
        }

        // remove a component from an existing entity
        template<typename T>
        static void RemoveComponent(Entity entity){
            //? check if component is not registered
            if(componentManager->GetComponentType<T>() == 255){
                std::cout << "ERROR: Failed to remove component to entity: " <<  entity << " | component: " << typeid(T).name() << " isn't registered to ECS!" << "\n";
                return;
            }

            componentManager->RemoveComponent<T>(entity);

            Signature signature = entityManager->GetSignature(entity);
            signature.set(componentManager->GetComponentType<T>(), false);
            entityManager->SetSignature(entity, signature);

            systemManager->EntitySignatureChange(entity, signature);
        }

        // get reference of an exiting entity's component
        template<typename T>
        static T& GetComponent(Entity entity){
            return componentManager->GetComponent<T>(entity);
        }

        // check if entity has such component
        template<typename T>
        static bool CheckComponent(Entity entity){
            return componentManager->CheckComponent<T>(entity);
        }

        // get component type of given component
        template<typename T>
        static ComponentType GetComponentType(){
            return componentManager->GetComponentType<T>();
        }
        
        // get signature of a component
        template<typename T>
        static Signature GetComponentSignature(){
            // create signature to fill
            Signature sig;
            
            // fill signature
            sig.set(componentManager->GetComponentType<T>());
            
            // return the signature
            return sig;
        }
        
        // get a signature with given component types
        template<typename... Args>
        static Signature GetMultiComponentSignature(Args... args){
            // check the amount of arguments to the max components
            static_assert(sizeof...(args) < MAX_COMPONENTS, "ERROR: Too many specified component, the max is 32");

            // check if args are of type ComponentType
            static_assert((std::is_same_v<Args, ComponentType> && ...), "ERROR: Invalid signature argument as all component types are of std::uint8_t"); 
            
            // create signature to fill
            Signature sig;
            // get value and grab amount of all arguments
            ComponentType arguments[] = { args... };

            // loop through all arguments and populate signature
            for(int i = 0; i < sizeof...(args); i++){
                sig.set(arguments[i]);
            }
            
            // return signature filled with given component types
            return sig;
        }
        
        // get signature bit of a component type
        template<typename T>
        static unsigned int GetComponentSignatureBit(){
            // create signature to fill
            Signature sig;
            
            // fill signature
            sig.set(componentManager->GetComponentType<T>());
            
            // return the signature
            return (unsigned int)sig.to_ulong();
        }

        //* System Functions

        // register a system, returns system reference for external use
        template<typename T>
        static std::shared_ptr<T> RegisterSystem(){
            return systemManager->RegisterSystem<T>();
        }

        // get reference of a registered system for external use
        template<typename T>
        static std::shared_ptr<T> GetSystem(){
            return systemManager->GetSystem<T>();
        }

        /* set the signatures that a system can use for in the entities

        */
        template<typename T>
        static void SetSystemSignature(Signature signature){
            systemManager->SetSignature<T>(signature);
        }

        /* set the signatures that a system can use for in the entities
            
        */
        template<typename T, typename... Args>
        static void SetSystemSignature(Args... args){
            // check if args are of type ComponentType
            static_assert((std::is_same_v<Args, ComponentType> && ...), "ERROR: Invalid signature argument as all component types are of std::uint8_t"); 
            
            // check the amount of arguments to the max components
            static_assert(sizeof...(args) < MAX_COMPONENTS, "ERROR: Too many specified signature component types, the max is 32");

            // create signature to fill
            Signature sig;
            // get value and grab amount of all arguments
            ComponentType arguments[] = { args... };

            // loop through all arguments and populate signature
            for(int i = 0; i < sizeof...(args); i++){
                sig.set(arguments[i]);
            }

            systemManager->SetSignature<T>(sig);
        }

    private:
        // private constructor
        ECS() {}

        // private pointer storage of the entity manager
        static std::unique_ptr<EntityManager> entityManager;

        // private pointer storage of the component manager
        static std::unique_ptr<ComponentManager> componentManager;

        // private pointer storage of the system manager
        static std::unique_ptr<SystemManager> systemManager;
};

#endif