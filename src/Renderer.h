#pragma once

#include <SDL.h>
#include <vector>
#include "Math.h"

namespace KrisRaycaster
{
    class Texture;

    class TextureFormat;

    struct RendererSettings
    {
        int screenWidth = 1280;
        int screenHeight = 720;
        int framebufferWidth = screenWidth / 2;
        int framebufferHeight = screenHeight;
        float fov = 60.0;
        int mapWidth;
        int mapHeight;
        int tileSize;
        float rayIncrementAngle = fov / framebufferWidth;
        int rayPrecision = 64;
        double depth;
        Vec2 playerPos = {7, 14}; // TODO: move out of here!
        float playerAngle = 0.0; // TODO: move out of here!
    };

    class Renderer
    {
    public:


        ~Renderer();

        static Renderer &Get();

        Renderer(const Renderer &) = delete;

        Renderer &operator=(const Renderer &) = delete;

        int Init(SDL_Renderer *rend);


        void Render();

        void Update();

        bool ProcessInput();

        Texture *CreateTexture(const std::string &filename, TextureFormat format);

        Texture *CreateTexture(TextureFormat format);

        bool InitMinimap(const Texture &floorTex, const std::vector<uint_fast8_t> &layout);


    private:
        Renderer() = default;

        RendererSettings settings;
        SDL_Renderer *sdlRend;
        std::vector<uint32_t> framebuffer;
        SDL_Texture *framebufferTexture;
        SDL_Texture *minimapTexture;
        std::vector<Texture> items;

        void BeforeFrame();

        void DrawFrame();

        SDL_Rect leftRec;
        SDL_Rect rightRec;


        void CastRaysBasic();

        void DrawVLine(int x, int height);
    };
}