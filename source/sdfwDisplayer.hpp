/**
 * @file    MessageReceiver.hpp
 * @author  Y.Nakaue
 */

#pragma once

#include "sdfwComponent.hpp"

#include <SDL.h>
#include <SDL_thread.h>

#include <memory>
#include <string>
#include <tuple>
#include <vector>

class IEventHandler;
class IFontManager;
class IMessageReceiver;
class IWindowManager;

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
     * @brief  Initialize all components
     */
    void init();

    /**
     * @brief  Quit and release all components
     */
    void quit();

    /**
     * @brief  Get instance
     * @return  Instance of singleton
     */
    static sdfwDisplayer* get()
    {
        if (sdfwDisplayer::pInstance_ == nullptr)
        {
            sdfwDisplayer::pInstance_ = new sdfwDisplayer();
        }

        return pInstance_;
    }

    /**
     * @brief  Run displayer
     */
    void run();

    /**
     * @brief  Select and execute function
     */
    void executeCommand();

    /**
     * @brief  Get engine component
     * @return  Specified engine component
     */
    template<class T>
    static auto* getComponent()
    {
        return std::get<sdfwComponent<T>>(sdfwDisplayer::pInstance_->components_).get();
    }
    #define SDFW_DISPLAYER(COMPONENT) sdfwDisplayer::getComponent<I##COMPONENT>()

    bool getQuitFlag() { return this->quit_flag_; }
    void setQuitFlag(bool new_flag) { this->quit_flag_ = new_flag; }

private:
    /**
     * @brief  Execute opening window
     */
    void execOpenWindow(uint32_t width, uint32_t height);

    /**
     * @brief  Execute closing window
     * @param  (win_id)  The ID of window to close
     */
    void execCloseWindow(int32_t win_id);

    /**
     * @brief  Execute setting background color
     * @param  (color)  New background color
     * @param  (win_id)  Target window ID
     */
    void execSetBackground(uint8_t red, uint8_t green, uint8_t blue, int32_t win_id);

    /**
     * @brief  Execute print string
     * @param  (str)  Output string
     * @param  (win_id)  The ID of window to print string
     */
    void execPrint(std::string str, int32_t win_id);

    /**
     * @brief  Update all drawings to the latest state
     */
    void execUpdate();

    /// Instance for singleton
    inline static sdfwDisplayer* pInstance_ = nullptr;

    /// Displayer components
    std::tuple<
        sdfwComponent<IFontManager>,
        sdfwComponent<IEventHandler>,
        sdfwComponent<IMessageReceiver>,
        sdfwComponent<IWindowManager>
    > components_;

    /// Quit flag
    bool quit_flag_;

};
