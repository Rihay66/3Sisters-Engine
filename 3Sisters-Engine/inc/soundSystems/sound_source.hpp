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
        /* generate sound source using OpenAL.
        * Requires a OpenAL device and context to be initialized
        * Warning: If not using SoundManager, you're responsinble for properly deleting the generate source
        */ 
        unsigned int& generateSource();

        //* setter functions

        // sets the pitch of the source, takes in a value within 0.0f to 1.0f
        void setPitch(float p);

        // sets the gain of the source, takes in a value within 0.0f to 1.0f
        void setGain(float g);

        // sets the position of the source in a 3D space
        void setPosition3D(glm::vec3 pos);

        // sets the position of the source in a 2D space
        void setPosition2D(glm::vec2 pos);

        // sets the velocity of the source in a 3D space
        void setVelocity3D(glm::vec3 velocity);

        // adds and sets the sound buffer of the source
        void setBuffer(unsigned int buff);

        // sets the loopSound parameter to make the sound be looped automatically when played once
        void setLoopSound(bool l);

        //* getter functions

        // returns the sound buffer that is set to this sound source
        unsigned int getBuffer();

        // returns the sound source ID
        unsigned int& getSource();

        //* helper functions

        // plays the given sound, also sets the given buffer to be current
        void play(const unsigned int sound, bool playAgainWhenFinished = false);
        
        // plays a pre-given sound
        void play(bool playAgainWhenFinished = false);

        // stops current sound
        void stop();

        // pauses current sound and restarts the sound to the beginning
        void restartSound();
};

#endif