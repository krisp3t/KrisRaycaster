#include <iostream>
#include <SDL2/SDL.h>

#include "Renderer.h"
#include "Map.h"
#include "Texture.h"

namespace KrisRaycaster
{
    Renderer::Renderer(SDL_Renderer *renderer) : sdlRend(renderer)
    {
        Init();
    }

    int Renderer::Init()
    {
        framebufferTexture = SDL_CreateTexture(
                sdlRend,
                SDL_PIXELFORMAT_ABGR8888,
                SDL_TEXTUREACCESS_STREAMING,
                settings.framebufferWidth,
                settings.framebufferHeight
        );

        if (!framebufferTexture)
        {
            std::cerr << "Failed to create framebuffer texture : " << SDL_GetError() << std::endl;
            return -1;
        }
        /*
        std::unique_ptr<Texture> wallTexture = std::make_unique<Texture>(
                "./assets/minecraft/terrain.png",
                TextureFormat{32, 32, 256, 16, SDL_PIXELFORMAT_ABGR8888},
                sdlRend
        );
         */
        return 0;
    }

    void Renderer::Render()
    {
        SDL_SetRenderDrawColor(sdlRend, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(sdlRend);
        // SDL_Rect g = wallTexture->GetRect(8);
        //   SDL_RenderCopy(renderer, wallTexture->img, &g, nullptr);
        SDL_RenderPresent(sdlRend);

    }

    Renderer::~Renderer()
    {
        SDL_DestroyTexture(framebufferTexture);
    }
}