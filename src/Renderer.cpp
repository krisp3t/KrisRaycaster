#include <iostream>
#include <SDL2/SDL.h>

#include "Renderer.h"
#include "Map.h"
#include "Texture.h"

namespace KrisRaycaster
{
    Renderer &Renderer::Get()
    {
        static Renderer instance;
        return instance;
    }

    int Renderer::Init(SDL_Renderer *rend)
    {
        sdlRend = rend;
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
        return 0;
    }

    Texture Renderer::CreateTexture(
            TextureFormat format
    )
    {
        return Texture{format, sdlRend};
    }

    // TODO: copy?
    Texture Renderer::CreateTexture(
            const std::string &filename,
            TextureFormat format
    )
    {
        return Texture{filename, format, sdlRend};
    }

    void Renderer::CopyToFramebuffer(const Texture &texture)
    {
        /*
        SDL_UpdateTexture(
                framebufferTexture,
                nullptr,
                texture.img,
                texture.format.imgW * sizeof(uint32_t)
        );
         */
    }

    void Renderer::BeforeFrame()
    {
        SDL_SetRenderDrawColor(sdlRend, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(sdlRend);
    }

    void Renderer::DrawFrame()
    {
        // Draw minimap
        // SDL_Rect g = wallTexture->GetRect(8);
        //   SDL_RenderCopy(renderer, wallTexture->img, &g, nullptr);
    }

    void Renderer::Render()
    {
        BeforeFrame();
        DrawFrame();
        SDL_RenderPresent(sdlRend);
    }

    Renderer::~Renderer()
    {
        SDL_DestroyTexture(framebufferTexture);
    }

}