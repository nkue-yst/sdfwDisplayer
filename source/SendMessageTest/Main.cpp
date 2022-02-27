#include <SDL.h>
#include <SDL_net.h>

#include <chrono>
#include <iostream>
#include <thread>

#define PORT_NUM 62491

int main(int argc, char** argv)
{
    if (SDL_Init(0) == -1)
    {
        std::cout << "Error: SDL_Init() - " << SDL_GetError() << std::endl;
        exit(1);
    }

    if (SDLNet_Init() == -1)
    {
        std::cout << "Error: SDLNet_Init() - " << SDLNet_GetError() << std::endl;
        exit(2);
    }

    // Start TCP connection
    IPaddress ip_addr;
    TCPsocket sock;

    if (SDLNet_ResolveHost(&ip_addr, "localhost", PORT_NUM) == -1)
    {
        std::cout << "Error: SDLNet_ResolveHost() - " << SDLNet_GetError() << std::endl;
        exit(3);
    }

    // Connection request
    while (true)
    {
        sock = SDLNet_TCP_Open(&ip_addr);
        if (sock)
        {
            break;
        }
        else
        {
            std::cout << "Connection not complete." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
    std::cout << "Connection complete" << std::endl;

    // Send test character
    int32_t result;

    while (true)
    {
        std::cout << "Command -> ";
        char msg[1024];
        std::cin >> msg;
        int32_t length = static_cast<int32_t>(strlen(msg) + 1);

        result = SDLNet_TCP_Send(sock, &msg, length);
        if (result < length)
        {
            std::cout << "Error: SDLNet_TCP_Send() - " << SDLNet_GetError() << std::endl;
        }

        if (strstr(msg, "quit") != NULL)
            break;
    }

    // End-Process
    SDLNet_TCP_Close(sock);
    SDLNet_Quit();
    SDL_Quit();

    return 0;
}
