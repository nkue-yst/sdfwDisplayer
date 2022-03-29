/**
 * @file    Main.cpp
 * @author  Y.Nakaue
 */

#include "Main.hpp"
#include "sdfwDisplayer.hpp"
#include "EventHandler.hpp"
#include "MessageReceiver.hpp"

#include "SDL_ttf.h"

#include <filesystem>
#include <thread>

#define PORT_NUM 62491

namespace sdfw
{

    void init()
    {
        sdfwDisplayer::get()->init();
    }

    void quit()
    {
        sdfwDisplayer::get()->setQuitFlag(true);
    }

    void abort(std::string message)
    {
        outputLog("Abort -> " + message);

        sdfwDisplayer::get()->quit();
        exit(1);
    }

    void outputLog(std::string message)
    {
        std::cout << "sdfw_Log: " << message << std::endl;
    }

    void threadMessageReceive()
    {
        SDFW_DISPLAYER(MessageReceiver)->openSocket(PORT_NUM);
        SDFW_DISPLAYER(MessageReceiver)->acceptConnection();

        SDFW_DISPLAYER(MessageReceiver)->receiveMessage();
    }

    void threadHandlingEvent()
    {
        SDFW_DISPLAYER(EventHandler)->openSocket(PORT_NUM + 1);
        SDFW_DISPLAYER(EventHandler)->acceptConnection();
    }

}


int main(int argc, char** argv)
{
    sdfw::init();

    // Thread for receiving messages
    std::thread th_msg_recv(sdfw::threadMessageReceive);
    th_msg_recv.detach();

    // Thread for handling event
    std::thread th_ev(sdfw::threadHandlingEvent);
    th_ev.join();

    sdfwDisplayer::get()->run();

    sdfwDisplayer::get()->quit();
    return 0;
}
