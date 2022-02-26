/**
 * @file    MessageReceiver.cpp
 * @author  Y.Nakaue
 */

#include "MessageReceiver.hpp"
#include "Command.hpp"
#include "sdfwDisplayer.hpp"
#include "Main.hpp"

#include <chrono>
#include <iostream>
#include <thread>

sdfwMessageReceiver::sdfwMessageReceiver(uint16_t port)
    : port_(port)
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

std::string sdfwMessageReceiver::receiveMessage()
{
    int32_t result = 0;
    std::string str;
    char buff;

    while (true)
        sdfw::outputLog("sdfwMessagereceiver::receiveMessage()");
/*
    while (true)
    {
        result = SDLNet_TCP_Recv(this->accepted_sock_, &buff, sizeof(char));

        if (buff == '\0')
            break;
        else
            str += buff;
    }
*/

    return str;
}

int32_t sdfwMessageReceiver::openSocket(uint16_t port)
{
    /* Create info of server ip */
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

        // If apccepted_sock_ is not NULL, successfully connected.
        if (this->accepted_sock_)
            break;

        sdfw::outputLog("Connection not complete.");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    sdfw::outputLog("Connection complete.");
}
