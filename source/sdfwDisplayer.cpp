/**
 * @file    sdfwDisplayer.cpp
 * @author  Y.Nakaue
 */

#include "sdfwDisplayer.hpp"
#include "Command.hpp"
#include "Main.hpp"
#include "MessageReceiver.hpp"
#include "Window.hpp"

#include <SDL_net.h>

#include <iostream>
#include <string>

sdfwDisplayer::sdfwDisplayer()
    : quit_flag_(false)
{
    sdfwDisplayer::pInstance_ = this;

    /* Initialize SDL2 */
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        sdfw::abort();
    }

    /* Initialize SDL2_net */
    if (SDLNet_Init() != 0)
    {
        abort();
    }
}

sdfwDisplayer::~sdfwDisplayer()
{
    sdfw::outputLog("The application terminated successfully.");

    /* Quit SDL2 and SDL2_net */
    SDLNet_Quit();
    SDL_Quit();

    // Release memory of instance
    delete sdfwDisplayer::pInstance_;
    sdfwDisplayer::pInstance_ = nullptr;
}

/* Initialize all components */
void sdfwDisplayer::init()
{
    SDFW_DISPLAYER(MessageReceiver)->init();
    SDFW_DISPLAYER(Window)->init();
}

namespace components
{
    /**
     * @brief  Release an engine component
     */
    template<size_t N, class T>
    static void release(T&& t)
    {
        std::get<N>(t).release();

        if constexpr (N > 0)
        {
            components::release<N - 1>(t);
        }
    }

    /**
     * @brief  Release all engine components
     */
    template <class T>
    static void releaseAll(T&& t)
    {
        return components::release<std::tuple_size_v<std::remove_reference_t<T>> -1>(std::forward<T>(t));
    }
}

/* Quit and release all components */
void sdfwDisplayer::quit()
{
    components::releaseAll(this->components_);
}

void sdfwDisplayer::run()
{
    while (!this->getQuitFlag())
    {
        executeCommand();
    }
}

/* Select and execute function */
void sdfwDisplayer::executeCommand()
{
    SDFW_DISPLAYER(MessageReceiver)->cmd_buff_mutex_.lock();

    // Execute and dequeue commands in receiving buffer
    for (Command cmd : SDFW_DISPLAYER(MessageReceiver)->cmd_buff_)
    {
        // Print for debug
        std::cout << cmd << std::endl;

        // Select called function
        if (cmd.isEqualFunc("openWindow"))
        {
            this->execOpenWindow(stoi(cmd.arguments[0]), stoi(cmd.arguments[1]));
        }
        else if (cmd.isEqualFunc("quit"))
        {
            sdfw::quit();
            break;
        }

        SDFW_DISPLAYER(MessageReceiver)->cmd_buff_.erase(SDFW_DISPLAYER(MessageReceiver)->cmd_buff_.begin());
    }

    SDFW_DISPLAYER(MessageReceiver)->cmd_buff_mutex_.unlock();
}

/* Execute opening window */
void sdfwDisplayer::execOpenWindow(uint32_t width, uint32_t height)
{
    /* Create new window */
    std::string window_title = "Window - " + std::to_string(SDFW_DISPLAYER(Window)->window_list_.size());
    SDL_Window* window = SDL_CreateWindow(window_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        sdfw::abort();
    }
    SDFW_DISPLAYER(Window)->window_list_.push_back(window);

    /* Create new renderer */
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        sdfw::abort();
    }
    SDFW_DISPLAYER(Window)->renderer_list_.push_back(renderer);

    /* Clear renderer */
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}
