#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

#include "Math.h"


namespace KrisRaycaster
{
    struct Texture;
    struct Text;

    class Map
    {
    public:
        explicit Map(const std::string &mapPath);

        [[nodiscard]] int Get(size_t i, size_t j) const;

        [[nodiscard]] bool IsEmpty(size_t i, size_t j) const;

        Vec2f playerPos;
        Vec2f dir;
        Vec2f cameraPlane;
        std::shared_ptr<Texture> floorTexture;
        std::shared_ptr<Texture> ceilTexture;
        std::shared_ptr<Texture> minimapTexture;
    private:
        int w, h;
        std::vector<uint_fast8_t> data; // map data

        void Render();
    };
}
