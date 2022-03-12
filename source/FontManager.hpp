/**
 * @file    FontManager.hpp
 * @author  Y.Nakaue
 */

#pragma once

#include "SDL_ttf.h"

#include <SDL_ttf.h>

/**
 * @brief  Interface class that manages font data
 */
class IFontManager
{
public:
    virtual ~IFontManager() = default;

    /**
     * @brief  Create instance
     */
    static IFontManager* create();

    /**
     * @brief  Initialize font data (create default font)
     */
    virtual void init() = 0;

    /**
     * @brief  Get default font data
     * @return  Default font data
     */
    virtual TTF_Font* getDefaultFont() = 0;
};


/**
 * @brief  Class that manages font data
 */
class FontManager : public IFontManager
{
public:
    FontManager();
    ~FontManager() override;

    /**
     * @brief  Initialize font data (create default font)
     */
    void init() override;

    /**
     * @brief  Get default font data
     * @return  Default font data
     */
    TTF_Font* getDefaultFont() override { return this->default_font_; }

private:
    /// Default font data
    TTF_Font* default_font_;
};
