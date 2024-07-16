#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace KrisRaycaster
{
    struct Texture;
    struct Text;

    struct Map
    {

        Map(const Texture &texture,
            const std::string &layoutFilename);

        const Texture &texture;
        std::vector<uint_fast8_t> data; // map data
        size_t w, h; // overall map dimensions

        int Get(size_t i, size_t j) const;

        bool IsEmpty(size_t i, size_t j) const;
    };
}
