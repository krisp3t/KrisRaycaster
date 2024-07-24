#pragma once

#include <SDL.h>
#include <vector>

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
        int fov;
        int mapWidth;
        int mapHeight;
        int tileSize;
        int numRays;
        double depth;
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
        SDL_Texture *framebufferTexture;
        SDL_Texture *minimapTexture;
        std::vector<Texture> items;

        void BeforeFrame();

        void DrawFrame();

        SDL_Rect leftRec;
        SDL_Rect rightRec;


    };
}