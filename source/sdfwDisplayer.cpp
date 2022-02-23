/**
 * @file    sdfwDisplayer.cpp
 * @author  Y.Nakaue
 */

#include "sdfwDisplayer.hpp"
#include "Command.hpp"
#include "MessageReceiver.hpp"

#include <SDL_net.h>

#include <iostream>

sdfwDisplayer::sdfwDisplayer()
    : message_receiver_(nullptr)
    , window_(nullptr)
    , renderer_(nullptr)
    , quit_flag_(false)
{
    /* Initialize SDL2 */
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        this->Abort("Failed to initialize SDL2");
    }

    /* Initialize SDL2_net */
    if (SDLNet_Init() != 0)
    {
        this->Abort("Failed to initialize SDL2_net");
    }
}

sdfwDisplayer::~sdfwDisplayer()
{
    this->OutputLog("The application terminated successfully.");

    /* Destroy SDL component */
    SDL_DestroyWindow(this->window_);
    SDL_DestroyRenderer(this->renderer_);

    /* Quit SDL2 and SDL2_net */
    SDLNet_Quit();
    SDL_Quit();
}

void sdfwDisplayer::init(uint16_t port)
{
    this->message_receiver_ = std::unique_ptr<sdfwMessageReceiver>(new sdfwMessageReceiver());

    if (this->message_receiver_->openSocket(port) != 0)
    {
        this->Abort("Failed to open socket");
    }
}

void sdfwDisplayer::run()
{
    /* Start to wait for a connection from a client program */
    this->message_receiver_->acceptConnection();

    while (!this->quit_flag_)
    {
        this->executeCommand(this->message_receiver_->waitReceivingMessage());
    }
}

void sdfwDisplayer::OutputLog(std::string message)
{
    std::cout << "sdfw_Log: " << message << std::endl;
}

void sdfwDisplayer::Abort(std::string message)
{
    std::cout << "sdfw_Log (Abort): " << message << std::endl;

    this->message_receiver_->closeSocket();

    std::exit(1);
}

/* Select and execute function */
void sdfwDisplayer::executeCommand(std::string message)
{
    std::vector<std::string> split_message = this->parseMessage(message);
    Command command(split_message);
    std::cout << command;

    if (command.isEqualFunc("quit"))
    {
        this->Abort("Successfully quit");
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
        sdfwDisplayer::Abort("Failed to open new window");
    }

    /* Create new renderer */
    this->renderer_ = SDL_CreateRenderer(this->window_, -1, SDL_RENDERER_ACCELERATED);
    if (this->renderer_ == NULL)
    {
        sdfwDisplayer::Abort("Failed to create renderer");
    }

    /* Clear renderer */
    SDL_SetRenderDrawColor(this->renderer_, 0, 255, 0, 255);
    SDL_RenderClear(this->renderer_);
    SDL_RenderPresent(this->renderer_);
}
