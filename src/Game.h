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
        //std::unique_ptr<Renderer> renderer;
        std::unique_ptr<Map> map;
        //std::unique_ptr<Text> text;
    };

    class Game
    {
    public:
        Game();

        Game(const Game &) = delete;

        Game &operator=(const Game &) = delete;

        static Game &Get();

        void ProcessInput();

        void Update(double deltaTime);

        void Render();

        [[nodiscard]] bool HasQuit() const;


        GameSystems systems;
    private:
        bool hasQuit = false;
    };
}
