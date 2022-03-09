/**
 * @file    Window.cpp
 * @author  Y.Nakaue
 */

#include "Window.hpp"
#include <iostream>

IWindowManager* IWindowManager::create()
{
    return new WindowManager();
}

WindowManager::WindowManager()
{
}

WindowManager::~WindowManager()
{
    // Destroy windows
    for (Window* window : this->window_list_)
    {
        if (window->window_)
            SDL_DestroyWindow(window->window_);

        if (window->renderer_)
            SDL_DestroyRenderer(window->renderer_);

        delete window;
    }
}

void WindowManager::init()
{
}
