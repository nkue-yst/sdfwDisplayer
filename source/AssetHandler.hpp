/**
 * @file    AssetHandler.hpp
 * @author  Y.Nakaue
 */

#pragma once

#include <string>
#include <unordered_map>

/**
 * @brief  Asset handling class interface
 */
class IAssetHandler
{
public:
    virtual ~IAssetHandler() = default;

    /**
     * @brief  Create instance
     */
    static IAssetHandler* create();

    /**
     * @brief  Load audio asset from file
     */
    virtual class Audio* getAudioAsset(const std::string& name) = 0;
};

/**
 * @brief  Class of handling asset
 */
class AssetHandler : public IAssetHandler
{
public:
    AssetHandler() {};
    ~AssetHandler() override;

    /**
     * @brief  Load audio asset from file
     */
    class Audio* getAudioAsset(const std::string& name) override;

    /// Audio asset list with file name
    std::unordered_map<std::string, class Audio*> audio_assets_;
};
