#include <input/sisters_glfw_gamepad.hpp>

// define globals
std::vector<GLFW::QueuedGamepad> GLFW::g_QueuedGamepads;
std::array<std::shared_ptr<GLFW::ControllerDevice>, 15> GLFW::g_controllerDevices;

int GLFW::getGamepadAmount(){
    // init var to contain actual amount of devices available
    int amount = 0;

    for(int i = 0; i < GLFW_JOYSTICK_LAST; i++){
        if(g_controllerDevices.at(i) != nullptr){
            amount++;
        }else{
            break; // stop loop
        }
    }

    return amount;
}

void GLFW::setGamepad(Gamepad& gamepad, int index){
    // check index is within range and if reference already exists
    if(index < GLFW_JOYSTICK_LAST && g_controllerDevices.at(index) != nullptr){
        // set the reference
        gamepad.device = g_controllerDevices.at(index);

        return; // stop function
    }else if(index < GLFW_JOYSTICK_LAST && g_controllerDevices.at(index) == nullptr){
        // queue the given gamepad
        
        QueuedGamepad qp;
        qp.index = index;
        qp.gamepad = &gamepad;

        // add to queue
        g_QueuedGamepads.push_back(qp);
    }
}

void GLFW::enableGamepad(int jid){
    // check if ID already exists
    for(int i = 0; i < g_controllerDevices.size(); i++){
        if(g_controllerDevices.at(i) != nullptr && g_controllerDevices.at(i)->ID == jid){
            // gamepad exists, then set the connection to be true
            g_controllerDevices.at(i)->isConnected = true;

            // stop function
            return;
        }
    }

    // if ID doesn't exist then create one
    if(jid < GLFW_JOYSTICK_LAST){
        // create a new gamepad
        g_controllerDevices.at(jid) = std::make_shared<ControllerDevice>();
        // set the ID
        g_controllerDevices.at(jid)->ID = jid;
        // then set the connection to be true
        g_controllerDevices.at(jid)->isConnected = true;

        // loop in reverse through queued up gamepads that need to be set
        for(int i = 0; i < g_QueuedGamepads.size(); i++){
            if(g_QueuedGamepads.at(i).index == jid && g_QueuedGamepads.at(i).gamepad != nullptr){
                // set the gamepad
                g_QueuedGamepads.at(i).gamepad->device = g_controllerDevices.at(jid);
                
                // remove from list
                g_QueuedGamepads.erase(g_QueuedGamepads.begin() + i);
            }
        }
    }
}

void GLFW::disableGamepad(int jid){
    // check if ID already exists
    if(g_controllerDevices.at(jid) != nullptr){
        // turn off device
        g_controllerDevices.at(jid)->isConnected = false;
    }
}


bool GLFW::getButtonInput(GLFW::Gamepad &pad, int key){
    // check if gamepad is set and connected
    if(pad.device != nullptr && pad.device->isConnected){
        // check state of given key
        if(pad.device->state.buttons[key] == GLFW_PRESS){
            return true;
        }
    }

    return false;
}

float GLFW::getAxisRawInput(GLFW::Gamepad &pad, int key, float deadzone){
    // check that the pad is set and the state is valid
    if(pad.device != nullptr && pad.device->isConnected){
        //check for positive and negative values above deadzone
        if(pad.device->state.axes[key] < -deadzone){
            return pad.device->state.axes[key];
        }else if(pad.device->state.axes[key] > deadzone){
            return pad.device->state.axes[key];
        }
    }

    // return 0 for no input
    return 0.0f;
}

float GLFW::getAxisInput(GLFW::Gamepad& pad, int key, float deadzone){
    // check the that pad is set and the state is valid
    if(pad.device != nullptr && pad.device->isConnected){
        //check for positive and negative values above deadzone
        if(pad.device->state.axes[key] < -deadzone){
            return -1.0f;
        }else if(pad.device->state.axes[key] > deadzone){
            return 1.0f;
        }
    }

    // return 0 for no input
    return 0.0f;
}