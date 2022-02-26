/**
 * @file    Main.hpp
 * @author  Y.Nakaue
 */

#pragma once

namespace sdfw
{

    /**
     * @brief  Quit system with error
     */
    void abort();

    /**
     * @brief  Output log message
     * @param  message  Output message
     */
    void outputLog(std::string message);

    /**
     * @brief  Receiving messages (in a new thread)
     */
    void threadMessageReceive();

}
