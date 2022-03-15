/**
 * @file    Logger.hpp
 * @author  Y.Nakaue
 */

#include "Logger.hpp"
#include "FontManager.hpp"
#include "sdfwDisplayer.hpp"
#include "Window.hpp"

#include <SDL.h>
#include <SDL_ttf.h>

ILogger* ILogger::create()
{
    return new Logger();
}

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::addToBuffer(std::string str, int32_t win)
{
    int32_t win_num = this->buff_.size() - 1;

    while (win_num < win)
    {
        this->buff_.push_back(std::vector<std::string>());
        win_num = this->buff_.size() - 1;
    }

    this->buff_.at(win).push_back(str);
}

void Logger::execPrint()
{
    SDL_Color color = { 255, 255, 255, 255 };

    size_t win_id = 0;
    for (std::vector<std::string> str_list : this->buff_)
    {
        uint32_t offset = 0;

        for (std::string str : str_list)
        {
            SDL_Surface* surface = TTF_RenderText_Blended(SDFW_DISPLAYER(FontManager)->getDefaultFont(), str.c_str(), color);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(SDFW_DISPLAYER(WindowManager)->window_list_.at(win_id)->renderer_, surface);
            SDL_Rect dst = { 0, offset, surface->w, surface->h };
            offset += surface->h;

            SDL_RenderCopy(SDFW_DISPLAYER(WindowManager)->window_list_.at(win_id)->renderer_, texture, NULL, &dst);
            SDL_FreeSurface(surface);
        }

        win_id++;
    }
}
