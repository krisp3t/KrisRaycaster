#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

#include "Math.h"
#include "SDL_rect.h"


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
        Vec2f dir = {-1, 0};
        Vec2f cameraPlane = {0, 0.66}; // 2 * atan(0.66) = FOV 60
        Texture *floorTexture;
        std::shared_ptr<Texture> ceilTexture;
        Texture *minimapTexture;

        void MovePlayer(float step);

        void StrafePlayer(float angle);

        Vec2f GetPlayer();

        SDL_Rect *GetTexCol(int spriteIx, int x) const;

        uint32_t GetTex(int spriteIx) const;

    private:
        int rowLength;
        std::vector<uint_fast8_t> data; // map data

        void Render();

    };
}
