#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <cassert>
#include <sstream>
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
        TTF_Init();
        font = TTF_OpenFont("./assets/ttf/roboto-mono/RobotoMono-SemiBold.ttf", 24);
        if (!font)
        {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            return -1;
        }
        return 0;
    }


    size_t Renderer::CreateTexture(
            TextureFormat format
    )
    {
        auto t = Texture{format};
        items.push_back(t);
        return items.size() - 1;
    }

    size_t Renderer::CreateTexture(
            const std::string &filename,
            TextureFormat format
    )
    {
        auto t = Texture{filename, format};
        items.push_back(t);
        return items.size() - 1;
    }

    size_t Renderer::CreateTexture(
            const std::string &filename,
            TextureFormat format,
            SDL_Renderer &rend
    )
    {
        auto t = Texture{filename, format, rend};
        items.push_back(t);
        return items.size() - 1;
    }

    bool Renderer::InitMinimap(const std::string &mapPath, const std::vector<uint_fast8_t> &layout)
    {
        const int tileBorder = 1;
        // TODO: free wallTex
        auto wallTexId = CreateTexture(mapPath + "/wall.png",
                                       TextureFormat
                                               {32, 32, 256, 16, SDL_PIXELFORMAT_ABGR8888}, *sdlRend);
        Texture *wallTex = &items[wallTexId];
        minimapTexture = SDL_CreateTexture(
                sdlRend,
                SDL_PIXELFORMAT_ABGR8888,
                SDL_TEXTUREACCESS_TARGET,
                settings.framebufferWidth,
                settings.framebufferHeight
        );
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
                SDL_Rect src = wallTex->GetRect(tile);
                assert(wallTex->tx != nullptr);
                SDL_RenderCopy(sdlRend, wallTex->tx, &src, &dest);
            }
            ix++;
        }
        SDL_SetRenderTarget(sdlRend, nullptr);
        return true;
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
        SDL_SetRenderDrawColor(sdlRend, 0, 255, 0, 255);  // Green color
        SDL_RenderDrawLine(sdlRend,
                           static_cast<int>(dirRay.x - cameraRay.x),
                           static_cast<int>(dirRay.y - cameraRay.y),
                           static_cast<int>(dirRay.x + cameraRay.x),
                           static_cast<int>(dirRay.y + cameraRay.y)
        );
    }

    // Basic (slow) implementation - just go along the rays with increments until you hit wall
    void Renderer::CastRaysStep()
    {
        rays = std::vector<SDL_Point>{};
        rays.reserve(settings.framebufferWidth * 2);
        Map *map = Game::Get().systems.map.get();
        Vec2f playerPos = map->playerPos;
        Vec2f dir = map->dir;
        Vec2f cameraPlane = map->cameraPlane;
        int mapSize = Game::Get().systems.map->GetSize();
        int maxDistance = std::sqrt(mapSize * mapSize + mapSize * mapSize);
        for (int x = 0; x < settings.framebufferWidth; x++)
        {
            // if FOV=60 and fbWidth=720, then angle from [-30, 30] with increments of 60/720
            float cameraX = 2 * x / static_cast<float>(settings.framebufferWidth) - 1; // x in camera space [-1, 1]
            Vec2f rayDir = {dir.x + cameraPlane.x * cameraX,
                            dir.y + cameraPlane.y * cameraX};
            Vec2 start = MapToScreen(playerPos + rayDir, Vec2{settings.framebufferWidth, settings.framebufferHeight},
                                     mapSize);
            float distance = 1;
            Vec2f rayPos;
            int wall = 0;
            while (distance < maxDistance)
            {
                rayPos = playerPos + rayDir * distance;
                int mapX = static_cast<int>(rayPos.x);
                int mapY = static_cast<int>(rayPos.y);
                wall = map->Get(mapX, mapY);
                if (wall != 0)
                {
                    break;
                }
                distance += settings.rayIncrement;
            }
            Vec2 collisionPx = MapToScreen(rayPos,
                                           Vec2{settings.framebufferWidth, settings.framebufferHeight},
                                           mapSize);
            rays.push_back({start.x, start.y});
            rays.push_back({collisionPx.x, collisionPx.y});
            int wallHeight = floor(settings.framebufferHeight / fmax(distance, 1.0));
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

    void Renderer::CastRaysDDA()
    {
        rays = std::vector<SDL_Point>{};
        rays.reserve(settings.framebufferWidth * 2);
        Map *map = Game::Get().systems.map.get();
        Vec2f playerPos = map->playerPos;
        Vec2f dir = map->dir;
        Vec2f cameraPlane = map->cameraPlane;
        int mapSize = Game::Get().systems.map->GetSize();
        for (int screenCol = 0; screenCol < settings.framebufferWidth; screenCol++)
        {
            // if FOV=60 and fbWidth=720, then angle from [-30, 30] with increments of 60/720
            float cameraX = 2 * screenCol / static_cast<float>(settings.framebufferWidth) -
                            1; // screenCol in camera space [-1, 1]
            Vec2f rayDir = {dir.x + cameraPlane.x * cameraX,
                            dir.y + cameraPlane.y * cameraX};
            Vec2f start = playerPos;

            int wall = 0;
            float mapX, mapY;
            int stepX = rayDir.x > 0 ? 1 : -1;
            int stepY = rayDir.y > 0 ? 1 : -1;
            float xFactor = rayDir.x > 0 ?
                            (ceil(start.x) - start.x) / rayDir.x :
                            (floor(start.x) - start.x) / rayDir.x;
            float yFactor = rayDir.y > 0 ?
                            (ceil(start.y) - start.y) / rayDir.y :
                            (floor(start.y) - start.y) / rayDir.y;
            bool hitXSide = false; // true if hitYSide
            // DDA only in one direction, if completely horizontal / vertical ray
            if (rayDir.x == 0)
            {
                xFactor = 1e30; // will never be chosen
            }
            if (rayDir.y == 0)
            {
                yFactor = 1e30;
            }
            // TODO: prevent infinite loop?
            while (true)
            {
                if (xFactor < yFactor)
                {
                    mapX = playerPos.x + rayDir.x * xFactor;
                    mapY = playerPos.y + rayDir.y * xFactor;
                    xFactor += stepX / rayDir.x;
                    hitXSide = true;
                } else
                {
                    mapX = playerPos.x + rayDir.x * yFactor;
                    mapY = playerPos.y + rayDir.y * yFactor;
                    yFactor += stepY / rayDir.y;
                    hitXSide = false;
                }
                wall = map->Get(static_cast<int>(mapX), static_cast<int>(mapY));
                if (wall != 0)
                {
                    break;
                }
            }
            Vec2f collision = {mapX, mapY};
            Vec2 startPx = MapToScreen(start, Vec2{settings.framebufferWidth, settings.framebufferHeight}, mapSize);
            Vec2 collisionPx = MapToScreen(collision,
                                           Vec2{settings.framebufferWidth, settings.framebufferHeight},
                                           mapSize);
            float distance = (collision - start).Length();
            int wallHeight = floor(settings.framebufferHeight / fmax(distance, 1.0));
            rays.push_back({startPx.x, startPx.y});
            rays.push_back({collisionPx.x, collisionPx.y});

//            SDL_Log("Collision: (%f, %f), col: %d, wall: %x, factor: %f, distance: %f", collision.x, collision.y,
//                    screenCol, wall, hitXSide ? xFactor : yFactor, distance);
            // ceiling
            DrawVLine(screenCol, 0, settings.framebufferHeight / 2 - wallHeight / 2, 0xFF00FF00);
            // walls
            DrawVLine(screenCol, settings.framebufferHeight / 2 - wallHeight / 2, wallHeight, 0xFF0000FF);
            // floor
            DrawVLine(screenCol, settings.framebufferHeight / 2 + wallHeight / 2,
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
        //CastRaysStep();
        CastRaysDDA();
    }

    void Renderer::Render(double deltaTime)
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
        // display fps and resolution
        int fps = floor(1.0f / deltaTime);
        int time = floor(deltaTime * 1000);
        std::ostringstream stringStream;
        stringStream << fps << " fps (" << time << " ms)" << "\n" << settings.framebufferWidth << "x"
                     << settings.framebufferHeight;
        std::string copyOfStr = stringStream.str();
        SDL_Surface *message = TTF_RenderText_Solid(font, copyOfStr.c_str(), {255, 255, 255});
        SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(sdlRend, message);
        SDL_Rect messageRect = {settings.framebufferWidth + 16, 0, message->w, message->h};
        SDL_RenderCopy(sdlRend, messageTexture, nullptr, &messageRect);
        SDL_FreeSurface(message);
        SDL_DestroyTexture(messageTexture); // allocations every frame are slow
        SDL_RenderPresent(sdlRend);
    }

    Renderer::~Renderer()
    {
        SDL_DestroyTexture(framebufferTexture);
    }

}