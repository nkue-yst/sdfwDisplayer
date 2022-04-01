/**
 * @file    AssetHandler.cpp
 * @author  Y.Nakaue
 */

#include "AssetHandler.hpp"
#include "Audio.hpp"

#include <iostream>

IAssetHandler* IAssetHandler::create()
{
    return new AssetHandler();
}

AssetHandler::~AssetHandler()
{
    for (auto iter : this->audio_assets_)
    {
    }
}

class Audio* AssetHandler::getAudioAsset(const std::string& name)
{
    Audio* audio = nullptr;

    auto iter = this->audio_assets_.find(name);
    if (iter != this->audio_assets_.end())
    {
        audio = iter->second;
    }
    else
    {
        audio = new Audio();
        if (audio->loadWAV(name))
        {
            this->audio_assets_.emplace(name, audio);
        }
        else
        {
            std::cout << "Failed to load audio asset" << std::endl;
            delete audio;
            audio = nullptr;
        }
    }

    return audio;
}
