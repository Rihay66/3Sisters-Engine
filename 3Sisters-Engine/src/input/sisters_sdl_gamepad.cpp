#include <input/sisters_sdl_gamepad.hpp>

#include <cstdint>

using namespace SDL;

bool SDL::getButtonInput(Gamepad &pad, int key){
    // check if gamepad is set and connected
    if(pad.device != nullptr && pad.isConnected){
        // check state of given key
        if(SDL_GetGamepadButton(pad.device, (SDL_GamepadButton)key) > 0){
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