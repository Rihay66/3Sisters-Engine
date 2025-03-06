#include <input/managers/sisters_sdl_gamepad_manager.hpp>

#include <cstdlib>
#include <SDL3/SDL.h>

//? include IO for debug prints
#include <iostream>

using namespace SDL;

// define static variables
std::vector<GamepadManager::QueuedGamepad>              GamepadManager::queuedGamepads;
SDL_Event*                                              GamepadManager::eventHandle = nullptr;
bool                                                    GamepadManager::isAutoClearSet = false;

void GamepadManager::InitializeQuery(SDL_Event& event){
    // check if SDL hasn't been initialized
    if(!SDL_WasInit(SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK)){
        std::cout << "ERROR: SDL is not initialized!\n";
        return; // stop functions 
    }

    // set the event handle 
    eventHandle = &event;
}

int GamepadManager::GetGamepadAmount(){
    // init var to contain actual amount of devices available
    int amount = 0;

    SDL_GetGamepads(&amount);

    return amount;
}

void GamepadManager::SetGamepad(Gamepad& gamepad, int priority){
    // set auto clear
    setUpAutoClear();

    // check if the gamepad already exists
    for(QueuedGamepad& pad : queuedGamepads){
        // check if the same
        if(gamepad.device != nullptr && gamepad.device == pad.gamepad->device && pad.priority == priority){
            // stop function
            std::cout << "Warning: Gamepad component to be set already exists or has similar priority number\n";
            return;
        }
    }

    // create make given gamepad set to be queried
    QueuedGamepad pad;
    pad.gamepad = &gamepad;
    pad.priority = priority;

    // add queried gamepad to list
    queuedGamepads.push_back(pad);
}

void GamepadManager::PollIO(){
    // check if the event handle is set meaning, there is intention of using the gamepad system
    if(eventHandle == nullptr){
        return; // stop functions
    }

    // check if SDL and event handle hasn't been initialized
    if(!SDL_WasInit(SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK)){
        std::cout << "ERROR: SDL is not initialized!\n";
        return; // stop functions
    }

    // check for events
    switch(eventHandle->type){
        case SDL_EVENT_GAMEPAD_ADDED:
            // call for enable gamepad with the correspoding device
            enableGamepad(eventHandle->gdevice.which);

            break;
        case SDL_EVENT_GAMEPAD_REMOVED:
            // disable the gamepad that was removed
            disableGamepad(eventHandle->gdevice.which);

            break;
        default:
            break;
    }
}


void GamepadManager::enableGamepad(int jid){
    // create a new gamepad using the given JID
    SDL_Gamepad* gamepad = SDL_OpenGamepad(jid);
    
    //std::cout << "Enabling controller at: " << jid << "\n";

    // set pad into the queried and then set pads according to priority
    
    // find the lowest number, meaning they are high priority to be set
    int priority = 0;
    for(QueuedGamepad& pads : queuedGamepads){
        if(pads.gamepad->device == nullptr && pads.priority < priority){
            priority = pads.priority;
        }
    }

    // with the priority number then set the pad with such priority number
    for(QueuedGamepad& pads : queuedGamepads){
        if(pads.priority == priority){
            pads.gamepad->device = gamepad;
            pads.gamepad->isConnected = true;
            //std::cout << "Gamepad set\n";
        }
    }
}

void GamepadManager::disableGamepad(int jid){
    //std::cout << "Closing controller at: " << jid << "\n";
    // check for given JID which device was removed and properly close it
    for(QueuedGamepad& pads : queuedGamepads){
        // check if pad is set
        if(pads.gamepad->device != nullptr && SDL_GetGamepadID(pads.gamepad->device) == jid){
            // then close the joystick
            SDL_CloseGamepad(pads.gamepad->device);
            pads.gamepad->device = nullptr;
            pads.gamepad->isConnected = false;
        }
    }
    
}

void GamepadManager::clear(){
    // de-allocate resources

    // close all created SDL joysticks
    for(int i = 0; i < queuedGamepads.size(); i++){
        if(queuedGamepads.at(i).gamepad->device != nullptr){
            // then close the joystick
            SDL_CloseGamepad(queuedGamepads.at(i).gamepad->device);
        }
    }
}

void GamepadManager::setUpAutoClear(){
    // set up on exit to call the Clear()
    if(!isAutoClearSet && std::atexit(clear) == 0){
        isAutoClearSet = true; // disable calling this function again
    }
}