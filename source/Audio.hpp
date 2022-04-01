/**
 * @file    Audio.hpp
 * @author  Y.Nakaue
 */

#pragma once

#include <SDL_mixer.h>

#include <filesystem>
#include <iostream>
#include <string>

/**
 * @brief  Audio file class
 */
class Audio
{
public:
    Audio()
        : data_(NULL)
    {
    }

    /**
     * @brief  Load wav data from file
     * @param  (name)  WAV file name
     * @return  Is successfuly
     */
    bool loadWAV(std::string name)
    {
        this->data_ = Mix_LoadMUS(name.c_str());

        bool success = (this->data_ != NULL);
        return success;
    }

    void play()
    {
        Mix_PlayMusic(this->data_, 1);
    }

private:
    /// Sound data
    Mix_Music* data_;
};
