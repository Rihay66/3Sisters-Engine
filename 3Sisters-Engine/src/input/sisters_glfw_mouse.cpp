#include <input/sisters_glfw_mouse.hpp>

// include std definitions
#include <cstddef>

// define global
struct GLFW::MouseStateHolder GLFW::g_MouseState;

void GLFW::getMousePosition(double *x, double *y){
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

bool GLFW::getMouseInput(unsigned int button){
    // check if mouse is within window
    if(g_MouseState.isWithinWindow){
        if((g_MouseState.button & (button + 1)) == (button + 1)){
            return true;
        }
    }

    return false;
}

double GLFW::getMouseWheelYRaw(){
    return g_MouseState.wheelY;
}

bool GLFW::getMouseWheelY(){
    if(g_MouseState.wheelY > 0.0f || g_MouseState.wheelY < 0.0f){
        return true;
    }
    
    return false;
}

bool GLFW::getMouseFocus(){
    return g_MouseState.isWithinWindow;
}
