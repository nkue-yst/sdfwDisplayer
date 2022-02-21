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
    bool executeCommand(uint32_t command_code);

    /**
     * @brief  Execute opening window
     */
    bool execOpenWindow();

private:
    /// TCP receiver
    std::unique_ptr<class sdfwMessageReceiver> message_receiver_;

    /// SDL Window
    SDL_Window* window_;

    /// SDL Renderer
    SDL_Renderer* renderer_;

    /// Function queue
    std::vector<uint32_t> func_;

    /// Parameters queue
    std::vector<uint32_t> params_;
};
