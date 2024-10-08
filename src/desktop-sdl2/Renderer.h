#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include "Math.h"

namespace KrisRaycaster
{
    class Texture;

    struct TextureFormat;

    struct RendererSettings
    {
        int screenWidth = 1280;
        int screenHeight = 720;
        int framebufferWidth = screenWidth / 2;
        int framebufferHeight = screenHeight;
        float rayIncrement = 0.01;
    };

    class Renderer
    {
    public:
        ~Renderer();

        static Renderer &Get();

        Renderer(const Renderer &) = delete;

        Renderer &operator=(const Renderer &) = delete;

        int Init(SDL_Renderer *rend);

        void Render(double d);

        void ResizeWindow(const int width, const int height);

        size_t CreateTexture(const std::string &filename, TextureFormat format);

        size_t CreateTexture(TextureFormat format);

        bool InitMinimap(const std::string &mapPath, const std::vector<uint_fast8_t> &layout);

    private:
        Renderer() = default;

        RendererSettings settings;
        SDL_Renderer *sdlRend;
        std::vector<Texture> items; // TODO: remove storage
        std::vector<uint32_t> framebuffer;
        std::vector<SDL_Point> rays;
        SDL_Texture *framebufferTexture;
        SDL_Texture *minimapTexture;
        TTF_Font *font;


        void BeforeFrame();

        void DrawFrame();

        void AfterFrame();

        SDL_Rect leftRec;
        SDL_Rect rightRec;

        void CastRaysStep();

        void CastRaysDDA();

        void DrawVLine(int x, int y, int height, uint32_t color);
    	void DrawVLine(int x, int y, int height, uint32_t fromColor, uint32_t toColor);
        void DrawVLine(int x, int y, int height, int wallType, float collisionAt, float brightness);

        void DrawHLines(int x, int y, int height, uint32_t fromColor, uint32_t toColor);


        void DrawPlayerMinimap();

        size_t CreateTexture(const std::string &filename, TextureFormat format, SDL_Renderer &rend);
    };
}