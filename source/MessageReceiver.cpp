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

IMessageReceiver* IMessageReceiver::create()
{
    return new MessageReceiver();
}

MessageReceiver::MessageReceiver(uint16_t port)
    : port_(port)
    , is_opend_(false)
{
    this->ip_addr_ = IPaddress();
    this->sock_ = TCPsocket();
    this->accepted_sock_ = TCPsocket();
}

MessageReceiver::~MessageReceiver()
{
    this->cmd_buff_mutex_.lock();
    this->closeSocket();
    this->cmd_buff_mutex_.unlock();
}

void MessageReceiver::init()
{
}

void MessageReceiver::receiveMessage()
{
    char buff;
    std::string str;
    struct Command command;

    sdfw::outputLog("Start to receive messages");

    char sync_msg = '0';
    SDLNet_TCP_Send(this->accepted_sock_, &sync_msg, sizeof(sync_msg));

    // Message receiving loop
    while (true)
    {
        // If fail to receive a message, terminate the receiving loop
        if (SDLNet_TCP_Recv(this->accepted_sock_, &buff, sizeof(char)) <= 0)
            break;

        // Add command to buffer when a terminating character is received
        if (buff == '\0')
        {
            command = Command(this->parseMessage(str));

            this->cmd_buff_mutex_.lock();
            this->cmd_buff_.push_back(command);
            this->cmd_buff_mutex_.unlock();

            str.clear();

            SDLNet_TCP_Send(this->accepted_sock_, &sync_msg, sizeof(sync_msg));
        }
        else
        {
            str += buff;
        }
    }
}

/* Parse a string */
std::vector<std::string> MessageReceiver::parseMessage(const std::string& str, const char delimiter)
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

int32_t MessageReceiver::openSocket(uint16_t port)
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

void MessageReceiver::closeSocket()
{
    if (this->is_opend_)
    {
        SDLNet_TCP_Close(this->sock_);
        this->is_opend_ = false;
    }
}

void MessageReceiver::acceptConnection()
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
