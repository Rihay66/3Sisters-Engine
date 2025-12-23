#pragma once

#ifndef SISTERS_SDL_MOUSE_HPP
#define SISTERS_SDL_MOUSE_HPP

namespace SDL{
//? Engine renamed mouse scancode to allow for mouse inputs

#define SISTER_MOUSE_BUTTON_LEFT     1
#define SISTER_MOUSE_BUTTON_MIDDLE   2
#define SISTER_MOUSE_BUTTON_RIGHT    3
#define SISTER_MOUSE_BUTTON_X1       4
#define SISTER_MOUSE_BUTTON_X2       5

#define SISTER_MOUSE_BUTTON_MASK(X)  (1u << ((X)-1))

// define mouse state holder
struct MouseStateHolder{
    float x, y;
    float wheelY = 0.0f;
    unsigned int button;
    bool isWithinWindow = true;
    bool isDoubleClicked = false;
};

// define global mouse state holder
extern struct MouseStateHolder g_MouseState;

// check for mouse position
void getMousePosition(float* x, float* y);

// check for mouse button input
bool getMouseInput(unsigned int button);

// check for mouse button input that is clicked more than once
bool getMouseDoubleInput(unsigned int button);

// check for vertical mouse wheel
float getMouseWheelYRaw();

// check for vertical mouse wheel raw input
bool getMouseWheelY();

// check if mouse is within the window
bool getMouseFocus();

}
#endif
