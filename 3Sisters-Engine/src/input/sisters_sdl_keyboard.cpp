#include <input/sisters_sdl_keyboard.hpp>

// define global
struct SDL::KeyboardStateHolder SDL::g_KeyboardState;

bool SDL::getKeyInput(int key){
    // grab the keyboard state holder, then check for given scancode when pressed down
    if(SDL::g_KeyboardState.keyboardState[key] > 0){
        return true;
    }

    return false;
}

bool SDL::getKeyInputDown(int key){
    // grab the keyboard state holder, then check for given scancode when pressed down
    if(SDL::g_KeyboardState.keyboardState[key] > 0){
        return true;
    }

    return false;
}

bool SDL::getKeyInputUp(int key){
    // grab the keyboard state holder, then check for given scancode when not pressed
    if(SDL::g_KeyboardState.keyboardState[key] == 0){
        return true;
    }

    return false;
}