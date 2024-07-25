#include <iostream>
#include <SDL.h>
#include <cassert>
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

    void Renderer::DrawPlayerMinimap()
    {
        constexpr int playerSize = 20;
        constexpr int playerOffset = playerSize / 2;
        // Draw rectangle for player
        Map *map = Game::Get().systems.map.get();
        int mapSize = map->GetSize();
        Vec2f playerPos = {
                map->playerPos.x * settings.framebufferWidth / mapSize,
                map->playerPos.y * settings.framebufferHeight / mapSize
        };


        // Draw player position (as a small circle or rectangle)
        SDL_SetRenderDrawColor(sdlRend, 255, 0, 0, 255);  // Red color
        SDL_Rect playerRect = {
                static_cast<int>(playerPos.x),
                static_cast<int>(playerPos.y),
                playerSize, playerSize
        };
        SDL_RenderFillRect(sdlRend, &playerRect);

        // Draw rays
        SDL_SetRenderDrawColor(sdlRend, 0, 0, 255, 255);  // Blue color
        /*
        for (const auto &ray: rays)
        {
            SDL_RenderDrawLine(sdlRend,
                               static_cast<int>(playerX + playerOffset),
                               static_cast<int>(playerY + playerOffset),
                               ray.x,
                               ray.y
            );
        }
         */
        SDL_RenderDrawLines(sdlRend, rays.data(), rays.size());

        Vec2f cameraRay = map->cameraPlane * 50.0f;
        Vec2f playerCentre = playerPos + static_cast<float>(playerOffset);
        Vec2f dirRay = playerCentre + map->dir * 50.0f;

        // Draw player direction
        SDL_SetRenderDrawColor(sdlRend, 180, 0, 0, 255);
        SDL_RenderDrawLine(sdlRend,
                           static_cast<int>(playerCentre.x),
                           static_cast<int>(playerCentre.y),
                           static_cast<int>(dirRay.x),
                           static_cast<int>(dirRay.y)
        );

        // Draw camera plane
        Vec2f cameraStart = {
                playerCentre.x + map->cameraPlane.x * 50.0f,
                playerCentre.y + map->cameraPlane.y * 50.0f
        };
        Vec2f cameraEnd = {
                playerCentre.x - map->cameraPlane.x * 50.0f,
                playerCentre.y - map->cameraPlane.y * 50.0f
        };
        SDL_SetRenderDrawColor(sdlRend, 0, 255, 0, 255);  // Green color
        SDL_RenderDrawLine(sdlRend,
                           static_cast<int>(dirRay.x - cameraRay.x),
                           static_cast<int>(dirRay.y - cameraRay.y),
                           static_cast<int>(dirRay.x + cameraRay.x),
                           static_cast<int>(dirRay.y + cameraRay.y)
        );


    }

    // Basic (slow) implementation - just go along the rays with increments until you hit wall
    void Renderer::CastRaysBasic()
    {
        rays = std::vector<SDL_Point>(settings.framebufferWidth * 2);
        Map *map = Game::Get().systems.map.get();
        Vec2f playerPos = map->playerPos;
        Vec2f dir = map->dir;
        Vec2f cameraPlane = map->cameraPlane;
        int mapSize = Game::Get().systems.map->GetSize();
        //int rectW = settings.framebufferWidth / mapSize;
        //int rectH = settings.framebufferHeight / mapSize;
        for (int x = 0; x < settings.framebufferWidth; x++)
        {
            // if FOV=60 and fbWidth=720, then angle from [-30, 30] with increments of 60/720
            float cameraX = 2 * x / static_cast<float>(settings.framebufferWidth) - 1; // x in camera space [-1, 1]
            Vec2f rayDir = {dir.x + cameraPlane.x * cameraX,
                            dir.y + cameraPlane.y * cameraX};
            float distance = 0;
            while (distance < 100)
            {
                Vec2f rayPos = playerPos + rayDir * distance;
                int mapX = static_cast<int>(rayPos.x);
                int mapY = static_cast<int>(rayPos.y);
                int wall = map->Get(mapX, mapY);
                if (wall != 0)
                {
                    int pixelX = floor((rayPos.x / mapSize) * settings.framebufferWidth);
                    int pixelY = floor((rayPos.y / mapSize) * settings.framebufferHeight);
                    SDL_Point p = {pixelX, pixelY};
                    rays[x] = {static_cast<int>(rayDir.x), static_cast<int>(rayDir.y)};
                    rays[x + 1] = p;
                    break;
                }
                distance += settings.rayIncrement;
            }
            int wallHeight = floor(settings.framebufferHeight / Fmax(distance, 1.0));
            // ceiling
            DrawVLine(x, 0, settings.framebufferHeight / 2 - wallHeight / 2, 0xFF00FF00);
            // walls
            DrawVLine(x, settings.framebufferHeight / 2 - wallHeight / 2, wallHeight, 0xFF0000FF);
            // floor
            DrawVLine(x, settings.framebufferHeight / 2 + wallHeight / 2,
                      settings.framebufferHeight / 2 - wallHeight / 2,
                      0xFFFF0000);
        }
    }

    void Renderer::DrawVLine(int x, int y, int height, uint32_t color)
    {
        assert((y >= 0 && y < settings.framebufferHeight) || (height == 0));
        assert(height >= 0 && height <= settings.framebufferHeight);
        assert(x >= 0 && x < settings.framebufferWidth || (height == 0));
        for (int i = 0; i < height; i++)
        {
            int iy = y + i;
            framebuffer[iy * settings.framebufferWidth + x] = color;
        }
    }

    void Renderer::BeforeFrame()
    {
        SDL_SetRenderTarget(sdlRend, nullptr);
        SDL_SetRenderDrawColor(sdlRend, 0xFF, 0x00, 0xFF, 0xFF);
        SDL_RenderClear(sdlRend);
        memset(framebuffer.data(), 0, framebuffer.size() * sizeof(uint32_t));
    }

    void Renderer::DrawFrame()
    {
        CastRaysBasic();
    }

    void Renderer::Render()
    {
        BeforeFrame();
        DrawFrame();
        // draw minimap
        SDL_RenderCopy(sdlRend, minimapTexture, nullptr, &leftRec);
        // draw player on minimap
        DrawPlayerMinimap();
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