/**
 * @file    sdfwDisplayer.cpp
 * @author  Y.Nakaue
 */

#include "sdfwDisplayer.hpp"
#include "Command.hpp"
#include "EventHandler.hpp"
#include "FontManager.hpp"
#include "Logger.hpp"
#include "Main.hpp"
#include "MessageReceiver.hpp"
#include "Window.hpp"

#include <SDL_net.h>
#include <SDL2_gfxPrimitives.h>

#include <iostream>
#include <string>

constexpr uint16_t BUFF_SIZE = 512;

sdfwDisplayer::sdfwDisplayer()
    : quit_flag_(false)
{
    sdfwDisplayer::pInstance_ = this;

    /* Initialize SDL2 */
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        sdfw::abort();
    }

    /* Initialize SDL2_net */
    if (SDLNet_Init() != 0)
    {
        abort();
    }
}

sdfwDisplayer::~sdfwDisplayer()
{
    sdfw::outputLog("The application terminated successfully.");

    /* Quit SDL2 and SDL2_net */
    SDLNet_Quit();
    SDL_Quit();

    // Release memory of instance
    delete sdfwDisplayer::pInstance_;
    sdfwDisplayer::pInstance_ = nullptr;
}

/* Initialize all components */
void sdfwDisplayer::init()
{
    SDFW_DISPLAYER(EventHandler)->init();
    SDFW_DISPLAYER(FontManager)->init();
    SDFW_DISPLAYER(MessageReceiver)->init();
    SDFW_DISPLAYER(WindowManager)->init();
}

namespace components
{
    /**
     * @brief  Release an engine component
     */
    template<size_t N, class T>
    static void release(T&& t)
    {
        std::get<N>(t).release();

        if constexpr (N > 0)
        {
            components::release<N - 1>(t);
        }
    }

    /**
     * @brief  Release all engine components
     */
    template <class T>
    static void releaseAll(T&& t)
    {
        return components::release<std::tuple_size_v<std::remove_reference_t<T>> -1>(std::forward<T>(t));
    }
}

