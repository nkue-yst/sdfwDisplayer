/**
 * @file    FontManager.cpp
 * @author  Y.Nakaue
 */

#include "FontManager.hpp"
#include "Main.hpp"

#include <iostream>

IFontManager* IFontManager::create()
{
    return new FontManager();
}

FontManager::FontManager()
    : default_font_(nullptr)
{
}

FontManager::~FontManager()
{
    TTF_Quit();
}

void FontManager::init()
{
    TTF_Init();

    default_font_ = TTF_OpenFont("../Font/NotoSansJP-Regular.otf", 16);
    if (!this->default_font_)
    {
        std::cout << TTF_GetError() << std::endl;
        sdfw::abort();
    }
}
