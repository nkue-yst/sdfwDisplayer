/**
 * @file    MessageReceiver.hpp
 * @author  Y.Nakaue
 */

#pragma once
#include <SDL_net.h>

/**
* @brief  Class of receiving message with TCP
*/
class sdfwMessageReceiver
{
public:
    /**
     * @brief  Initialize variables
     */
    sdfwMessageReceiver();

    /**
     * @brief  Close TCP socket if opend
     */
    ~sdfwMessageReceiver();

    /**
     * @brief  Open TCP socket
     * @param  port  Port num using TCP connection
     * @return  Error code (0: Success, 1: Error)
     */
    int32_t openSocket(uint16_t port);

    /**
     * @brief  Close TCP socket
     * @param  sock  Socket to close
     */
    void closeSocket();

    /**
     * @brief  Wait for TCP_Accept to complete
     */
    void acceptConnection();

    /**
     * @brief  Waiting for data to be received
     * @return  Received data
     */
    uint16_t waitReceivingData();

private:
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
