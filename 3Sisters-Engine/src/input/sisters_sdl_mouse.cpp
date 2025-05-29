#include <input/sisters_sdl_mouse.hpp>

#include <cstddef>

// define global
struct SDL::MouseStateHolder SDL::g_MouseState;

void SDL::getMousePosition(float *x, float *y){
    // check if mouse is within window
    if(g_MouseState.isWithinWindow){
        if(x != NULL){
            *x = g_MouseState.x;
        }

        if(y != NULL){
            *y = g_MouseState.y;
        }
    }
}

bool SDL::getMouseInput(unsigned int button){
    // check if mouse is within window
    if(g_MouseState.isWithinWindow){
        if((g_MouseState.button & SISTER_MOUSE_BUTTON_MASK(button)) == SISTER_MOUSE_BUTTON_MASK(button)){
            return true;
        }
    }

    return false;
}

bool SDL::getMouseDoubleInput(unsigned int button){
    // check if mouse is within window
    if(g_MouseState.isWithinWindow){
        if((g_MouseState.button & SISTER_MOUSE_BUTTON_MASK(button)) == SISTER_MOUSE_BUTTON_MASK(button)
            && g_MouseState.isDoubleClicked){
            return true;
        }
    }

    return false;
}

float SDL::getMouseWheelY(){
    return g_MouseState.wheelY;
}

bool SDL::getMouseWheelYRaw(){
    if(g_MouseState.wheelY > 0.0f || g_MouseState.wheelY < 0.0f){
        return true;
    }
    
    return false;
}

bool SDL::getMouseFocus(){
    return g_MouseState.isWithinWindow;
}