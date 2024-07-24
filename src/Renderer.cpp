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
        leftRec = SDL_Rect{0, 0, settings.framebufferWidth, settings.framebufferHeight};
        rightRec = SDL_Rect{settings.framebufferWidth, 0, settings.framebufferWidth, settings.framebufferHeight};
        return 0;
    }

    bool Renderer::InitMinimap(const Texture &floorTex, const std::vector<uint_fast8_t> &layout)
    {
        const int tileBorder = 1;
        minimapTexture = SDL_CreateTexture(
                sdlRend,
                SDL_PIXELFORMAT_ABGR8888,
                SDL_TEXTUREACCESS_TARGET,
                settings.framebufferWidth,
                settings.framebufferHeight
        );
        if (!minimapTexture)
        {
            std::cerr << "Failed to create minimap texture : " << SDL_GetError() << std::endl;
            return false;
        }
        SDL_SetRenderTarget(sdlRend, minimapTexture);
        int ix = 0;
        int mapSize = layout.size();
        int mapWidth = sqrt(mapSize);
        Vec2 tileSize{settings.framebufferWidth / mapWidth, settings.framebufferHeight / mapWidth};
        for (uint_fast8_t tile: layout)
        {
            SDL_Rect dest;
            dest.x = (ix % mapWidth) * tileSize.x;
            dest.y = (ix / mapWidth) * tileSize.y;
            dest.w = tileSize.x;
            dest.h = tileSize.y;
            if (tile == 0)
            {
                SDL_SetRenderDrawColor(sdlRend, 0xE5, 0xE5, 0xE5, 0xFF);
                SDL_RenderFillRect(sdlRend, &dest);
            } else
            {
                dest.x += tileBorder;
                dest.y += tileBorder;
                dest.w -= tileBorder * 2;
                dest.h -= tileBorder * 2;
                SDL_Rect src = floorTex.GetRect(tile);
                SDL_RenderCopy(sdlRend, floorTex.img, &src, &dest);
            }
            ix++;
        }
        //SDL_Texture *t = SDL_CreateTextureFromSurface(sdlRend, floorTex.img);


        SDL_SetRenderTarget(sdlRend, nullptr);
        return true;
    }

    Texture *Renderer::CreateTexture(
            TextureFormat format
    )
    {
        auto t = Texture{format, sdlRend};
        items.push_back(t);
        return &items.back();
    }

    Texture *Renderer::CreateTexture(
            const std::string &filename,
            TextureFormat format
    )
    {
        auto t = Texture{filename, format, sdlRend};
        items.push_back(t);
        return &items.back();
    }

    void Renderer::BeforeFrame()
    {
        SDL_SetRenderTarget(sdlRend, nullptr);
        SDL_SetRenderDrawColor(sdlRend, 0xFF, 0x00, 0xFF, 0xFF);
        SDL_RenderClear(sdlRend);
    }

    void Renderer::DrawFrame()
    {
        for (const auto &item: items)
        {
            /*
            SDL_LockTexture(framebufferTexture, nullptr, &item.pixels, &item.pitch);

            SDL_UnlockTexture(framebufferTexture);
             */
        }
    }

    void Renderer::Render()
    {
        BeforeFrame();
        DrawFrame();
        // draw minimap
        SDL_RenderCopy(sdlRend, minimapTexture, nullptr, &leftRec);
        // draw raycasted scene
        SDL_RenderCopy(sdlRend, framebufferTexture, nullptr, &rightRec);
        SDL_RenderPresent(sdlRend);
    }

    Renderer::~Renderer()
    {
        SDL_DestroyTexture(framebufferTexture);
    }

}