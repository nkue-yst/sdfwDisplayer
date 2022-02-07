/**
 * @file    Assertion.hpp
 * @author  Y.Nakaue
 */

#pragma once
#include <string>

/**
 * @brief  Assertion class for debug and output log
 */
class SDFW_Assertion
{
public:
    /**
     * @brief  Output log message
     * @param  message  Output message
     */
    static void OutputLog(std::string message);

    /**
     * @brief  Output error message and quit system
     * @param  message  Output message
     */
    static void Abort(std::string message);
};
