/**
 * @file    Window.hpp
 * @author  Y.Nakaue
 */

#pragma once
#include "Color.hpp"

#include <SDL.h>

#include <vector>

 /**
  * @brief  Class of window info
  */
class Window
{
public:
    Window()
        : bg_color_(Color())
        , window_(nullptr)
        , renderer_(nullptr)
    {}

    /// Background color
    Color bg_color_;

    /// SDL window
    SDL_Window* window_;

    /// SDL renderer
    SDL_Renderer* renderer_;
};


 /**
  * @brief  Window interface class
  */
class IWindowManager
{
public:
    virtual ~IWindowManager() = default;

    /**
     * @brief  Create instance
     */
    static IWindowManager* create();

    /**
     * @brief  Initialize window class
     */
    virtual void init() = 0;

    /// Created window list
    std::vector<Window*> window_list_;
};


 /**
  * @brief  Window handller class
  */
class WindowManager : public IWindowManager
{
public:
    WindowManager();
    ~WindowManager() override;

    /**
     * @brief  Initialize window class
     */
    void init() override;
};
