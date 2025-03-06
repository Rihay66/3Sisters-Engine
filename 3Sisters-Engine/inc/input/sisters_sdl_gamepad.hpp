#pragma once

#ifndef SISTERS_SDL_GAMEPAD_HPP
#define SISTERS_SDL_GAMEPAD_HPP

// include SDL
#include <SDL3/SDL.h>

namespace SDL{

//? Playstation specific button macros

#define PLAYSTATION_BUTTON_CROSS 0
#define PLAYSTATION_BUTTON_CIRCLE 1
#define PLAYSTATION_BUTTON_SQUARE 2
#define PLAYSTATION_BUTTON_TRIANGLE 3
#define PLAYSTATION_BUTTON_L1 4
#define PLAYSTATION_BUTTON_R1 5
#define PLAYSTATION_BUTTON_SELECT 6
#define PLAYSTATION_BUTTON_START 7
#define PLAYSTATION_BUTTON_PS 8
#define PLAYSTATION_BUTTON_L3 9
#define PLAYSTATION_BUTTON_R3 10
#define PLAYSTATION_BUTTON_DPAD_UP 11
#define PLAYSTATION_BUTTON_DPAD_RIGHT 12
#define PLAYSTATION_BUTTON_DPAD_DOWN 13
#define PLAYSTATION_BUTTON_DPAD_LEFT 14

#define PLAYSTATION_JOYSTICK_LEFT_X 0
#define PLAYSTATION_JOYSTICK_LEFT_Y 1
#define PLAYSTATION_JOYSTICK_RIGHT_X 2
#define PLAYSTATION_JOYSTICK_RIGHT_Y 3
#define PLAYSTATION_L2_TRIGGER_LEFT 4
#define PLAYSTATION_R2_TRIGGER_RIGHT 5

//? Xbox specific button macros

#define XBOX_BUTTON_A 0
#define XBOX_BUTTON_B 1
#define XBOX_BUTTON_X 2
#define XBOX_BUTTON_Y 3
#define XBOX_BUTTON_LEFT_BUMPER 4
#define XBOX_BUTTON_RIGHT_BUMPER 5
#define XBOX_BUTTON_BACK 6
#define XBOX_BUTTON_START 7
#define XBOX_BUTTON_GUIDE 8
#define XBOX_BUTTON_LEFT_THUMB 9
#define XBOX_BUTTON_RIGHT_THUMB 10
#define XBOX_BUTTON_DPAD_UP 11
#define XBOX_BUTTON_DPAD_RIGHT 12
#define XBOX_BUTTON_DPAD_DOWN 13
#define XBOX_BUTTON_DPAD_LEFT 14

#define XBOX_JOYSTICK_LEFT_X 0
#define XBOX_JOYSTICK_LEFT_Y 1
#define XBOX_JOYSTICK_RIGHT_X 2
#define XBOX_JOYSTICK_RIGHT_Y 3
#define XBOX_LEFT_TRIGGER 4
#define XBOX_RIGHT_TRIGGER 5

//? Nintendo specific button macros

#define NINTENDO_BUTTON_B 0
#define NINTENDO_BUTTON_A 1
#define NINTENDO_BUTTON_Y 2
#define NINTENDO_BUTTON_X 3
#define NINTENDO_BUTTON_LEFT_BUMPER 4
#define NINTENDO_BUTTON_RIGHT_BUMPER 5
#define NINTENDO_BUTTON_MINUS 6
#define NINTENDO_BUTTON_PLUS 7
#define NINTENDO_BUTTON_GUIDE 8
#define NINTENDO_BUTTON_LEFT_THUMB 9
#define NINTENDO_BUTTON_RIGHT_THUMB 10
#define NINTENDO_BUTTON_DPAD_UP 11
#define NINTENDO_BUTTON_DPAD_RIGHT 12
#define NINTENDO_BUTTON_DPAD_DOWN 13
#define NINTENDO_BUTTON_DPAD_LEFT 14

#define NINTENDO_JOYSTICK_LEFT_X 0
#define NINTENDO_JOYSTICK_LEFT_Y 1
#define NINTENDO_JOYSTICK_RIGHT_X 2
#define NINTENDO_JOYSTICK_RIGHT_Y 3
#define NINTENDO_LEFT_TRIGGER 4
#define NINTENDO_RIGHT_TRIGGER 5

// define structure of a gamepad
struct Gamepad{
    // connection state of this gamepad
    bool isConnected = false;
    // keep reference of the controller
    SDL_Gamepad* device = nullptr;
};

//? Gamepad functions

/* button callback of the gamepad which checks for the given button
* @NOTE: Requires a set gamepad
*/ 
bool getButtonInput(Gamepad& pad, int key);

/* axis callback of the gamepad which returns a incremental value when there is input of the given key
* @NOTE: Requires a set gamepad
*/
float getAxisRawInput(Gamepad& pad, int key, float deadzone = 0.5f);

/* axis callback of the gamepad which returns a value of direction when there is input of the given key
* @NOTE: Requires a set gamepad
*/
float getAxisInput(Gamepad& pad, int key, float deadzone = 0.5f);

}

#endif