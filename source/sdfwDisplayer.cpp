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
    , is_waiting_param_(false)
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

    uint16_t command_code = COMMAND_INIT;

    /***********************************
     * Receive and execute command     *
     * until the QUIT command is read. *
     ***********************************/
    while (command_code != COMMAND_QUIT)
    {
        // Read command code or parameters
        command_code = this->message_receiver_->waitReceivingCommand();

        /* Execute command if the most upper bit is '1' */
        if ((command_code >> 15) & 1)
        {
            switch (command_code)
            {
            case COMMAND_OPEN_WINDOW:
                execOpenWindow();
                break;

            default:
                break;
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

/* Execute opening window */
void sdfwDisplayer::execOpenWindow()
{
    std::cout << "execOpenWindow()" << std::endl;

    /* If the window exist, finish without doing anything */
    if (this->window_ != NULL)
    {
        return;
    }

    /* Wait for parameters */
    std::vector<uint16_t> params;
    params = this->message_receiver_->waitReceivingParams(2);

    /* Create new window */
    this->window_ = SDL_CreateWindow("Window - 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, params[0], params[1], SDL_WINDOW_BORDERLESS);
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
