#include <window/sdl_window.hpp>

// include standard libraries
#include <iostream>
#include <cstddef>
#include <chrono>

using namespace SDL;

Window::Window(){

}

Window::~Window(){
    // delete the keyboard state holder
    delete this->kState;

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

void Window::additionalWindowOptions(){
    // Disable v-sync
    SDL_GL_SetSwapInterval(0);
}

void Window::setUpOpenGL(){
    //set up rendering for 2D
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    SDL_GL_LoadLibrary(NULL);

    // set up OpenGL context
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    
    // call for additional window options
    additionalWindowOptions();

    // create window handle
    handle = SDL_CreateWindow(name, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(handle == nullptr){
        std::cout << "Failed to create window!" << std::endl;
        exit(-1);
    }

    // set the context
    context = SDL_GL_CreateContext(handle);
    if(context == NULL){
        std::cout << "Failed to create context!" << std::endl;
        exit(-1);
    }

    // initialize GLAD

    // load glad
    if (!gladLoadGL()) {
        std::cout << "Failed to init GLAD!" << std::endl;
        exit(-1);
    }
    // set openGL window size
    SDL_GetWindowSize(handle, &w, &h);
    glViewport(0, 0, w, h);

    // add additional OpenGL capabilities
    setUpOpenGL();

    // set up keyboard state holder
    this->kState = new KeyboardStateHolder();
    this->kState->keyboardState = (Uint8*)SDL_GetKeyboardState(NULL);

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
    if(handle == nullptr || kState == nullptr){
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
                if (eventHandle.window.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
                    // set openGL window size
                    SDL_GetWindowSize(handle, &width, &height);
                    glViewport(0, 0, width, height);
                }
                break;
            default:
                break;
          }

          // check for device IO events, works only when GamepadManager is initialized
          GamepadManager::PollIO();
        }

        // grab the current keyboard state
        this->kState->keyboardState = (Uint8*)SDL_GetKeyboardState(NULL);

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