#include "../inc/test_window.hpp"
#include <iostream>

int main(int argc, char* argv[]){

    TestWindow window;

    window.initializeWindow(1280, 720);

    //? debug print of GL version
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

    window.runtime();

    return 0;
}