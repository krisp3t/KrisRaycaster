#include <iostream>
#include <memory>
#include <SDL.h>

#include "Renderer.h"
#include "Game.h"

namespace KrisRaycaster
{
    constexpr int INIT_FRAMEBUFFER_WIDTH = 1280;
    constexpr int INIT_FRAMEBUFFER_HEIGHT = 720;

    bool ProcessInput()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return true;
            }
        }
        return false;
    }

    void Update()
    {
        return;
    }

    void Render()
    {

        return;
    }

    int main(int argc, char *args[])
    {
        if (SDL_Init(SDL_INIT_VIDEO))
        {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
            return -1;
        }

        SDL_Window *window = nullptr;
        SDL_Renderer *sdlRend = nullptr;
        if (SDL_CreateWindowAndRenderer(
                INIT_FRAMEBUFFER_WIDTH,
                INIT_FRAMEBUFFER_HEIGHT,
                SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_RESIZABLE,
                &window,
                &sdlRend)
                )
        {
            std::cerr << "Failed to create window and renderer: " << SDL_GetError() << std::endl;
            return -1;
        }
        SDL_SetWindowTitle(window, "KrisRaycaster");
        Renderer &renderer = Renderer::Get();
        renderer.Init(sdlRend);
        Uint32 oldTime = 0, time = 0;
        double deltaTime;
        std::unique_ptr<Game> game = std::make_unique<Game>();
        while (!game->HasQuit())
        {
            time = SDL_GetTicks();
            game->ProcessInput();
            deltaTime = (time - oldTime) / 1000.0;
            game->Update(deltaTime);
            game->Render();
            oldTime = time;

            // Cap frame rate (optional)
            if (deltaTime < (1.0f / 60.0f))
            {
                SDL_Delay((Uint32) (((1.0f / 60.0f) - deltaTime) * 1000.0f));
            }
        }
        SDL_DestroyRenderer(sdlRend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }
}

int main(int argc, char *argv[])
{
    return KrisRaycaster::main(argc, argv);
}