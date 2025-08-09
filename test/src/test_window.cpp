#include "../inc/test_window.hpp"

void TestWindow::init(){  
    
}

void TestWindow::stepUpdate(double ts){
   
}

void TestWindow::update(){

}

void TestWindow::render(double alpha){
    // recalculate camera projection
    camera.calculateProjectionView(ShaderManager::GetShader("quad"));
    camera.calculateProjectionView(ShaderManager::GetShader("line"));
    camera.calculateProjectionView(ShaderManager::GetShader("text"));

    // render background
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}
