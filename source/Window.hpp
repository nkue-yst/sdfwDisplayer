/**
 * @file    Window.hpp
 * @author  Y.Nakaue
 */

#pragma once

#include <SDL.h>

#include <vector>

 /**
  * @brief  Window interface class
  */
class IWindow
{
public:
    virtual ~IWindow() = default;

    /**
     * @brief  Create instance
     */
    static IWindow* create();

    /**
     * @brief  Initialize window class
     */
    virtual void init() = 0;

    /// SDL window list
    std::vector<SDL_Window*> window_list_;

    /// SDL Renderer list
    std::vector<SDL_Renderer*> renderer_list_;
};


 /**
  * @brief  Window handller class
  */
class Window : public IWindow
{
public:
    Window();
    ~Window() override;

    /**
     * @brief  Initialize window class
     */
    void init() override;
};
