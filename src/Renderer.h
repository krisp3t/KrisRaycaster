#pragma once

#include <SDL.h>
#include "Texture.h"

namespace KrisRaycaster
{

    struct RendererSettings
    {
        int framebufferWidth = 1280;
        int framebufferHeight = 720;
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

        KrisRaycaster::Texture CreateTexture(const std::string &filename, KrisRaycaster::TextureFormat format);

        Texture CreateTexture(TextureFormat format);

    private:
        Renderer() = default;

        RendererSettings settings;
        SDL_Renderer *sdlRend;
        SDL_Texture *framebufferTexture;

        void BeforeFrame();

        void DrawFrame();


        Texture CreateEmptyTexture(TextureFormat format);

    };
}