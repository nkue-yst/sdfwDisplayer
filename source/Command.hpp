/**
 * @file    Command.hpp
 * @author  Y.Nakaue
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>

struct Command
{
    Command(std::vector<std::string> word_list)
    {
        this->function = word_list.front();
        word_list.erase(word_list.begin());

        for (std::string word : word_list)
        {
            arguments.push_back(word);
        }
    }

    /**
     * @brief  Compare the specified string with the function
     */
    bool isEqualFunc(const char* command)
    {
        return this->function == command;
    }

    /**
     * @brief  Overload for output stream
     */
    friend std::ostream& operator<<(std::ostream& stream, const Command& command)
    {
        // Output function name
        stream << "Function: " << command.function << std::endl;

        // Output arguments
        uint16_t i = 0;
        for (std::string arg : command.arguments)
        {
            stream << "Arg[" << i << "]: " << arg << std::endl;
            i++;
        }
        stream << std::endl;

        // Return received stream
        return stream;
    }

    /// Function
    std::string function;

    /// Arguments
    std::vector<std::string> arguments;
};
