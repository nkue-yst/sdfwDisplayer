/**
 * @file    MessageReceiver.cpp
 * @author  Y.Nakaue
 */

#include "MessageReceiver.hpp"
#include "sdfwDisplayer.hpp"

#include <chrono>
#include <iostream>
#include <thread>

sdfwMessageReceiver::sdfwMessageReceiver()
    : port_(0)
    , is_opend_(false)
{
    this->ip_addr_ = IPaddress();
    this->sock_ = TCPsocket();
    this->accepted_sock_ = TCPsocket();
}

sdfwMessageReceiver::~sdfwMessageReceiver()
{
    this->closeSocket();
}

int32_t sdfwMessageReceiver::openSocket(uint16_t port)
{
    /* Create info of server ip */
    if (SDLNet_ResolveHost(&this->ip_addr_, NULL, port) == -1)
    {
        return 1;
    }
    this->port_ = port;

    /* Open TCP socket */
    this->sock_ = SDLNet_TCP_Open(&this->ip_addr_);
    if (!this->sock_)
    {
        return 1;
    }
    this->is_opend_ = true;

    return 0;
}

void sdfwMessageReceiver::closeSocket()
{
    if (this->is_opend_)
    {
        SDLNet_TCP_Close(this->sock_);
        this->is_opend_ = false;
    }
}

void sdfwMessageReceiver::acceptConnection()
{
    /* Attempt to connect until TCP_Accept is complete */
    while (true)
    {
        this->accepted_sock_ = SDLNet_TCP_Accept(this->sock_);

        if (this->accepted_sock_)
            break;

        sdfwDisplayer::OutputLog("Connection not complete.");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    sdfwDisplayer::OutputLog("Connection complete.");
}

uint16_t sdfwMessageReceiver::waitReceivingData()
{
    int32_t result;
    uint16_t buff;

    while (true)
    {
        result = SDLNet_TCP_Recv(this->accepted_sock_, &buff, sizeof(buff));

        /***************************************
         * If data reception fails,            *
         * it will attempt to receive the data *
         * after executing the sleep process.  *
         ***************************************/
        if (result <= 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        else
            break;
    }

    return buff;
}
