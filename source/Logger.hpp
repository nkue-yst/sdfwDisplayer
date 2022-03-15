/**
 * @file    Logger.hpp
 * @author  Y.Nakaue
 */

#pragma once

#include <string>
#include <vector>

/**
 * @brief  Interface class that manages log
 */
class ILogger
{
public:
    virtual ~ILogger() = default;

    /**
     * @brief  Create instance
     */
    static ILogger* create();

    /**
     * @brief  Add string to buffer
     * @param  (str)  String to add to buffer
     * @param  (win)  Target window ID
     */
    virtual void addToBuffer(std::string str, int32_t win) = 0;

    /**
     * @brief  Execute print string
     * @param  (str)  Output string
     */
    virtual void execPrint() = 0;

    /// Buffer for rendering text per window
    std::vector<std::vector<std::string>> buff_;
};

/**
 * @brief  Class that manages log
 */
class Logger : public ILogger
{
public:
    Logger();
    ~Logger() override;

    /**
     * @brief  Add string to buffer
     * @param  (str)  String to add to buffer
     * @param  (win)  Target window ID
     */
    void addToBuffer(std::string str, int32_t win);

    /**
     * @brief  Execute print string
     */
    void execPrint() override;
};
