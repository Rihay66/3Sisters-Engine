#pragma once

#ifndef SOUND_SOURCE_HPP
#define SOUND_SOURCE_HPP

// include GLM
#include <glm/glm.hpp>
// include OpenAL
#include <AL/al.h>

/* Sound Source Class allows for a sound that can be played, 
stopped, and looped and adjusted to meet a specific need.
The sound must be a given buffer, and can only play one 
sound at a time.
*/
class SoundSource{
    protected:
        // storage of the OpenAL source
        unsigned int source;
        // storage of the pitch of the sound
        float pitch = 1.0f;
        // storage of the gain of the sound, aka volume
        float gain = 1.0f;
        // storage of the source position in 3D space
        glm::vec3 position = glm::vec3(0.0f);
        // storage of the source velocity in 3D space
        glm::vec3 velocity = glm::vec3(0.0f);
        // storage of the sound loop parameter
        bool loopSound = false;
        // storage of a given sound buffer
        unsigned int buffer = 0;

    public:
        //* setter functions

        // sets the pitch of the source, takes in a vlaue within 0.0f to 1.0f
        void setPitch(float p);

        // sets the gain of the source, takes in a vlaue within 0.0f to 1.0f
        void setGain(float g);

        // adds and sets the sound buffer of the source
        void setBuffer(unsigned int buff);

        // sets the loopSound parameter to make the sound be looped automatically when played once
        void setLoopSound(bool l);

        //* getter functions

        // returns the sound buffer that is set to this sound source
        unsigned int getBuffer();

        //* helper functions

        // plays the given sound, also sets the given buffer to be current
        void play(const unsigned int sound, bool playOnce = false);
        
        // plays a pre-given sound
        void play(bool playOnce = false);

        // stops current sound
        void stop();

        // pauses current sound and restarts the sound to the beginning
        void restartSound();

        // constructor
        SoundSource();
        // decontructor
        ~SoundSource();
};

#endif