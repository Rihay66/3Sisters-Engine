#include <soundSystems/managers/sound_manager.hpp>

#include <AL/alext.h>
#include <stdlib.h>
#include <sndfile.h>
#include <inttypes.h>
#include <limits.h>

#include "al.h"
#include "alc.h"
#include <cstring>
// include standard output library
#include <iostream>

// instantiate static variables
SoundDevice                             SoundManager::device;
std::map<std::string, SoundBuffer>      SoundManager::sounds;
std::map<std::string, SoundSource>       SoundManager::sources;
bool                                    SoundManager::isAutoClearSet = false;

void SoundManager::InitDevice(const char* queriedDeviceName){
    // set up automatic clear()
    SetUpAutoClear();

    // init sound device
    
    // check if sound device is already initialized
    if(device.ALCDevice != nullptr || device.ALCContext != nullptr)
        return; // stop function
    
    // get specified name sound device
    device.ALCDevice = alcOpenDevice(queriedDeviceName);

    if(!device.ALCDevice){
        throw("ERROR: failed to get sound device");
    }

    // create context 
    device.ALCContext = alcCreateContext(device.ALCDevice, nullptr);

    if(!device.ALCContext){
        throw("ERROR: failed to set sound context");
    }

    if(!alcMakeContextCurrent(device.ALCContext)){ // make context current
        throw("ERROR: to make context current");
    }

    // Print out any errors encountered
    if(alcGetError(device.ALCDevice) != AL_NO_ERROR){
        std::cout << "ERROR: OpenAL encountered an error: " << alcGetString(device.ALCDevice, ALC_DEVICE_SPECIFIER) << "\n";
    }
}

std::vector<std::string> SoundManager::GetAllSoundDevices(bool debugPrint){
    // create local storage of the device names found
    std::vector<std::string> devices;

    // check if the enumeration extension is supported 
    if(!alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT")){
        std::cout << "ERROR: enumeration of devices is not supported!\n";
        return devices;
    }

    // grab the devices
    const ALchar* deviceList = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
    while(strlen(deviceList) > 0){
        if(debugPrint){
            std::cout << "Found Sound device: " << deviceList << "\n";
        }

        devices.push_back(deviceList);
        deviceList += strlen(deviceList) + 1;
    }

    return devices;
}

SoundBuffer SoundManager::GetSound(std::string name){
    return sounds[name];
}

SoundSource& SoundManager::GetSoundSource(std::string name){
    return sources[name];
}

SoundBuffer SoundManager::LoadSound(std::string name, const char* filename){
    // set up automatic clear()
    SetUpAutoClear();
    
    // set up device
    InitDevice();
    
    // create local vars
    ALenum err, format;
    SNDFILE* sndfile;
    SF_INFO sfinfo;
    short* membuf;
    sf_count_t num_frames;
    ALsizei num_bytes;

    // open the audio file and check if it's usable
    sndfile = sf_open(filename, SFM_READ, &sfinfo);
    if(!sndfile){
        fprintf(stderr, "ERROR: could not open audio in %s: %s\n", filename, sf_strerror(sndfile));
        return 0;
    }

    if(sfinfo.frames < 1 || sfinfo.frames > (sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels){
        fprintf(stderr, "ERROR: bas sample count in %s (%" PRId64 ")\n", filename, sfinfo.frames);
        sf_close(sndfile);
        return 0;
    }

    // get the sound format, and figure out the OpenAL format
    format = AL_NONE;
    if(sfinfo.channels == 1){
        format = AL_FORMAT_MONO16;
    }else if(sfinfo.channels == 2){
        format = AL_FORMAT_STEREO16;
    }else if(sfinfo.channels == 3){
        if(sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT){
            format = AL_FORMAT_BFORMAT2D_16;
        }
    }else if(sfinfo.channels == 4){
        if(sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT){
            format = AL_FORMAT_BFORMAT3D_16;
        }
    }

    // check if format has been set
    if(!format){
        fprintf(stderr, "ERROR: unsupported channel count: %d\n", sfinfo.channels);
        sf_close(sndfile);
        return 0;
    }

    // decode the whole file into a buffer
    membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

    num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
    if(num_frames < 1){
        free(membuf);
        sf_close(sndfile);
        fprintf(stderr, "ERROR: failed to read samples in %s (%" PRId64 ")\n", filename, num_frames);
        return 0;
    }

    num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

    // buffer the audio data into a new buffer object, then free the data and close the file
    unsigned int buffer = 0;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

    free(membuf);
    sf_close(sndfile);

    // check for any OpenAL errors
    err = alGetError();
    if(err != AL_NO_ERROR){
        fprintf(stderr, "ERROR: Sound Buffer Sound Loading OpenAL Error: %s\n", alGetString(err));
        if(buffer && alIsBuffer(buffer)){
            alDeleteBuffers(1, &buffer);
        }
        return INT_MAX;
    }
    
    // add buffer to the list
    sounds[name] = buffer;
    
    return sounds[name];
}

SoundSource& SoundManager::CreateSoundSource(std::string name, SoundBuffer buffer){
    // create local sound source
    SoundSource source;
    // generate source
    source.generateSource();
    // set buffer that is passed down
    source.setBuffer(buffer);
    // store source and return it
    sources[name] = source;
    return sources[name];
}

void SoundManager::Clear(){
    // try to properly close OpenAL and catch any errors
    try{
        // clear all sound sources
        for(auto& it : sources){
            // check if it's a valid source
            if(alIsSource(it.second.getSource())){
                // remove source
                alDeleteSources(1, &it.second.getSource());
            }else{
                std::cout << "ERROR: Failed to remove a sound source\n";
            }
        }

        // clear all sound buffers
        for(auto& iter : sounds){
            if(alIsBuffer(iter.second)){
                // remove buffer
                alDeleteBuffers(1, &iter.second);
            }else{
                std::cout << "ERROR: Failed to identify sound buffer\n";
            }
        }

        // stop context from receiving updates
        alcSuspendContext(device.ALCContext);

        // try to close device
        alcMakeContextCurrent(nullptr);

        // destroy the context
        alcDestroyContext(device.ALCContext);

        // close the sound device
        alcCloseDevice(device.ALCDevice);
        
    }catch(std::exception& e){
        // print out error
        std::cout << "ERROR: OpenAL caused an Exception: " << e.what() << "\n";

        // check for any OpenAL errors
        ALenum err = alGetError();
        if(err != AL_NO_ERROR){
            fprintf(stderr, "ERROR: OpenAL Error: %s\n", alGetString(err));
        }
    }
}

void SoundManager::SetUpAutoClear(){
    // set up on exit to call the Clear()
    if(!isAutoClearSet && std::atexit(Clear) == 0){
        isAutoClearSet = true; // disable calling this function again
    }
}