/* Quit and release all components */
void sdfwDisplayer::quit()
{
    /* Delay for safe memory release */
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    components::releaseAll(this->components_);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void sdfwDisplayer::run()
{
    while (!this->getQuitFlag())
    {
        executeCommand();
    }
}

/* Select and execute function */
void sdfwDisplayer::executeCommand()
{
    SDFW_DISPLAYER(MessageReceiver)->cmd_buff_mutex_.lock();

    // Execute and dequeue commands in receiving buffer
    for (Command cmd : SDFW_DISPLAYER(MessageReceiver)->cmd_buff_)
    {
        // Print for debug
        // std::cout << cmd << std::endl;

        // Select called function
        if (cmd.isEqualFunc("quit"))
        {
            this->setQuitFlag(true);
            break;
        }
        else if (cmd.isEqualFunc("openWindow"))
        {
            this->execOpenWindow(stoi(cmd.arguments[0]), stoi(cmd.arguments[1]));
        }
        else if (cmd.isEqualFunc("closeWindow"))
        {
            this->execCloseWindow(stoi(cmd.arguments[0]));
        }
        else if (cmd.isEqualFunc("setBackground"))
        {
            this->execSetBackground(stoi(cmd.arguments[0]), stoi(cmd.arguments[1]), stoi(cmd.arguments[2]), stoi(cmd.arguments[3]));
        }
        else if (cmd.isEqualFunc("draw"))
        {
            std::string shape_name = cmd.arguments[0];
            cmd.arguments.erase(cmd.arguments.begin());
            this->execDrawShape(shape_name, cmd.arguments);
        }
        else if (cmd.isEqualFunc("print"))
        {
            SDFW_DISPLAYER(Logger)->addToBuffer(cmd.arguments[0], stoi(cmd.arguments[1]));
        }
        else if (cmd.isEqualFunc("update"))
        {
            this->execUpdate();
        }

        SDFW_DISPLAYER(MessageReceiver)->cmd_buff_.erase(SDFW_DISPLAYER(MessageReceiver)->cmd_buff_.begin());
    }

    SDFW_DISPLAYER(MessageReceiver)->cmd_buff_mutex_.unlock();
}

void sdfwDisplayer::execOpenWindow(uint32_t width, uint32_t height)
{
    /* Create new window */
    Window* win = new Window();

    std::string window_title = "Window - " + std::to_string(SDFW_DISPLAYER(WindowManager)->window_list_.size());
    win->window_ = SDL_CreateWindow(window_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (win->window_ == NULL)
    {
        sdfw::abort();
    }

    /* Create new renderer for new window */
    win->renderer_ = SDL_CreateRenderer(win->window_, -1, SDL_RENDERER_ACCELERATED);
    if (win->renderer_ == NULL)
    {
        sdfw::abort();
    }

    // Add new window to window list
    SDFW_DISPLAYER(WindowManager)->window_list_.push_back(win);

    /* Clear renderer */
    Color bg_color = win->bg_color_;
    SDL_SetRenderDrawColor(win->renderer_, bg_color.r, bg_color.g, bg_color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(win->renderer_);
    SDL_RenderPresent(win->renderer_);
}

void sdfwDisplayer::execCloseWindow(int32_t win_id)
{
    /* Destroy window and renderer */
    SDL_DestroyWindow(SDFW_DISPLAYER(WindowManager)->window_list_.at(win_id)->window_);
    SDL_DestroyRenderer(SDFW_DISPLAYER(WindowManager)->window_list_.at(win_id)->renderer_);
}

void sdfwDisplayer::execSetBackground(uint8_t red, uint8_t green, uint8_t blue, int32_t win_id)
{
    /* Set drawing color and clear scene */
    SDFW_DISPLAYER(WindowManager)->window_list_.at(win_id)->bg_color_.r = red;
    SDFW_DISPLAYER(WindowManager)->window_list_.at(win_id)->bg_color_.g = green;
    SDFW_DISPLAYER(WindowManager)->window_list_.at(win_id)->bg_color_.b = blue;
}

void sdfwDisplayer::execDrawShape(std::string name, std::vector<std::string> params)
{
    if (name == "Line")
    {
        int16_t x0, y0, x1, y1;
        int32_t thickness, win;

        x0 = static_cast<int16_t>(stoi(params.at(0)));
        y0 = static_cast<int16_t>(stoi(params.at(1)));
        x1 = static_cast<int16_t>(stoi(params.at(2)));
        y1 = static_cast<int16_t>(stoi(params.at(3)));
        thickness = stoi(params.at(4));
        win = stoi(params.at(9));

         thickLineRGBA(SDFW_DISPLAYER(WindowManager)->window_list_.at(win)->renderer_, x0, y0, x1, y1, thickness, stoi(params.at(5)), stoi(params.at(6)), stoi(params.at(7)), stoi(params.at(8)));
    }
    else if (name == "Circle")
    {
        int16_t x, y, rad;
        int32_t win;

        x = static_cast<int16_t>(stoi(params.at(0)));
        y = static_cast<int16_t>(stoi(params.at(1)));
        rad = static_cast<int16_t>(stoi(params.at(2)));
        win = stoi(params.at(7));

        filledCircleRGBA(SDFW_DISPLAYER(WindowManager)->window_list_.at(win)->renderer_, x, y, rad, stoi(params.at(3)), stoi(params.at(4)), stoi(params.at(5)), stoi(params.at(6)));
    }
}

void sdfwDisplayer::execUpdate()
{
    /* Update input state */
    SDL_Event ev;
    char msg[BUFF_SIZE] = "";

    /* Get and send events by mouse and keyboard */
    while (SDL_PollEvent(&ev))
    {
        switch (ev.type)
        {
            /* Mouse button down event */
            case SDL_MOUSEBUTTONDOWN:
                strcat_s(msg, "Mouse/Button/Down/");
                switch (ev.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        strcat_s(msg, "LEFT");
                        strcat_s(msg, "/");
                        break;

                    case SDL_BUTTON_MIDDLE:
                        strcat_s(msg, "MIDDLE");
                        strcat_s(msg, "/");
                        break;

                    case SDL_BUTTON_RIGHT:
                        strcat_s(msg, "RIGHT");
                        strcat_s(msg, "/");
                        break;

                    default:
                        break;
                }
                strcat_s(msg, std::to_string(ev.button.x).c_str());
                strcat_s(msg, "/");
                strcat_s(msg, std::to_string(ev.button.y).c_str());
                SDFW_DISPLAYER(EventHandler)->sendMessage(msg);
                break;

            /* Mouse button up event */
            case SDL_MOUSEBUTTONUP:
                strcat_s(msg, "Mouse/Button/Up/");
                switch (ev.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        strcat_s(msg, "LEFT");
                        strcat_s(msg, "/");
                        break;

                    case SDL_BUTTON_MIDDLE:
                        strcat_s(msg, "MIDDLE");
                        strcat_s(msg, "/");
                        break;

                    case SDL_BUTTON_RIGHT:
                        strcat_s(msg, "RIGHT");
                        strcat_s(msg, "/");
                        break;

                    default:
                        break;
                }
                strcat_s(msg, std::to_string(ev.button.x).c_str());
                strcat_s(msg, "/");
                strcat_s(msg, std::to_string(ev.button.y).c_str());
                SDFW_DISPLAYER(EventHandler)->sendMessage(msg);
                break;

            default:
                break;
        }

        /* User quit request */
        if (ev.window.event == SDL_WINDOWEVENT_CLOSE)
        {
            strcat_s(msg, "QUIT");
            SDFW_DISPLAYER(EventHandler)->sendMessage(msg);
            return;
        }
    }


    /* Send mouse cursor coordinate */
    // Initialize message buffer
    memset(msg, '\0', sizeof(msg));

    int32_t x, y;  // Mouse cursor coord

    SDL_GetMouseState(&x, &y);
    strcat_s(msg, "Mouse/X/");
    strcat_s(msg, std::to_string(x).c_str());
    strcat_s(msg, "/Y/");
    strcat_s(msg, std::to_string(y).c_str());

    SDFW_DISPLAYER(EventHandler)->sendMessage(msg);

    /* Draw text */
    SDFW_DISPLAYER(Logger)->execPrint();

    /* Update drawing */
    for (Window* win : SDFW_DISPLAYER(WindowManager)->window_list_)
    {
        SDL_RenderPresent(win->renderer_);
        SDL_SetRenderDrawColor(win->renderer_, win->bg_color_.r, win->bg_color_.g, win->bg_color_.b, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(win->renderer_);
    }

    /* Refresh print buffer */
    for (auto& str_list : SDFW_DISPLAYER(Logger)->buff_)
    {
        str_list.clear();
    }
}
