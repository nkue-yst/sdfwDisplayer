/**
 * @file    Execute.cpp
 * @author  Y.Nakaue
 */

#include <SDL.h>

#include <iostream>

#define COMMAND_QUIT 0x0000
#define COMMAND_INIT 0xFFFF
#define COMMAND_EXEC 0xFFFF

#define COMMAND_OPEN_WINDOW 0x0001

/**
 * @brief  Command set
 */
enum class ECommand : uint8_t
{
    OPEN_WINDOW,
};


/**
 * @brief  Class for parsing and executing command
 */
class Execute
{
public:
    /**
     * @brief  Parse command
     * @param  command  Command code
     */
    static ECommand parseCommand(uint16_t command);

    /**
     * @brief  Parse parameters
     */
    static void parseParams();

    /**
     * @brief  Exec opening window
     */
    static SDL_Window* execOpenWindow();
};
