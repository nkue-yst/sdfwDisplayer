/**
 * @file    Window.cpp
 * @author  Y.Nakaue
 */

#include "Window.hpp"
#include <iostream>

IWindow* IWindow::create()
{
    return new Window();
}

Window::Window()
{
}

Window::~Window()
{
    // Destroy windows
    for (SDL_Window* win : this->window_list_)
    {
        SDL_DestroyWindow(win);
    }

    // Destroy renderer
    for (SDL_Renderer* renderer : this->renderer_list_)
    {
        SDL_DestroyRenderer(renderer);
    }
}

void Window::init()
{
}
