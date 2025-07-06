#include <window/sisters_glfw_window.hpp>

// include the namespace
using namespace GLFW;

// include input headers
#include <input/sisters_glfw_keyboard.hpp>
#include <input/sisters_glfw_gamepad.hpp>

// include standard libraries
#include <iostream>
#include <chrono>

// callback function to move the OpenGL viewport to the GLFW window's position
static void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

// callback function to check for keyboard inputs and apply them to the global keyboard state
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key < 0)
        return;
    
    switch(action){
        case GLFW_PRESS:
            g_KeyboardState[key] = true;
            break;
        case GLFW_RELEASE:
            g_KeyboardState[key] = false;
            break;
        default:
            break;
    }
}

static void controller_connection_callback(int jid, int event){
    // check event type
    if(event == GLFW_CONNECTED){
        // create or enable a gamepad
        enableGamepad(jid);
    }else if(event == GLFW_DISCONNECTED){
        // disable a gamepad
        disableGamepad(jid);
    }  
}

// constructor
Window::Window() {
    
}

// destructor
Window::~Window(){
    // destroy window handle
    glfwDestroyWindow(handle);

    // delete any pointers
    glfwTerminate();
}

void Window::initializeWindow(int w, int h, const char* name){
    // allow the initialization of the window to be called ONLY once

    // check if the handle is already set 
    if(handle != nullptr)
        return;

    // init GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW!" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    
    // set the width and height
    width = w;
    height = h;

    // set specific opengl version to 4.5
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    // set up opengl window profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // add additional hints
    additionalWindowOptions();

    // create the window and check for errors
    handle = glfwCreateWindow(width, height, name, NULL, NULL);
    if (!handle) {
        std::cout << "Failed to create window!" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    // create window
    glfwMakeContextCurrent((GLFWwindow*)handle);

    // load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD!" << std::endl;
        exit(-1);
    }

    //* set up callbacks
    
    // set up call back to update the opengl window
    glfwSetFramebufferSizeCallback((GLFWwindow*)handle, framebuffer_size_callback);
    // set up call back to update the keyboard state
    glfwSetKeyCallback((GLFWwindow*)handle, key_callback);
    
    // check all slots for pre-connected devices
    for(int i = 0; i < GLFW_JOYSTICK_LAST; i++){
        // check if it is a gamepad and it's slot doesn't exist
        if(glfwJoystickIsGamepad(i) == GLFW_TRUE && g_controllerDevices.at(i) == nullptr){
            // create controller device
            g_controllerDevices.at(i) = std::make_shared<ControllerDevice>();
            // set the ID
            g_controllerDevices.at(i)->ID = i;
            // then set the connection to be true
            g_controllerDevices.at(i)->isConnected = true;
        }
    }
    
    // throw ID into a connection or disconnect events
    glfwSetJoystickCallback(controller_connection_callback);
    
    // set openGL window size
    glViewport(0, 0, width, height);

    // add additional OpenGL capabilities
    setUpOpenGL();

    // TODO: Create debug options for the window class to display to a console
    // show any errors or messages
    // std::cout << "MSG: Window successfully created\n";
}

void Window::setTargetTimeStep(double time){
    // check the passed parameter if it less than the fixed time step
    if(this->fixedTimeStep >= time){
        // set the time step to given parameter
        this->targetTimeStep = time;
    }
}

void Window::setFixedTimeStep(double time){
    // check the passed parameter is less more than target time step
    if(this->targetTimeStep <= time){
        // set the fixed time step to given parameter
        this->fixedTimeStep = time;
    }
}

void Window::additionalWindowOptions(){
    // add additional glfw window hints, options, etc.

    // disable vsync by default
    glfwSwapInterval(0);
}

void Window::setUpOpenGL(){
    // set up rendering for 2D
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

double Window::getDeltaTime(){
    // get current frame 
    currentFrame = glfwGetTime();
    // calculate delta time
    DeltaTime = currentFrame - lastFrame;
    // ensure deltatime does not increase over 0.25ms
    if(DeltaTime > 0.25)
        DeltaTime = 0.25;
    
    // set the last frame to be current frame
    lastFrame = currentFrame;

    return DeltaTime;
}

// initialization
void Window::init(){
    // here goes the initial processing of shaders, textues, and objects
}

//updating
void Window::update(){
    // update any variables like moving objects or updating input
    //* NOTE: that any object that needs input will need to have reference to the window handles a parameter to be passed down
}

// update physics or ticks
void Window::stepUpdate(double ts){
    // used to update physics, ticks, or other with a fixed time step 
}

// rendering
void Window::render(double alpha){
    // here update visually the objects, shaders, textures, etc
}

// single threaded runtime of input(), update(), stepUpdate() and render()
void Window::runtime(){
    // check if GLFW has been initialized
    if(handle == nullptr){
        //! display error
        std::cout << "ERROR: Window hasn't been initialized\n";
        return; // stop function
    }
    
    //create local vars for timing
    std::chrono::steady_clock::time_point frameStart, frameEnd;

    // call init for resource initialization
    init();

    while(!glfwWindowShouldClose((GLFWwindow*)this->handle)){
        // start timing the frame
        frameStart = std::chrono::steady_clock::now();

        // get Deltatime
        this->getDeltaTime();

        // check for glfw events
        glfwPollEvents();
        
        // loop through available gamepads and updated their states
        for(int i = 0; i < g_controllerDevices.size(); i++){
            if(g_controllerDevices.at(i) != nullptr && g_controllerDevices.at(i)->isConnected){
                // update state
                glfwGetGamepadState(i, &g_controllerDevices.at(i)->state);
            }
        }

        // gran current window size
        glfwGetWindowSize((GLFWwindow*)handle, &width, &height);
        
        //  accumulate time and do stepUpdate()
        this->accumulator += this->DeltaTime;
        while(this->accumulator >= fixedTimeStep){
            // update with fixed time step
            stepUpdate(this->fixedTimeStep);
            accumulator -= fixedTimeStep;
        } 
        // calculate alpha for linear interpolation
        this->alpha = accumulator / this->fixedTimeStep;

        // update any input, values, objects, loading etc..
        update();

        // clear screen
        glClear(GL_COLOR_BUFFER_BIT);

        // draw or render
        render(alpha);

        // swap buffers
        glfwSwapBuffers((GLFWwindow*)this->handle);

        // end timing the frame
        frameEnd = std::chrono::steady_clock::now();

        // calculate the duration of the frame
        frameDuration = std::chrono::duration<double>(frameEnd - frameStart).count();

        // calculate the time to sleep to achieve the desired time step
        threadSleepTime = targetTimeStep - frameDuration;
        if(threadSleepTime > 0){
            // sleep until specified thread sleep time
            auto start = std::chrono::high_resolution_clock::now();
            auto end = start + std::chrono::duration<double>(threadSleepTime);
            // sleep
            while (std::chrono::high_resolution_clock::now() < end){}
        }
    }
}