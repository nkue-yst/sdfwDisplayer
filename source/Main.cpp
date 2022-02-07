/**
 * @file    Assertion.cpp
 * @author  Y.Nakaue
 */

#include "Assertion.hpp"

#include <SDL.h>
#include <SDL_net.h>

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDFW_Assertion::Abort("Failed to initialize SDL2");
    }

    if (SDLNet_Init() != 0)
    {
        SDFW_Assertion::Abort("Failed to initialize SDL2_net");
    }

    SDLNet_Quit();
    SDL_Quit();
    return 0;
}
