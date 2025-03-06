#pragma once

#ifndef SISTERS_GLFW_GAMEPAD_MANAGER_HPP
#define SISTERS_GLFW_GAMEPAD_MANAGER_HPP

// include standard libraries
#include <array>
#include <vector>

// include gamepad
#include <input/sisters_glfw_gamepad.hpp>

//TODO: Add debug options to display info of the gamepads

namespace GLFW{
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
        static void InitializeQuery();

        //* getter functions

        // returns the number of devices that identify as "Gamepads"
        static int GetGamepadAmount();

        /* add a gamepad reference that can be filled from the list of queried gamepads
        * @NOTE: By default picks the first gamepad
        */
        static void SetGamepad(GLFW::Gamepad& gamepad, int index = 0);

        /* polls input from the connected gamepads to allow for checking for inputs
        */
        static void PollIO();

    private:
        // define a queued gamepad that needs to be set
        struct QueuedGamepad{
            // wanted index to be set, default picks a NULL value
            int index = -1;
            // stored reference of the gamepad that needs to be set
            GLFW::Gamepad* gamepad = nullptr;
        };

        //* private resource storage

        // define a dynamic list of yet to be set gamepads components from objects or entities
        static std::vector<QueuedGamepad> queuedGamepads;

        // define a list of game controllers detected by GLFW
        static std::array<std::shared_ptr<GLFW::ControllerDevice>, 15> devices;
 
        // private constructor, that is to avoid any actual gamepad manager objects
        GamepadManager() {}

        // properly de-allocates all resources
        static void clear();

        //* private helper functions

        // glfw callback to check connection of a gamepad
        static void connectionCheck(int jid, int event); 

        // create a gamepad reference to be used on connection
        static void enableGamepad(int jid);

        // disable gamepad reference when disconnected
        static void disableGamepad(int jid);

        //! Currently EXPERIMENTAL, may cause exceptions or segfaults
        // private boolean to track automatic clear()
        static bool isAutoClearSet;
        // set up automatic de-allocation of loaded resources
        static void setUpAutoClear();
};
}

#endif