#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace KrisRaycaster
{
    struct Texture
    {
        size_t img_w, img_h; // overall map dimensions
        size_t count, size; // number of textures and size in pixels
        std::vector<uint32_t> img; // textures storage container
        Texture(std::string filename, uint32_t format);
    };
}