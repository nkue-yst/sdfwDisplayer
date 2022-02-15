/**
 * @file    MessageReceiver.cpp
 * @author  Y.Nakaue
 */

#include "MessageReceiver.hpp"
#include "Command.hpp"
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

uint16_t sdfwMessageReceiver::waitReceivingCommand()
{
    int32_t result = 0;
    uint16_t command = COMMAND_INIT;
    uint8_t buff;

    while (true)
    {
        result += SDLNet_TCP_Recv(this->accepted_sock_, &buff, sizeof(uint8_t));

        auto printb = [](uint16_t x)
        {
            putchar('0');
            putchar('b');

            uint32_t mask = (int)1 << (sizeof(x) * CHAR_BIT - 1);
            do
            {
                putchar(mask & x ? '1' : '0');
            } while (mask >>= 1);

            putchar('\n');
        };

        if (buff == COMMAND_QUIT)
            return COMMAND_QUIT;

        /***************************************
         * If data reception fails,            *
         * it will attempt to receive the data *
         * after executing the sleep process.  *
         ***************************************/
        if (result <= 0)
        {
            sdfwDisplayer::OutputLog("Waiting command");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        else if (result == 1)
        {
            command = buff;
            command <<= 8;
            printb(command);
        }
        else if (result == 2)
        {
            command += buff;
            printb(command);
            break;
        }
    }

    return command;
}

/* Waiting for parameters to be received */
std::vector<uint16_t> sdfwMessageReceiver::waitReceivingParams(uint16_t parameters_num)
{
    int32_t result;
    uint16_t buff;
    std::vector<uint16_t> return_value;

    while (true)
    {
        result = SDLNet_TCP_Recv(this->accepted_sock_, &buff, sizeof(buff));

        /***************************************
         * If data reception fails,            *
         * it will attempt to receive the data *
         * after executing the sleep process.  *
         ***************************************/
        if (result <= 0)
        {
            sdfwDisplayer::OutputLog("Waiting parameters");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        else
        {
            return_value.push_back(buff);

            if (return_value.size() == parameters_num)
                break;
        }
    }

    return return_value;
}
