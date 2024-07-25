#include <SDL_events.h>
#include "Game.h"

namespace KrisRaycaster
{
    Game &Game::Get()
    {
        static Game instance;
        return instance;
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
        Renderer::Get().Render(); // TODO: avoid static call?
    }

    bool Game::HasQuit() const
    {
        return hasQuit;
    }

    Game::Game()
    {
        systems.map = std::make_unique<Map>("./map/minecraft");
    }


}


