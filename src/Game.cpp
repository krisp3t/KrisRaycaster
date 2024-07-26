#include <SDL_events.h>
#include "Game.h"

namespace KrisRaycaster
{
    Game &Game::Get()
    {
        static Game instance;
        return instance;
    }


    void Game::ProcessInput(double deltaTime)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                hasQuit = true;
            }
        }
        const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_W])
        {
            systems.map->MovePlayer(3 * deltaTime);
        }
        if (currentKeyStates[SDL_SCANCODE_S])
        {
            systems.map->MovePlayer(-3 * deltaTime);
        }
        if (currentKeyStates[SDL_SCANCODE_A])
        {
            systems.map->StrafePlayer(-5 * deltaTime);
        }
        if (currentKeyStates[SDL_SCANCODE_D])
        {
            systems.map->StrafePlayer(5 * deltaTime);
        }
    }

    void Game::Update(double deltaTime)
    {

    }

    void Game::Render(double deltaTime)
    {
        Renderer::Get().Render(deltaTime); // TODO: avoid static call?
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


