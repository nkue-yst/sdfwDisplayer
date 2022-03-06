/**
 * @file    MessageReceiver.hpp
 * @author  Y.Nakaue
 */

#pragma once

#include "Command.hpp"

#include <SDL_net.h>

#include <string>
#include <vector>
#include <mutex>

 /**
  * @brief Message receiver interface class
  */
class IMessageReceiver
{
public:
    virtual ~IMessageReceiver() = default;

    /**
     * @brief  Create instance
     */
    static IMessageReceiver* create();

    /**
     * @brief  Initialize socket
     */
    virtual void init() = 0;

    /**
     * @brief  Open TCP socket
     * @param  port  Port num using TCP connection
     * @return  Error code (0: Success, 1: Error)
     */
    virtual int32_t openSocket(uint16_t port = 62491) = 0;

    /**
     * @brief  Wait for TCP_Accept to complete
     */
    virtual void acceptConnection() = 0;

    /**
     * @brief  Waiting for message to be received
     */
    virtual void receiveMessage() = 0;

    /// Buffer for receiving message
    std::vector<struct Command> cmd_buff_;

    /// Mutex for command buffer
    std::mutex cmd_buff_mutex_;
};


/**
* @brief  Class of receiving message with TCP
*/
class MessageReceiver : public IMessageReceiver
{
public:
    /**
     * @brief  Initialize variables
     */
    MessageReceiver(uint16_t port = 62491);

    /**
     * @brief  Close TCP socket if opend
     */
    ~MessageReceiver() override;

    /**
     * @brief  Initialize socket
     */
    void init() override;

    /**
     * @brief  Waiting for message to be received
     */
    void receiveMessage() override;

    /**
     * @brief  Open TCP socket
     * @param  port  Port num using TCP connection
     * @return  Error code (0: Success, 1: Error)
     */
    int32_t openSocket(uint16_t port = 62491) override;

    /**
     * @brief  Wait for TCP_Accept to complete
     */
    void acceptConnection() override;

private:
    /**
     * @brief  Close TCP socket
     * @param  sock  Socket to close
     */
    void closeSocket();

    /**
     * @brief  Parse a string
     * @param  str  Target string
     * @param  delimiter  Character used to delimit string
     * @return  Split word list
     */
    static std::vector<std::string> parseMessage(const std::string& str, const char delimiter = '/');
    
    /// Instance for singleton
    inline static MessageReceiver* pInstance_ = nullptr;

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
