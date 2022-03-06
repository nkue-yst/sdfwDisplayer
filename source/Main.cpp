/**
 * @file    Main.cpp
 * @author  Y.Nakaue
 */

#include "Main.hpp"
#include "sdfwDisplayer.hpp"
#include "MessageReceiver.hpp"

#include <thread>

#define PORT_NUM 62491

namespace sdfw
{

    /* Initialize singleton instances */
    void init()
    {
        sdfwDisplayer::get()->init();
    }

    /* Quit the application */
    void quit()
    {
        sdfwDisplayer::get()->setQuitFlag(true);
    }

    /* Quit system with error */
    void abort()
    {
        outputLog("Abort");

        sdfwDisplayer::get()->quit();
    }

    void outputLog(std::string message)
    {
        std::cout << "sdfw_Log: " << message << std::endl;
    }

    /* Receiving messages (in a new thread) */
    void threadMessageReceive()
    {
        outputLog("Start message receiving thread");

        SDFW_DISPLAYER(MessageReceiver)->openSocket(PORT_NUM);
        SDFW_DISPLAYER(MessageReceiver)->acceptConnection();

        SDFW_DISPLAYER(MessageReceiver)->receiveMessage();

        // Execute destroying singleton classes when quit flag is setted true
        sdfwDisplayer::get()->quit();
    }

}


int main(int argc, char** argv)
{
    sdfw::init();

    // Thread for receiving messages
    std::thread th_msg_recv(sdfw::threadMessageReceive);
    th_msg_recv.detach();

    sdfwDisplayer::get()->run();

    return 0;
}
