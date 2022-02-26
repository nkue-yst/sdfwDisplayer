/**
 * @file    MessageReceiver.hpp
 * @author  Y.Nakaue
 */

#pragma once

#include "Command.hpp"

#include <SDL_net.h>

#include <string>
#include <vector>

/**
* @brief  Class of receiving message with TCP
*/
class sdfwMessageReceiver
{
protected:
    /**
     * @brief  Initialize variables
     */
    sdfwMessageReceiver(uint16_t port = 62491);

    /**
     * @brief  Close TCP socket if opend
     */
    ~sdfwMessageReceiver();

public:
    /**
     * @brief  Create instance
     */
    static sdfwMessageReceiver* create()
    {
        return new sdfwMessageReceiver();
    }

    /**
     * @brief  Get instance
     * @return  Instance of singleton
     */
    static sdfwMessageReceiver* get()
    {
        if (pInstance_ == nullptr)
        {
            pInstance_ = create();
        }

        return pInstance_;
    }

    /**
     * @brief  Destroy instance
     */
    static void destroy()
    {
        delete pInstance_;
        pInstance_ = nullptr;
    }

    /**
     * @brief  Waiting for message to be received
     * @return  Received message
     */
    std::string receiveMessage();

    /**
     * @brief  Open TCP socket
     * @param  port  Port num using TCP connection
     * @return  Error code (0: Success, 1: Error)
     */
    int32_t openSocket(uint16_t port = 62491);

    /**
     * @brief  Wait for TCP_Accept to complete
     */
    void acceptConnection();

private:
    /**
     * @brief  Close TCP socket
     * @param  sock  Socket to close
     */
    void closeSocket();
    
    /// Instance for singleton
    inline static sdfwMessageReceiver* pInstance_ = nullptr;

    /// Buffer for receiving message
    std::vector<struct Command> msg_buff_;

    /// Server IP address
    IPaddress ip_addr_;

    /// TCP socket
    TCPsocket sock_;

    /// Accepted TCP socket
    TCPsocket accepted_sock_;

    /// Port usnig TCP connection
    uint16_t port_;

    /// Whether TCP_Open has been executed or not
    bool is_opend_;
};
