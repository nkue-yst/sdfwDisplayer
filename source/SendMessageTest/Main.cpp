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

    // Send test character
    int32_t result;
    uint8_t msg;

    while (true)
    {
        std::cout << "Command -> ";
        std::scanf("%u", &msg);

        result = SDLNet_TCP_Send(sock, &msg, 1);
        if (result < sizeof(msg))
        {
            std::cout << "Error: SDLNet_TCP_Send() - " << SDLNet_GetError() << std::endl;
        }

        auto printb = [](uint32_t x)
        {
            putchar('0');
            putchar('b');

            uint32_t mask = (int)1 << (sizeof(x) * CHAR_BIT - 1);
            do
            {
                putchar(mask & x ? '1' : '0');
            } while (mask >>= 1);

            putchar('\n');
        };

        printb(msg);

        if (msg == 0b00000000)
        {
            break;
        }
    }

    // End-Process
    SDLNet_TCP_Close(sock);
    SDLNet_Quit();
    SDL_Quit();

    return 0;
}
