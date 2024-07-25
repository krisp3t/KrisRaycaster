#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

#include "Math.h"


namespace KrisRaycaster
{
    class Texture;

    struct Text;

    class Map
    {
    public:
        explicit Map(const std::string &mapPath);

        [[nodiscard]] int Get(size_t i, size_t j) const;

        [[nodiscard]] bool IsEmpty(size_t i, size_t j) const;

        int GetSize() const;


        Vec2f playerPos = {7, 14};
        Vec2f dir = {1, 0};
        Vec2f cameraPlane;
        Texture *floorTexture;
        std::shared_ptr<Texture> ceilTexture;
        Texture *minimapTexture;

        void MovePlayer(float step);

        void StrafePlayer(float angle);

        Vec2f GetPlayer();


    private:
        int rowLength;
        std::vector<uint_fast8_t> data; // map data

        void Render();

    };
}
