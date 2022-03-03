/**
 * @file    sdfwDisplayer.cpp
 * @author  Y.Nakaue
 */

#include "sdfwDisplayer.hpp"
#include "Command.hpp"
#include "Main.hpp"
#include "MessageReceiver.hpp"

#include <SDL_net.h>

#include <iostream>

sdfwDisplayer::sdfwDisplayer()
    : window_(nullptr)
    , renderer_(nullptr)
    , quit_flag_(false)
{
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

    /* Destroy SDL component */
    SDL_DestroyWindow(this->window_);
    SDL_DestroyRenderer(this->renderer_);

    /* Quit SDL2 and SDL2_net */
    SDLNet_Quit();
    SDL_Quit();
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
    sdfwMessageReceiver::get()->cmd_buff_mutex_.lock();

    // Execute and dequeue commands in receiving buffer
    for (Command cmd : sdfwMessageReceiver::get()->cmd_buff_)
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
        }

        sdfwMessageReceiver::get()->cmd_buff_.erase(sdfwMessageReceiver::get()->cmd_buff_.begin());
    }

    sdfwMessageReceiver::get()->cmd_buff_mutex_.unlock();
}

/* Execute opening window */
void sdfwDisplayer::execOpenWindow(uint32_t width, uint32_t height)
{
    /* Create new window */
    this->window_ = SDL_CreateWindow("Window - 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_BORDERLESS);
    if (this->window_ == NULL)
    {
        sdfw::abort();
    }

    /* Create new renderer */
    this->renderer_ = SDL_CreateRenderer(this->window_, -1, SDL_RENDERER_ACCELERATED);
    if (this->renderer_ == NULL)
    {
        sdfw::abort();
    }

    /* Clear renderer */
    SDL_SetRenderDrawColor(this->renderer_, 0, 255, 0, 255);
    SDL_RenderClear(this->renderer_);
    SDL_RenderPresent(this->renderer_);
}
