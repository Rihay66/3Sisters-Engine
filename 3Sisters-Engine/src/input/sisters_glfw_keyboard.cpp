#include <input/sisters_glfw_keyboard.hpp>

// define static global
bool GLFW::g_KeyboardState[SISTER_KEY_LAST];

bool GLFW::getKeyInput(int key){
    // check for input using GLFW
    if(GLFW::g_KeyboardState[key]){
        // return true if the key was pressed
        return true;
    }

    // return false if the key wasn't pressed
    return false;
}

bool GLFW::getKeyInputDown(int key){
    // check for input using GLFW
    if(GLFW::g_KeyboardState[key]){
        // return true if the key was pressed
        return true;
    }

    // return false if the key wasn't pressed
    return false;
}

bool GLFW::getKeyInputUp(int key){
    // check for input using GLFW
    if(!GLFW::g_KeyboardState[key]){
        // return true if the key was pressed
        return true;
    }

    // return false if the key wasn't pressed
    return false;
}