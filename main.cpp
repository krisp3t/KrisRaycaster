#include <iostream>
#include <SDL.h>
#include "constants.h"

namespace KrisRaycaster
{
    int main(int argc, char *args[])
    {
        if (SDL_Init(SDL_INIT_VIDEO))
        {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
            return -1;
        }

        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;
        if (SDL_CreateWindowAndRenderer(
                FRAMEBUFFER_WIDTH,
                FRAMEBUFFER_HEIGHT,
                SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS,
                &window,
                &renderer)
                )
        {
            std::cerr << "Failed to create window and renderer: " << SDL_GetError() << std::endl;
            return -1;
        }

        return 0;
    }
}

int main(int argc, char *args[])
{
    return KrisRaycaster::main(argc, args);
}