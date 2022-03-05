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
        sdfwMessageReceiver::get();
        sdfwDisplayer::get();
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

        sdfwMessageReceiver::get()->destroy();
        sdfwDisplayer::get()->destroy();
    }

    void outputLog(std::string message)
    {
        std::cout << "sdfw_Log: " << message << std::endl;
    }

    /* Receiving messages (in a new thread) */
    void threadMessageReceive()
    {
        outputLog("Start message receiving thread");

        sdfwMessageReceiver* msg_recv = sdfwMessageReceiver::get();

        msg_recv->openSocket(PORT_NUM);
        msg_recv->acceptConnection();

        msg_recv->receiveMessage();

        // Execute destroying singleton classes when quit flag is setted true
        sdfwMessageReceiver::destroy();
        sdfwDisplayer::destroy();
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
