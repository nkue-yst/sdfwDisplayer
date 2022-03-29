/**
 * @file    EventHandler.cpp
 * @author  Y.Nakaue
 */

#include "EventHandler.hpp"
#include "sdfwDisplayer.hpp"
#include "Main.hpp"

IEventHandler* IEventHandler::create()
{
    return new EventHandler();
}

EventHandler::EventHandler(uint16_t port)
    : port_(port)
    , is_opened_(false)
{
    this->ip_addr_ = IPaddress();
    this->sock_ = TCPsocket();
    this->accepted_sock_ = TCPsocket();
}

EventHandler::~EventHandler()
{
    this->closeSocket();
}

void EventHandler::init()
{
}

int32_t EventHandler::openSocket(uint16_t port)
{
    /* Create info of server IP */
    if (SDLNet_ResolveHost(&this->ip_addr_, NULL, port) == -1)
    {
        return 1;
    }
    this->port_ = port;

    /* Try TCP socket open until success */
    while (!this->sock_)
    {
        this->sock_ = SDLNet_TCP_Open(&this->ip_addr_);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    this->is_opened_ = true;

    return 0;
}

void EventHandler::acceptConnection()
{
    /* Attempt to connect until TCP_Accept is complete */
    while (true)
    {
        this->accepted_sock_ = SDLNet_TCP_Accept(this->sock_);

        // If apccepted_sock_ is not NULL, successfully connected.
        if (this->accepted_sock_)
            break;

        sdfw::outputLog("Connection for event not complete.");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    sdfw::outputLog("Connection for event complete.");
}

void EventHandler::sendMessage(const char* msg)
{
    SDLNet_TCP_Recv(this->accepted_sock_, &this->sync_msg_, sizeof(char));
    SDLNet_TCP_Send(this->accepted_sock_, msg, static_cast<int32_t>(strlen(msg) + 1));
}

void EventHandler::closeSocket()
{
    if (this->is_opened_)
    {
        SDLNet_TCP_Close(this->sock_);
        this->is_opened_ = false;
    }
}
