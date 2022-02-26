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
    while (!this->quit_flag_)
    {
        sdfw::outputLog("sdfwDisplayer::run()");
    }
}

/* Select and execute function */
void sdfwDisplayer::executeCommand(std::string message)
{
    std::vector<std::string> split_message = this->parseMessage(message);
    Command command(split_message);
    std::cout << command << std::endl;

    if (command.isEqualFunc("quit"))
    {
        int temp;
        std::cin >> temp;
        sdfw::abort();

    }
}

/* Parse a string */
std::vector<std::string> sdfwDisplayer::parseMessage(const std::string& str, const char delimiter)
{
    std::vector<std::string> words;
    std::string word;

    for (int8_t c : str)
    {
        if (c == delimiter)
        {
            if (!word.empty())
            {
                words.push_back(word);
            }
            word.clear();
        }
        else
        {
            word += c;
        }
    }

    if (!word.empty())
    {
        words.push_back(word);
    }

    return words;
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
