#include <window/sisters_sdl_window.hpp>

// include std math
#include <cmath>

// include input headers
#include <input/sisters_sdl_keyboard.hpp>
#include <input/sisters_sdl_gamepad.hpp>
#include <input/sisters_sdl_mouse.hpp>

// include standard libraries
#include <iostream>
#include <cstddef>
#include <chrono>

using namespace SDL;

Window::Window(){

}

Window::~Window(){
    // close all created SDL joysticks
    for(int i = 0; i < g_QueuedGamepads.size(); i++){
        if(g_QueuedGamepads.at(i).gamepad->device != nullptr){
            // then close the joystick
            SDL_CloseGamepad(g_QueuedGamepads.at(i).gamepad->device);
        }
    }
    
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(handle);
    SDL_Quit();
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

void Window::setMouseWheelStopSpeed(float speed){
    this->mouseWheelStopSpeed = fabsf(speed);
}

void Window::setMouseWheelStopDeadzone(float deadzone){
    this->mouseWheelStopDeadzone = fabsf(deadzone);
}

void Window::additionalWindowOptions(){
    // Disable v-sync
    SDL_GL_SetSwapInterval(0);
}

void Window::setUpOpenGL(){
    //set up rendering for 2D
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::additionalEventHandling(SDL_Event* event){
    // used to handle any additional SDL events
}

double Window::getDeltaTime(){
    return DeltaTime;
} 

void Window::initializeWindow(int w, int h, const char* name){
    // check if handle was already made
    if(handle != nullptr){
        return; // stop function
    }

    // init SDL
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK)){
        std::cout << "Failed to initialize SDL!" << std::endl;
        exit(-1);
    }

    // set the width and height
    width = w;
    height = h;
    
    // Load default OpenGL
    if(!SDL_GL_LoadLibrary(NULL)){
        std::cout << "Failed to load default OpenGL!" << std::endl;
        exit(-1);
    }

    // check if using emscripten
    #ifdef __EMSCRIPTEN__
    // set up OpenGL ES 3.x context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    #else
    // set up OpenGL core 4.5 context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    #endif

    // create window handle
    handle = SDL_CreateWindow(name, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(handle == nullptr || handle == NULL){
        std::cout << "Failed to create window!" << std::endl;
        exit(-1);
    }
    
    // set the context
    context = SDL_GL_CreateContext(handle);
    if(context == NULL){
        std::cout << "Failed to create context!" << std::endl;
        exit(-1);
    }

    // call for additional window options
    additionalWindowOptions();

    // initialize GLAD
    
    // load glad
    if (!gladLoadGL()) {
        std::cout << "Failed to init GLAD!" << std::endl;
        exit(-1);
    }
    
    // set openGL window size
    if(!SDL_GetWindowSize(handle, &w, &h)){
        std::cout << "Failed to get SDL window size!" << std::endl;
        exit(-1);
    }
    glViewport(0, 0, w, h);

    // add additional OpenGL capabilities
    setUpOpenGL();

    // set up keyboard state holder
    SDL::g_KeyboardState.keyboardState = (Uint8*)SDL_GetKeyboardState(NULL);

    // show any errors or messages from SDL

    // TODO: Create debug options for the window class to display to a console
    // show any errors or messages
    // std::cout << "MSG: Window successfully created\n";
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
        std::cout << "ERROR: Window or keyboard state holder hasn't been initialized\n";
        return; // stop function
    }
    
    //create local vars for timing
    std::chrono::steady_clock::time_point frameStart, frameEnd;

    // call init for resource initialization
    init();

    while(!quit){
        // start timing the frame
        frameStart = std::chrono::steady_clock::now();

        // get current frame
        currentFrame = SDL_GetPerformanceCounter();

        // check for all queued sdl events
        while(SDL_PollEvent(&eventHandle)){
          // check the sdl events relating the window
          switch (eventHandle.type) {
            case SDL_EVENT_QUIT:
                quit = true;
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                // update window size and openGL viewport size
                SDL_GetWindowSize(handle, &width, &height);
                glViewport(0, 0, width, height);

                break;
            case SDL_EVENT_GAMEPAD_ADDED:
                // call for enable gamepad with the correspoding device
                SDL::enableGamepad(eventHandle.gdevice.which);
    
                break;
            case SDL_EVENT_GAMEPAD_REMOVED:
                // disable the gamepad that was removed
                SDL::disableGamepad(eventHandle.gdevice.which);
    
                break;
            case SDL_EVENT_MOUSE_MOTION:
                // update mouse position
                SDL::g_MouseState.x = eventHandle.motion.x;
                SDL::g_MouseState.y = eventHandle.motion.y;

                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                // enable button bit mask
                SDL::g_MouseState.button ^= SISTER_MOUSE_BUTTON_MASK(eventHandle.button.button);
                // check if button was double clicked
                if(eventHandle.button.clicks >= 2){
                    g_MouseState.isDoubleClicked = true;
                }else{
                    g_MouseState.isDoubleClicked = false;
                }

                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                // disable button bit mask
                SDL::g_MouseState.button ^= SISTER_MOUSE_BUTTON_MASK(eventHandle.button.button);

                break;
            case SDL_EVENT_MOUSE_WHEEL:
                // get mouse wheel amount
                SDL::g_MouseState.wheelY += eventHandle.wheel.y;
                
                break;
            case SDL_EVENT_WINDOW_MOUSE_ENTER:
                // set mouse within window to true
                SDL::g_MouseState.isWithinWindow = true;

                break;
            case SDL_EVENT_WINDOW_MOUSE_LEAVE:
                // set mouse within window to false
                SDL::g_MouseState.isWithinWindow = false;

                break;  
            default:    
                break;
          }
          // additional event handling
          additionalEventHandling(&eventHandle);
        }
        
        // accumulate time and do stepUpdate()
        this->accumulator += this->DeltaTime;
        while(this->accumulator >= fixedTimeStep){
            // update with fixed time step
            stepUpdate(this->fixedTimeStep);
            
            // reduce mouse wheel amount
            if(g_MouseState.wheelY > mouseWheelStopDeadzone){
                g_MouseState.wheelY -= mouseWheelStopSpeed;
            }else if(g_MouseState.wheelY < -mouseWheelStopDeadzone){
                g_MouseState.wheelY += mouseWheelStopSpeed;
            }else{
                g_MouseState.wheelY = 0.0f;
            }
            
            accumulator -= fixedTimeStep;
        } 
        // calculate alpha for linear interpolation
        this->alpha = accumulator / this->fixedTimeStep;

        // update any input, values, objects, loading etc..
        update();

        // clear buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // draw or render
        render(alpha);

        // swap buffers
        SDL_GL_SwapWindow(handle);

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

        // get the last frame
        lastFrame = SDL_GetPerformanceCounter();
        // calculate deltatime 
        DeltaTime = (lastFrame - currentFrame) / (double)SDL_GetPerformanceFrequency();
    }
}
