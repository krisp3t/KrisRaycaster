#pragma once

#include <memory>
#include "Map.h"
#include "Renderer.h"

struct SDL_Renderer;
namespace KrisRaycaster
{
    class Renderer;

    class Map;


    struct GameSystems
    {
        std::unique_ptr<Renderer> renderer;
        std::unique_ptr<Map> map;
        //std::unique_ptr<Text> text;
    };

    class Game
    {
    public:
        Game(SDL_Renderer *sdlRend);

        void ProcessInput();

        void Update(double deltaTime);

        void Render();

        [[nodiscard]] bool HasQuit() const;


    private:
        GameSystems systems;
        SDL_Renderer *sdlRend;
        bool hasQuit = false;
    };
}
