#pragma once

#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

// include standard libraries
#include <map>
#include <string>
#include <vector>
#include <climits>

// include necessary components
#include <soundSystems/types/sound_device.hpp>
#include <soundSystems/types/sound_buffer.hpp>
#include <soundSystems/sound_source.hpp>

/* A Static singleton Sound Manager class that hosts several
 function to load, retrieve, and store sounds. It also handles
 initializing the sound device to enable playing sounds. 
 Each loaded sound is also stored for future reference 
 by string reference. All functions and resources are static 
 and no public constructor is defined.  
*/
class SoundManager{
    public:
        // initializes the sound device and OpenAL
        static void InitDevice(const char* queriedDeviceName = nullptr);

        /* returns a list of all sound devices connected to the system.
        * Debug printing of the queried sounds device names is disabled by default
        */
        static std::vector<std::string> GetAllSoundDevices(bool debugPrint = false);

        //* getter functions

        // get a loaded sound buffer by name
        static SoundBuffer GetSound(std::string name);

        // get a sound source by name
        static SoundSource& GetSoundSource(std::string name);

        //* adder/setter functions 

        /* load a sound by file path to generate the sound buffer
        *   @NOTE: The file format must be .wav
        */
        static SoundBuffer LoadSound(std::string name, const char* filename);

        /* create a sound source by name.
        * Optionally give a loaded sound buffer to the sound source upon creation.
        * NOTE: Sound Manager automatically handles properly deleting sources
        */
        static SoundSource& CreateSoundSource(std::string name, SoundBuffer buffer = INT_MAX);

    private:
        // private static storage for sound device
        static SoundDevice device;

        // private static storage for sound buffers as a collection
        static std::map<std::string, SoundBuffer> sounds;
        
        // private static reference storage of sound sources as a collection
        static std::map<std::string, SoundSource> sources;

        // closes and clear the sound device, sound buffers, and sound sources
        static void Clear();

        // constructor
        SoundManager() {};

        //! Currently EXPERIMENTAL, may cause exceptions or segfaults
        // private boolean to track automatic clear()
        static bool isAutoClearSet;
        // set up automatic de-allocation of loaded resources
        static void SetUpAutoClear();
};

#endif