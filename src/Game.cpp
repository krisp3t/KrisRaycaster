#include <SDL_events.h>
#include "Game.h"

namespace KrisRaycaster
{
    Game::Game(SDL_Renderer *sdlRend) : sdlRend(sdlRend)
    {
        systems.renderer = std::make_unique<Renderer>(sdlRend);
    }

    void Game::ProcessInput()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                hasQuit = true;
            }
        }
    }

    void Game::Update(double deltaTime)
    {

    }

    void Game::Render()
    {

    }

    bool Game::HasQuit() const
    {
        return hasQuit;
    }
}


