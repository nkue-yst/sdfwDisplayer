/**
 * @file    Main.hpp
 * @author  Y.Nakaue
 */

#pragma once

#include <string>

namespace sdfw
{

    /**
     * @brief  Initialize singleton instances
     */
    void init();

    /**
     * @brief  Quit the application
     */
    void quit();

    /**
     * @brief  Quit system with error
     */
    void abort(std::string message = "");

    /**
     * @brief  Output log message
     * @param  message  Output message
     */
    void outputLog(std::string message);

    /**
     * @brief  Receiving messages (in a new thread)
     */
    void threadMessageReceive();

    /**
     * @brief  Start handling event
     */
    void startHandlingEvent();

}
