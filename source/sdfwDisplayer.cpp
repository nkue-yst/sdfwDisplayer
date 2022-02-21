/**
 * @file    sdfwDisplayer.cpp
 * @author  Y.Nakaue
 */

#include "Command.hpp"
#include "MessageReceiver.hpp"
#include "sdfwDisplayer.hpp"

#include <SDL_net.h>

#include <iostream>

sdfwDisplayer::sdfwDisplayer()
    : message_receiver_(nullptr)
    , window_(nullptr)
    , renderer_(nullptr)
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

    uint32_t msg = COMMAND_INIT;

    /***********************************
     * Receive and execute command     *
     * until the QUIT command is read. *
     ***********************************/
    while (msg != COMMAND_QUIT)
    {
        // Read command code or parameters
        msg = this->message_receiver_->waitReceivingMessage();

        if (msg == COMMAND)
        {
            if (!func_.empty())
            {
                switch (func_[0])
                {
                case COMMAND_OPEN_WINDOW:
                    this->execOpenWindow();
                    break;

                default:
                    break;
                }
            }
            func_.clear();
            params_.clear();

            msg = this->message_receiver_->waitReceivingMessage();
            this->func_.push_back(msg);
        }
        else if (msg == PARAMETER)
        {
            this->params_.push_back(this->message_receiver_->waitReceivingMessage());
        }
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

/* Execute opening window */
void sdfwDisplayer::execOpenWindow()
{
    /* If the window exist, finish without doing anything */
    if (this->window_ != NULL)
    {
        return;
    }

    /* Create new window */
    this->window_ = SDL_CreateWindow("Window - 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->params_[0], this->params_[1], SDL_WINDOW_BORDERLESS);
    if (this->window_ == NULL)
    {
        sdfwDisplayer::Abort("Failed to open new window");
    }

    std::cout << this->params_[0] << ", " << this->params_[1] << std::endl;

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
