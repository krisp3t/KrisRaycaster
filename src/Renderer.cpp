#include <SDL_ttf.h>
#include <cassert>
#include <sstream>
#include "Renderer.h"

#include "Math.h"
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
            settings.framebufferHeight);
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

        // TODO: come up with better texture architecture
        auto wallSurfId = CreateTexture("./map/minecraft/wall.png",
                                        TextureFormat{32, 32, 256, 16, SDL_PIXELFORMAT_ABGR8888});
        Map *map = Game::Get().systems.map.get();
        map->wallTexId = wallSurfId;
        return 0;
    }

    size_t Renderer::CreateTexture(
        TextureFormat format)
    {
        items.push_back(Texture{format});
        return items.size() - 1;
    }

    size_t Renderer::CreateTexture(
        const std::string &filename,
        TextureFormat format)
    {
        items.push_back(Texture{filename, format});
        return items.size() - 1;
    }

    size_t Renderer::CreateTexture(
        const std::string &filename,
        TextureFormat format,
        SDL_Renderer &rend)
    {
        items.push_back(Texture{filename, format, rend});
        return items.size() - 1;
    }

    bool Renderer::InitMinimap(const std::string &mapPath, const std::vector<uint_fast8_t> &layout)
    {
        const int tileBorder = 1;
        // TODO: free wallTex
        // TODO: don't assume sprite size!
        auto wallTexId = CreateTexture(mapPath + "/wall.png",
                                       TextureFormat{32, 32, 256, 16, SDL_PIXELFORMAT_ABGR8888}, *sdlRend);
        Texture *wallTex = &items[wallTexId];
        minimapTexture = SDL_CreateTexture(
            sdlRend,
            SDL_PIXELFORMAT_ABGR8888,
            SDL_TEXTUREACCESS_TARGET,
            settings.framebufferWidth,
            settings.framebufferHeight);
        SDL_SetRenderTarget(sdlRend, minimapTexture);
        int ix = 0;
        int mapSize = layout.size();
        int mapWidth = sqrt(mapSize);
        Vec2 tileSize{settings.framebufferWidth / mapWidth, settings.framebufferHeight / mapWidth};
        for (uint_fast8_t tile : layout)
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
            }
            else
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
            map->playerPos.y * settings.framebufferHeight / mapSize};

        // Draw player position (as a small circle or rectangle)
        SDL_SetRenderDrawColor(sdlRend, 255, 0, 0, 255); // Red color
        SDL_Rect playerRect = {
            static_cast<int>(playerPos.x),
            static_cast<int>(playerPos.y),
            playerSize, playerSize};
        SDL_RenderFillRect(sdlRend, &playerRect);

        // Draw rays
        SDL_SetRenderDrawColor(sdlRend, 0, 0, 255, 255); // Blue color
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
                           static_cast<int>(dirRay.y));
        // Draw camera plane
        SDL_SetRenderDrawColor(sdlRend, 0, 255, 0, 255); // Green color
        SDL_RenderDrawLine(sdlRend,
                           static_cast<int>(dirRay.x - cameraRay.x),
                           static_cast<int>(dirRay.y - cameraRay.y),
                           static_cast<int>(dirRay.x + cameraRay.x),
                           static_cast<int>(dirRay.y + cameraRay.y));
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
        Vec2f dir = map->dir;                 // direction vector of player
        Vec2f cameraPlane = map->cameraPlane; // perpendicular to dir, magnitude defines FOV
        int mapSize = Game::Get().systems.map->GetSize();
        for (int screenCol = 0; screenCol < settings.framebufferWidth; screenCol++)
        {
            int wallType = 0;
            // if FOV=60 and fbWidth=720, then angle from [-30, 30] with increments of 60/720
            float cameraX = 2 * screenCol / static_cast<float>(settings.framebufferWidth) -
                            1; // screenCol in camera space [-1, 1]
            Vec2f rayDir = {dir.x + cameraPlane.x * cameraX,
                            dir.y + cameraPlane.y * cameraX};
            Vec2 step = {
                rayDir.x > 0 ? 1 : -1,
                rayDir.y > 0 ? 1 : -1};
            bool isHitVertical = false;
            Vec2 tile = {
                static_cast<int>(playerPos.x),
                static_cast<int>(playerPos.y)};
            // We have a direction of the ray for one screen column.
            // 1. Figure out the scaling factor of rayDir so that x side == 1 / so that y side == 1.
            Vec2f deltaFactor = {std::abs(1.0f / rayDir.x), std::abs(1.0f / rayDir.y)};
            // 2. Figure out how much we need to travel to get to next x and y side.
            Vec2f sideDist = {};
            sideDist.x = rayDir.x > 0 ? (tile.x - playerPos.x + 1.0f) * deltaFactor.x : (playerPos.x - tile.x) * deltaFactor.x;
            sideDist.y = rayDir.y > 0 ? (tile.y - playerPos.y + 1.0f) * deltaFactor.y : (playerPos.y - tile.y) * deltaFactor.y;
            // 3. DDA - one step in x/y direction (whichever has shorter ray magnitude - scaling factor) until you hit a wall
            Vec2f totalDist = sideDist;
            while (true)
            {
                assert(tile.x >= 0 && tile.x < mapSize && tile.y >= 0 && tile.y < mapSize && "Map layout must be closed off at edges!");
                if (totalDist.x < totalDist.y)
                {
                    totalDist.x += deltaFactor.x;
                    tile.x += step.x;
                    isHitVertical = false;
                }
                else
                {
                    totalDist.y += deltaFactor.y;
                    tile.y += step.y;
                    isHitVertical = true;
                }
                wallType = map->Get(tile.x, tile.y);
                if (wallType != 0)
                {
                    break;
                }
            }
            // 4. When collision found, calculate distance to wall.
            // Subtract one tile, because collision actually happens at edge of wall (position + size of tile).
            float distance = isHitVertical ? totalDist.y - deltaFactor.y : totalDist.x - deltaFactor.x;
            int wallHeight = floor(settings.framebufferHeight / fmax(distance, 1.0));
            // Vec2 startPx = MapToScreen(playerPos, Vec2{settings.framebufferWidth, settings.framebufferHeight}, mapSize);
            // Vec2 collisionPx = MapToScreen(map,
            //                                Vec2{settings.framebufferWidth, settings.framebufferHeight},
            //                                mapSize);p
            // rays.push_back({startPx.x, startPx.y});
            // rays.push_back({collisionPx.x, collisionPx.y});
            // SDL_Log("Collision: (%f, %f), col: %d, wall: %x, distance: %f", tile.x, tile.y,
            //         screenCol, wallType, distance);
            float collisionAt = isHitVertical ? playerPos.x + rayDir.x * distance : playerPos.y + rayDir.y * distance;
            collisionAt = collisionAt - floor(collisionAt); // [0.0f, 1.0f]
            // brightness at distance 1 max, across whole map almost 0
            float brightness = pow(((mapSize + 1) - distance) / mapSize, 2);

            // ceiling
            DrawVLine(screenCol, 0, settings.framebufferHeight / 2, 0x9C8255, 0xFF3E1016);
            // floor
            DrawVLine(screenCol, settings.framebufferHeight / 2,
                      settings.framebufferHeight / 2, 0xFF000000, 0xFF110E1A);
            // walls
            DrawVLine(screenCol, settings.framebufferHeight / 2 - wallHeight / 2, wallHeight, wallType, collisionAt, brightness);
        }
    }

    void Renderer::DrawVLine(int x, int y, int height, uint32_t color)
    {
        if (height <= 0)
        {
            return;
        }
        assert((y >= 0 && y < settings.framebufferHeight));
        assert(height <= settings.framebufferHeight);
        assert(x >= 0 && x < settings.framebufferWidth);
        for (int i = 0; i < height; i++)
        {
            int iy = y + i;
            framebuffer[iy * settings.framebufferWidth + x] = color;
        }
    }

    void Renderer::DrawVLine(int x, int y, int height, uint32_t fromColor, uint32_t toColor)
    {
        constexpr short stepSize = 4;
        if (height <= 0)
        {
            return;
        }
        assert((y >= 0 && y < settings.framebufferHeight));
        assert(height <= settings.framebufferHeight);
        assert(x >= 0 && x < settings.framebufferWidth);

        uint8_t fromA = (fromColor >> 24) & 0xFF;
        uint8_t fromB = (fromColor >> 16) & 0xFF;
        uint8_t fromG = (fromColor >> 8) & 0xFF;
        uint8_t fromR = fromColor & 0xFF;

        uint8_t toA = (toColor >> 24) & 0xFF;
        uint8_t toB = (toColor >> 16) & 0xFF;
        uint8_t toG = (toColor >> 8) & 0xFF;
        uint8_t toR = toColor & 0xFF;

        uint32_t currentColor = fromColor;

        for (int i = 0; i < height; i++)
        {
            if (i % stepSize == 0)
            {
                const float t = static_cast<float>(i) / (height - 1);

                // Linear interpolation
                uint8_t a = static_cast<uint8_t>((1.0f - t) * fromA + t * toA);
                uint8_t b = static_cast<uint8_t>((1.0f - t) * fromB + t * toB);
                uint8_t g = static_cast<uint8_t>((1.0f - t) * fromG + t * toG);
                uint8_t r = static_cast<uint8_t>((1.0f - t) * fromR + t * toR);

                currentColor = (a << 24) | (b << 16) | (g << 8) | r;
            }
            int iy = y + i;
            framebuffer[iy * settings.framebufferWidth + x] = currentColor;
        }
    }

    void Renderer::DrawVLine(int x, int y, int height, int wallType, float collisionAt, float brightness)
    {
        assert((y >= 0 && y < settings.framebufferHeight) || (height == 0));
        assert(height >= 0 && height <= settings.framebufferHeight);
        assert(x >= 0 && x < settings.framebufferWidth || (height == 0));
        Map *map = Game::Get().systems.map.get();
        Texture *wallSurf = &items[map->wallTexId];
        int spriteW = wallSurf->format.spriteW;
        int spriteH = wallSurf->format.spriteH;
        int spriteRow = wallType / wallSurf->format.rowCount;
        int spriteCol = wallType % wallSurf->format.rowCount - 1;
        int rowPixels = wallSurf->format.rowCount * spriteW;
        Vec2 spriteStart = {
            spriteCol * spriteW + static_cast<int>(collisionAt * spriteW),
            spriteRow * rowPixels};
        uint32_t *px = static_cast<uint32_t *>(wallSurf->surf->pixels); // TODO: separate function, boundary checks...
        px += spriteStart.y * spriteW + spriteStart.x;
        float texStep = static_cast<float>(spriteH) / static_cast<float>(height);
        for (int i = 0; i < height; i++)
        {
            int iy = y + i;
            uint32_t texColor = *(px + rowPixels * static_cast<int>(i * texStep));
            framebuffer[iy * settings.framebufferWidth + x] = ApplyBrightnessAbgr(texColor, brightness);
            // TODO: transpose framebuffer?
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
        // CastRaysStep();
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
        stringStream << fps << " fps (" << time << " ms)" << "\n"
                     << settings.framebufferWidth << "x"
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