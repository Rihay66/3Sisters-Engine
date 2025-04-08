#pragma once

#ifndef SOUND_DEVICE_HPP
#define SOUND_DEVICE_HPP

// include standard libraries
#include <string>
// include OpenAL

#include <AL/alc.h>

/* Sound Device class for allowing sound sources to play sounds. 
This class also initializes OpenAL and opens either a 
default sound device or a given exact device name. 
*/
class SoundDevice{
    private:
        // storage of the device
        ALCdevice* ALCDevice;
        // storage of the context
        ALCcontext* ALCContext;
        // storage of sound device name
        std::string deviceName;

    public:
        // constructor
        SoundDevice(const char* queriedDevice = nullptr, const char debugMode = 'y');
        // destructor
        ~SoundDevice();

        //* getter functions

        // returns the name of the sound device in use
        std::string getSoundDeviceName();
    
};


#endif