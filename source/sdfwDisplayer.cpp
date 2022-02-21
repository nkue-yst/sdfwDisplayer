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
            msg = this->message_receiver_->waitReceivingMessage();
            this->func_.clear();
            this->func_.push_back(msg);
        }
        else if (msg == PARAMETER)
        {
            msg = this->message_receiver_->waitReceivingMessage();
            this->params_.push_back(msg);
        }

        /* Execute command if there are enough parameters */
        if (!this->func_.empty())
        {
            if (this->executeCommand(this->func_[0]))
            {
                this->func_.clear();
                this->params_.clear();
            }
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

/* Select and execute function */
bool sdfwDisplayer::executeCommand(uint32_t command_code)
{
    switch (command_code)
    {
    case COMMAND_OPEN_WINDOW:
        return this->execOpenWindow();
        
    default:
        break;
    }

    return false;
}

/* Execute opening window */
bool sdfwDisplayer::execOpenWindow()
{
    /* If the parameters are missing, it end up doing nothing */
    if (this->params_.size() < 2)
    {
        return false;
    }

    /* Create new window */
    this->window_ = SDL_CreateWindow("Window - 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->params_[0], this->params_[1], SDL_WINDOW_BORDERLESS);
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

    return true;
}
