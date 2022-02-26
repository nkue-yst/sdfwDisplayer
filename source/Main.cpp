/**
 * @file    Main.cpp
 * @author  Y.Nakaue
 */

#include "sdfwDisplayer.hpp"
#include "MessageReceiver.hpp"

#include <thread>

#define PORT_NUM 62491

namespace sdfw
{

    /* @brief  Quit system with error */
    void abort()
    {
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
        sdfwMessageReceiver* msg_recv = sdfwMessageReceiver::get();

        std::cout << msg_recv->openSocket(PORT_NUM) << std::endl;
        msg_recv->acceptConnection();

        msg_recv->receiveMessage();
    }

}


int main(int argc, char** argv)
{
    // Thread for receiving messages
    std::thread th_msg_recv(sdfw::threadMessageReceive);
    th_msg_recv.detach();

    sdfwDisplayer::get()->run();

    return 0;
}
