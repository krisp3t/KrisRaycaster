#include <iostream>
#include <SDL.h>
#include "constants.h"
#include "map.h"
#include "text.h"

namespace KrisRaycaster
{
    bool ProcessInput()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return true;
            }
        }
        return false;
    }

    void Update()
    {
        return;
    }

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
        SDL_SetWindowTitle(window, "KrisRaycaster");
        SDL_Texture *framebufferTexture = SDL_CreateTexture(
                renderer,
                SDL_PIXELFORMAT_ABGR8888,
                SDL_TEXTUREACCESS_STREAMING,
                FRAMEBUFFER_WIDTH,
                FRAMEBUFFER_HEIGHT);
        if (!framebufferTexture)
        {
            std::cerr << "Failed to create framebuffer texture : " << SDL_GetError() << std::endl;
            return -1;
        }

        ReadBytes("./map/minecraft_map.txt");

        while (true)
        {
            bool quit = ProcessInput();
            if (quit)
            {
                break;
            }
            Update();
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, framebufferTexture, nullptr, nullptr);
            SDL_RenderPresent(renderer);
        }
        // SDL_UpdateTexture()
        SDL_DestroyTexture(framebufferTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }
}

int main(int argc, char *args[])
{
    return KrisRaycaster::main(argc, args);
}