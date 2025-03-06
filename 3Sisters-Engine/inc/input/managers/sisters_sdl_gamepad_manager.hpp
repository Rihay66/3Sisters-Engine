#pragma once

#ifndef SISTERS_SDL_GAMEPAD_MANAGER_HPP
#define SISTERS_SDL_GAMEPAD_MANAGER_HPP

// include standard library
#include <vector>

// include gamepad
#include <input/sisters_sdl_gamepad.hpp>

// include SDL libraries
#include <SDL3/SDL.h>

namespace SDL{
/* A static singleton GamepadManager class that hosts 
 several functions to query for connected or disconnected 
 Gamepads. Each obtained connected controller are stored for
 reference to allow for further usage of the gamepad.
 This class keeps track of all connected or diconnected 
 devices. All functions and resources are static and no 
 public constructor is defined.  
*/
class GamepadManager{
    public:
        //* helper functions

        // set query event callback, expands a list of connected and disconnected devices
        static void InitializeQuery(SDL_Event& handle);

        //* getter functions

        // returns the number of devices that identify as "Gamepads"
        static int GetGamepadAmount();

        /* add a gamepad reference that can be filled from the list of queried gamepads
        * @NOTE: By default picks and sets the first gamepad with the highest priority number
        * @NOTE: Multiple gamepads aiming to be the same priority, only one will be set
        */
        static void SetGamepad(SDL::Gamepad& gamepad, int priority = 0);

        /* polls event from gamepads such as connections and disconnects
        * @NOTE: Already called by SDL Window runtime()
        */
        static void PollIO();

    private:
        // define a queued gamepad that needs to be set
        struct QueuedGamepad{
            // wanted priority to be set, default picks a NULL value
            int priority = 0;
            // stored reference of the gamepad that needs to be set
            SDL::Gamepad* gamepad = nullptr;
        };

        //* private resource storage

        // define a dynamic list of yet to be set gamepads components from objects or entities
        static std::vector<QueuedGamepad> queuedGamepads;

        // contain reference to the event handle
        static SDL_Event* eventHandle;

        // private constructor, that is to avoid any actual gamepad manager objects
        GamepadManager() {}

        //* private helper functions

        // create a gamepad reference to be used on connection
        static void enableGamepad(int index);

        // disable gamepad reference when disconnected
        static void disableGamepad(int index);

        // properly de-allocates all resources
        static void clear();

        //! Currently EXPERIMENTAL, may cause exceptions or segfaults
        // private boolean to track automatic clear()
        static bool isAutoClearSet;
        // set up automatic de-allocation of loaded resources
        static void setUpAutoClear();
};
}

#endif