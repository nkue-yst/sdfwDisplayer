/**
 * @file    EventHandler.hpp
 * @author  Y.Nakaue
 */

#pragma once

#include <SDL_net.h>

#include <mutex>
#include <vector>

/**
 * @brief  Event handling class interface
 */
class IEventHandler
{
public:
    virtual ~IEventHandler() = default;

    /**
     * @brief  Create instance
     */
    static IEventHandler* create();

    /**
     * @brief  Initialize socket
     */
    virtual void init() = 0;

    /**
     * @brief  Open TCP socket
     * @param  (port)  Port num using TCP connection
     * @return Error code (0: Success, 1: Error)
     */
    virtual int32_t openSocket(uint16_t port = 62492) = 0;

    /**
     * @brief  Wait for TCP_Accept to complete
     */
    virtual void acceptConnection() = 0;

    /**
     * @brief  Send message with sync msg
     */
    virtual void sendMessage(const char* msg) = 0;

protected:
    char sync_msg_;
};


/**
 * @brief  Class of handling event
 */
class EventHandler : public IEventHandler
{
public:
    /**
     * @brief  Initialize variables
     */
    EventHandler(uint16_t port = 62492);

    /**
     * @brief  Close TCP socket if opend
     */
    ~EventHandler() override;

    /**
     * @brief  Initialize socket
     */
    void init() override;

    /**
     * @brief  Open TCP socket
     * @param  (port)  Port num using TCP connection
     * @return Error code (0: Success, 1: Error)
     */
    int32_t openSocket(uint16_t port = 62492) override;

    /**
     * @brief  Wait for TCP_Accept to complete
     */
    void acceptConnection() override;

    /**
     * @brief  Send message with sync msg
     */
    void sendMessage(const char* msg) override;

private:
    /**
     * @brief  Close TCP socket
     * @param  sock  Socket to close
     */
    void closeSocket();

    /// Server IP address
    IPaddress ip_addr_;

    /// TCP socket
    TCPsocket sock_;

    /// Accepted TCP socket
    TCPsocket accepted_sock_;

    /// Port using TCP connection
    uint16_t port_;

    /// Whether TCP_Open has been executed or not
    bool is_opened_;
};
