#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "Math.h"


namespace KrisRaycaster
{
    struct Texture;
    struct Text;

    class Map
    {

        Map(const Texture &texture,
            const std::string &layoutFilename);

        [[nodiscard]] int Get(size_t i, size_t j) const;

        [[nodiscard]] bool IsEmpty(size_t i, size_t j) const;

        Vec2f playerPos;
        Vec2f dir;
        Vec2f cameraPlane;

    private:
        int w, h;
        std::vector<uint_fast8_t> data; // map data
        const Texture &texture;

    };
}
