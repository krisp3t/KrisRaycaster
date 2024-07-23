#pragma once

#include <SDL.h>

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
        Renderer(SDL_Renderer *renderer);

        ~Renderer();

        int Init();

        void Render();

        void Update();

        bool ProcessInput();

    private:
        RendererSettings settings;
        SDL_Renderer *sdlRend;
        SDL_Texture *framebufferTexture;
    };
}