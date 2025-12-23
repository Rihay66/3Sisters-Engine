#pragma once

#ifndef SISTERS_GLFW_MOUSE_HPP
#define SISTERS_GLFW_MOUSE_HPP

namespace GLFW {
//? Engine renamed mouse scancode to allow for mouse inputs

#define SISTER_MOUSE_BUTTON_LEFT 0
#define SISTER_MOUSE_BUTTON_RIGHT 1
#define SISTER_MOUSE_BUTTON_MIDDLE 2
#define SISTER_MOUSE_BUTTON_X1 3
#define SISTER_MOUSE_BUTTON_X2 4
#define SISTER_MOUSE_BUTTON_X3 5
#define SISTER_MOUSE_BUTTON_X4 6
#define SISTER_MOUSE_BUTTON_X5 7

// define mouse state holder
struct MouseStateHolder{
    double x, y;
    double wheelY = 0.0;
    unsigned int button;
    bool isWithinWindow = true;
};

// define global mouse state holder
extern struct MouseStateHolder g_MouseState;

// check for mouse position
void getMousePosition(double* x, double* y);

// check for mouse button input
bool getMouseInput(unsigned int button);

// check for vertical mouse wheel
double getMouseWheelYRaw();

// check for vertical mouse wheel raw input
bool getMouseWheelY();

// check if mouse is within the window
bool getMouseFocus();

}
#endif
