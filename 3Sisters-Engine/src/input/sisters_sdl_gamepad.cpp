#include <input/sisters_sdl_gamepad.hpp>

// include standard libarry for printing
#include <iostream>

using namespace SDL;

// define global
std::vector<SDL::QueuedGamepad> SDL::g_QueuedGamepads;

int SDL::getGamepadAmount(){
    // init var to contain actual amount of devices available
    int amount = 0;

    SDL_GetGamepads(&amount);

    return amount;
}

void SDL::setGamepad(Gamepad& gamepad, int priority){
    // check if the gamepad already exists
    for(QueuedGamepad& pad : g_QueuedGamepads){
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
    g_QueuedGamepads.push_back(pad);
}

void SDL::enableGamepad(int jid){
    // create a new gamepad using the given JID
    SDL_Gamepad* gamepad = SDL_OpenGamepad(jid);

    // set pad into the queried and then set pads according to priority
    
    // find the lowest number, meaning they are high priority to be set
    int priority = 0;
    for(QueuedGamepad& pads : g_QueuedGamepads){
        if(pads.gamepad->device == nullptr && pads.priority < priority){
            priority = pads.priority;
        }
    }

    // with the priority number then set the pad with such priority number
    for(QueuedGamepad& pads : g_QueuedGamepads){
        if(pads.priority == priority){
            pads.gamepad->device = gamepad;
            pads.gamepad->isConnected = true;
            //std::cout << "Gamepad set\n";
        }
    }
}

void SDL::disableGamepad(int jid){
    // check for given JID which device was removed and properly close it
    for(QueuedGamepad& pads : g_QueuedGamepads){
        // check if pad is set
        if(pads.gamepad->device != nullptr && SDL_GetGamepadID(pads.gamepad->device) == jid){
            // then close the joystick
            SDL_CloseGamepad(pads.gamepad->device);
            pads.gamepad->device = nullptr;
            pads.gamepad->isConnected = false;
        }
    }
    
}

bool SDL::getButtonInput(Gamepad &pad, int key){
    // check if gamepad is set and connected
    if(pad.device != nullptr && pad.isConnected){
        // check state of given key
        if(SDL_GetGamepadButton(pad.device, (SDL_GamepadButton)key)){
            return true;
        }
    }

    return false;
}

float SDL::getAxisRawInput(Gamepad &pad, int axis, float deadzone){
    // check if gamepad is set and connected
    if(pad.device != nullptr && pad.isConnected){
        // grab current value
        float input = (float)SDL_GetGamepadAxis(pad.device, (SDL_GamepadAxis)axis) / (float)INT16_MAX;
        // check state of given key
        if(input > deadzone){
            return input;
        }else if(input < -deadzone){
            return input;
        }
    }

    return 0.0f;
}

float SDL::getAxisInput(Gamepad &pad, int axis, float deadzone){
    // check if gamepad is set and connected
    if(pad.device != nullptr && pad.isConnected){
        // grab current value
        float input = (float)SDL_GetGamepadAxis(pad.device, (SDL_GamepadAxis)axis) / (float)INT16_MAX;
        // check state of given key
        if(input > deadzone){
            return 1.0f;
        }else if(input < -deadzone){
            return -1.0f;
        }
    }

    return 0.0f;
}

bool SDL::applyRumble(Gamepad &pad, uint16_t leftIntensity, uint16_t rightIntensity, uint32_t duration_ms){
    // check if gamepad is set and connected
    if(pad.device != nullptr && pad.isConnected){
        // apply rumble
        return SDL_RumbleGamepad(pad.device, leftIntensity, rightIntensity, duration_ms);
    }
    
    return false;
}