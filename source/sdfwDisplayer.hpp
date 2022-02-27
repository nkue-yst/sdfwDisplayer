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
protected:
    /**
     * @brief  Initialize SDL2 and SDL2_net
     */
    sdfwDisplayer();

    /**
     * @brief  Quit SDL2 and SDL2_net
     */
    ~sdfwDisplayer();

public:
    /**
     * @brief  Create instance
     */
    static sdfwDisplayer* create()
    {
        return new sdfwDisplayer();
    }

    /**
     * @brief  Get instance
     * @return  Instance of singleton
     */
    static sdfwDisplayer* get()
    {
        if (pInstance_ == nullptr)
        {
            pInstance_ = create();
        }

        return pInstance_;
    }

    /**
     * @brief  Destroy instance
     */
    static void destroy()
    {
        delete pInstance_;
        pInstance_ = nullptr;
    }

    /**
     * @brief  Run displayer
     */
    void run();

    /**
     * @brief  Select and execute function
     */
    void executeCommand();

private:
    /**
     * @brief  Execute opening window
     */
    void execOpenWindow(uint32_t width, uint32_t height);

    /// Instance for singleton
    inline static sdfwDisplayer* pInstance_ = nullptr;

    /// SDL Window
    SDL_Window* window_;

    /// SDL Renderer
    SDL_Renderer* renderer_;

    /// Quit flag
    bool quit_flag_;

};
