#include <input/managers/sisters_glfw_gamepad_manager.hpp>

//? include IO for debug prints
#include <iostream>

using namespace GLFW;

// define static variables
std::vector<GamepadManager::QueuedGamepad>              GamepadManager::queuedGamepads;
std::array<std::shared_ptr<ControllerDevice>, 15>       GamepadManager::devices;
bool                                                    GamepadManager::isAutoClearSet = false;


void GamepadManager::InitializeQuery(){
    // check that GLFW has not been initialized
    if(glfwGetError(NULL) == GLFW_NOT_INITIALIZED){
        std::cout << "ERROR: GLFW is not initialized!\n";
        return; // stop function
    }

    // check all slots for pre-connected devices
    for(int i = 0; i < GLFW_JOYSTICK_LAST; i++){
        // check if it is a gamepad and it's slot doesn't exist
        if(glfwJoystickIsGamepad(i) == GLFW_TRUE && devices.at(i) == nullptr){
            // create controller device
            devices.at(i) = std::make_shared<ControllerDevice>();
            // set the ID
            devices.at(i)->ID = i;
            // then set the connection to be true
            devices.at(i)->isConnected = true;
        }
    }

    // throw ID into a connection or disconnect events
    glfwSetJoystickCallback(connectionCheck);
}

int GamepadManager::GetGamepadAmount(){
    // init var to contain actual amount of devices available
    int amount = 0;

    for(int i = 0; i < GLFW_JOYSTICK_LAST; i++){
        if(devices.at(i) != nullptr){
            amount++;
        }else{
            break; // stop loop
        }
    }

    return amount;
}

void GamepadManager::SetGamepad(Gamepad& gamepad, int index){
    // set auto clear
    setUpAutoClear();

    // check index is within range and if reference already exists
    if(index < GLFW_JOYSTICK_LAST && devices.at(index) != nullptr){
        // set the reference
        gamepad.device = devices.at(index);

        return; // stop function
    }else if(index < GLFW_JOYSTICK_LAST && devices.at(index) == nullptr){
        // queue the given gamepad
        
        QueuedGamepad qp;
        qp.index = index;
        qp.gamepad = &gamepad;

        // add to queue
        queuedGamepads.push_back(qp);
    }
}

void GamepadManager::PollIO(){    
    // check that GLFW has not been initialized
    if(glfwGetError(NULL) == GLFW_NOT_INITIALIZED){
        std::cout << "ERROR: GLFW is not initialized!\n";
        return; // stop function
    }

    // loop through available gamepads
    for(int i = 0 ; devices.size(); i++){
        if(devices.at(i) != nullptr && devices.at(i)->isConnected){
            // update state
            glfwGetGamepadState(i, &devices.at(i)->state);
        }else if(devices.at(i) == nullptr){
            return; // stop function 
        }
    }
}

void GamepadManager::connectionCheck(int jid, int event){
    // check event type
    if(event == GLFW_CONNECTED){
        // create or enable a gamepad
        enableGamepad(jid);
    }else if(event == GLFW_DISCONNECTED){
        // disable a gamepad
        disableGamepad(jid);
    }  
}

void GamepadManager::enableGamepad(int jid){
    // check if ID already exists
    for(int i = 0; i < devices.size(); i++){
        if(devices.at(i) != nullptr && devices.at(i)->ID == jid){
            // gamepad exists, then set the connection to be true
            devices.at(i)->isConnected = true;

            // stop function
            return;
        }
    }

    // if ID doesn't exist then create one
    if(jid < GLFW_JOYSTICK_LAST){
        // create a new gamepad
        devices.at(jid) = std::make_shared<ControllerDevice>();
        // set the ID
        devices.at(jid)->ID = jid;
        // then set the connection to be true
        devices.at(jid)->isConnected = true;

        // loop in reverse through queued up gamepads that need to be set
        for(int i = 0; i < queuedGamepads.size(); i++){
            if(queuedGamepads.at(i).index == jid && queuedGamepads.at(i).gamepad != nullptr){
                // set the gamepad
                queuedGamepads.at(i).gamepad->device = devices.at(jid);
                
                // remove from list
                queuedGamepads.erase(queuedGamepads.begin() + i);
            }
        }
    }
}

void GamepadManager::disableGamepad(int jid){
    // check if ID already exists
    if(devices.at(jid) != nullptr){
        // turn off device
        devices.at(jid)->isConnected = false;
    }
}

void GamepadManager::clear(){
    // remove all reference of queued gamepads
    for(QueuedGamepad& pad : queuedGamepads){
        pad.gamepad = nullptr;
    }

    // clear the queued gamepad list
    queuedGamepads.clear();
}

void GamepadManager::setUpAutoClear(){
    // set up on exit to call the Clear()
    if(!isAutoClearSet && std::atexit(clear) == 0){
        isAutoClearSet = true; // disable calling this function again
    }
}