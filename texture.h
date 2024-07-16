#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace KrisRaycaster
{
    struct TextureFormat
    {
        size_t img_w, img_h; // sprite size
        size_t count; // number of textures and size in pixels
        uint32_t format; // texture format
    };

    struct Texture
    {
        Texture(const std::string &filename, TextureFormat format);

        TextureFormat format;
        std::vector<uint32_t> img; // textures storage container
    };
}