#include <iostream>
#include <SDL.h>
#include "constants.h"
#include "map.h"
#include "text.h"
#include "texture.h"

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
                SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_RESIZABLE,
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

        std::unique_ptr<Texture> wallTexture = std::make_unique<Texture>(
                "./assets/minecraft/terrain.png",
                TextureFormat{32, 32, 256, 16, SDL_PIXELFORMAT_ABGR8888},
                *renderer
        );
        std::unique_ptr<Map> map = std::make_unique<Map>(
                *wallTexture,
                "./map/minecraft_map.txt");

        while (true)
        {
            bool quit = ProcessInput();
            if (quit)
            {
                break;
            }
            Update();
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(renderer);
            SDL_Rect g = wallTexture->GetRect(8);
            //SDL_RenderCopy(renderer, wallTexture->img, nullptr, nullptr);
            SDL_RenderCopy(renderer, wallTexture->img, &g, nullptr);
            //SDL_RenderCopy(renderer, framebufferTexture, nullptr, nullptr);
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