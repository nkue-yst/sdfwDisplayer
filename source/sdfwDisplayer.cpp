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
        command_code = this->message_receiver_->waitReceivingData();

        switch (command_code)
        {
        case COMMAND_OPEN_WINDOW:
            std::cout << "Open window" << std::endl;
            break;

        default:
            break;
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
