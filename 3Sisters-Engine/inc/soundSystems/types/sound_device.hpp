#pragma once

#ifndef SOUND_DEVICE_HPP
#define SOUND_DEVICE_HPP

// include OpenAL
#include <AL/alc.h>

/* Sound Device data structure for containing the OpenAL soft 
 context and reference of the audio device.
*/
struct SoundDevice{
    // storage of the device
    ALCdevice* ALCDevice = nullptr;
    // storage of the context
    ALCcontext* ALCContext = nullptr;
};


#endif