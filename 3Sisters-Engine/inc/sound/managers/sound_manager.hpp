#pragma once

#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

// include standard libraries
#include <map>
#include <vector>

// include necessary components
#include <sound/sound_device.hpp>
#include <sound/sound_buffer.hpp>

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
        static void InitDevice();

        //* getter functions

        // get a loaded sound buffer by name
        static SoundBuffer& GetSoundBuffer(std::string name);

        // get the raw buffer from a loaded sound buffer  by name
        static unsigned int GetRawSoundBuffer(std::string name);

        //* adder/setter functions 

        /* load a sound by file path to generate the sound buffer
        *   @NOTE: The file format must be .wav
        */
        static SoundBuffer& LoadSound(std::string name, const char* filename);

        /* store a sound source reference for automatic deletion when program terminates
        * NOTE: that the Sound Source class already includes itself for automatic deletion
        */
        static void AddSoundSource(unsigned int source);

        //* remover functions

        // to remove a loaded sound 
        static void RemoveSound(std::string name);

        // to remove a sound source
        static void RemoveSoundSource(unsigned int source);
    private:
        // private static storage for sound device
        static SoundDevice* device;

        // private static storage for sound buffers as a collection
        static std::map<std::string, SoundBuffer> sounds;
        
        // private static reference storage of sound sources
        static std::vector<unsigned int> sources;
    
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