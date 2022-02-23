/**
 * @file    MessageReceiver.hpp
 * @author  Y.Nakaue
 */

#pragma once
#include <SDL.h>
#include <SDL_thread.h>

#include <memory>
#include <string>
#include <vector>

 /**
  * @brief  Main application class
  */
class sdfwDisplayer
{
public:
    /**
     * @brief  Initialize SDL2 and SDL2_net
     */
    sdfwDisplayer();

    /**
     * @brief  Quit SDL2 and SDL2_net
     */
    ~sdfwDisplayer();

    /**
     * @brief  Initialize sdfw Displayer app
     * @param  port  Port num using TCP connection
     */
    void init(uint16_t port);

    /**
     * @brief  Run displayer
     */
    void run();

    /**
     * @brief  Output error message and quit system
     * @param  message  Output message
     */
    void Abort(std::string message);

    /**
     * @brief  Output log message
     * @param  message  Output message
     */
    static void OutputLog(std::string message);

    /**
     * @brief  Select and execute function
     */
    void executeCommand(std::string message);

private:
    /**
     * @brief  Parse a string
     * @param  str  Target string
     * @param  delimiter  Character used to delimit string
     * @return  Split word list
     */
    std::vector<std::string> parseMessage(const std::string& str, const char delimiter = '/');

    /**
     * @brief  Execute opening window
     */
    void execOpenWindow(uint32_t width, uint32_t height);

    /// TCP receiver
    std::unique_ptr<class sdfwMessageReceiver> message_receiver_;

    /// SDL Window
    SDL_Window* window_;

    /// SDL Renderer
    SDL_Renderer* renderer_;

    /// Quit flag
    bool quit_flag_;
};
