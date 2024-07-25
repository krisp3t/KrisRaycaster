#include <iostream>
#include <SDL2/SDL.h>

#include "Renderer.h"
#include "Map.h"
#include "Texture.h"
#include "Game.h"

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
        framebuffer = std::vector<uint32_t>(settings.framebufferWidth * settings.framebufferHeight, 0);
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

    // Basic (slow) implementation - just go along the rays with increments until you hit wall
    void Renderer::CastRaysBasic()
    {
        // TODO: display in minimap
        float rayAngle = settings.playerAngle - (settings.fov / 2); // if FOV 60, then [-30, 30]
        int mapSize = Game::Get().systems.map->GetSize();
        for (int i = 0; i < settings.framebufferWidth; i++)
        {
            Vec2 ray;

            float dx = cos(rayAngle);
            float dy = sin(rayAngle);
            int wall = 0;
            while (wall == 0)
            {
                ray = {
                        static_cast<int>(floor(settings.playerPos.x + dx)),
                        static_cast<int>(floor(settings.playerPos.y + dy))
                };
                wall = Game::Get().systems.map->Get(ray.x / mapSize, ray.y / mapSize);
                dx += cos(rayAngle) / settings.rayPrecision;
                dy += sin(rayAngle) / settings.rayPrecision;
                rayAngle += settings.rayIncrementAngle;
            }
            float distance = sqrt(pow(ray.x, 2) + pow(ray.y, 2));
            int wallHeight = floor(settings.framebufferHeight / distance);
            SDL_Log("Drawing line at %d with height %d, distance %f, wall: (%d, %d)", i, wallHeight, distance, ray.x,
                    ray.y);

            DrawVLine(i, wallHeight);
        }
    }

    void Renderer::DrawVLine(int x, int height)
    {
        for (int y = settings.framebufferHeight - 1; y >= height; y--)
        {
            framebuffer[y * settings.framebufferWidth + x] = 0xFF0000FF;
        }
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
        CastRaysBasic();
    }

    void Renderer::Render()
    {
        BeforeFrame();
        DrawFrame();
        // draw minimap
        SDL_RenderCopy(sdlRend, minimapTexture, nullptr, &leftRec);
        // draw raycasted scene
        SDL_UpdateTexture(framebufferTexture, nullptr, framebuffer.data(),
                          static_cast<int>(settings.framebufferWidth * sizeof(uint32_t)));
        SDL_RenderCopy(sdlRend, framebufferTexture, nullptr, &rightRec);
        SDL_RenderPresent(sdlRend);
    }

    Renderer::~Renderer()
    {
        SDL_DestroyTexture(framebufferTexture);
    }

}