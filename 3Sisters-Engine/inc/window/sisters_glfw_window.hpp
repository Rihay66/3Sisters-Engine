#pragma once

#ifndef GLFW_WINDOW_HPP
#define GLFW_WINDOW_HPP

// check platform
#if defined(_WIN32) || defined(WIN32) // Windows platform
    #include <windows.h>
#endif

// GLAD and GLFW libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace GLFW{
/* Window abstract class used for creating a graphical context
 window that is used to allow for OpenGL capabilities.
 The window class provides various functions and most
 functions are overwritable.
 !It is recommended to inherit this class and override functions
*/
class Window{
    private:
        // set up vars for calculating delta time and the fixed time step
	    double lastFrame = 0, currentFrame = 0, frameDuration = 0, accumulator = 0,
            alpha = 0, threadSleepTime = 0;

        // vars storing and referencing to window size, width x height
        unsigned int width = 0, height = 0;

        /* Fixed rate that updates the stepUpdate(), adjust accordingly as needed
         @Default value is 16.6ms
        */
        double fixedTimeStep = 1.0f / 60.0f;

        /* Fixed rate that updates application, adjust accordingly as needed
         @Default value is 16.6ms
        */
        double targetTimeStep = 1.0f / 60.0f;

        // delta time variable for updating input, physics, and kind of movement
        double DeltaTime = 0;

        // window handle
        GLFWwindow* handle = nullptr;

    protected:
        // used to set the target frame time between frame, aka max frame time
        void setTargetTimeStep(double time);
        
        // used to set the fixed frame time between frame
        void setFixedTimeStep(double time);
        
        // used for adding additional glfw window hints
        virtual void additionalWindowOptions();

        /* used for setting up OpenGL rendering
         @Default is 2D rendering
        */
        virtual void setUpOpenGL();

        //TODO: Refactor the window state system to allow for robust changing input or going to debug, etc...

    public:
        // constructor
        Window();

        // destructor
        ~Window();

        //* Initialization functions

        /* used to initialize the window and it's contexts by default
          initializes GLFW and creates a Window with OpenGL 4.5 capabilities
          @Overwriting is not recommended, however due note that runtime(), getDeltaTime(),
          @setUpOpenGL() require GLFW to be initialized and have a created window handle context
        */
        virtual void initializeWindow(int w, int h, const char* name = "");

        //* Getters functions

        // used to grab refernce to the window handle context
        GLFWwindow* getWindowHandle() {return this->handle;}

        /* returns the current deltatime
          @Overwriting may need further modifications or accommodation to the update(), stepUpdate(), and render() as it may cause unintended behavior
        */
        virtual double getDeltaTime();

        // returns the width of the window
        unsigned int getWidth(){return this->width;}

        // return the height of the window
        unsigned int getHeight(){return this->height;}

        //* Virtual functions

        // used to call classes that handle the loading of shaders, textures, and objects
        virtual void init() = 0;

        // used to update Physics, ticks systems, or other at a fixed time step
        virtual void stepUpdate(double ts) = 0;
        
        // used to update logic, custom events, and other at the target time step
        virtual void update() = 0;

        // used to render things on the screen
        virtual void render(double alpha) = 0;
        
        /* Calls init() once, and then loops getDeltaTime(), update(), stepUpdate(), and render()
          Can be overwriten depending on the need of the game or application
        * @NOTE: it is a single threaded function
        * @NOTE: calls GLFW poll events, swap buffers and clears the OpenGL color buffer
          @If overwritten, may need to apply calculations of time step, fixed time step and accumulator yourself
        */
        void runtime();
};

}
#endif
