#pragma once

#include "SDL2/SDL_render.h"
#include <vector>
#include <cstdint>
#include <string>

namespace KrisRaycaster
{
    struct TextureFormat
    {
        int imgW, imgH; // sprite size
        int count; // number of textures and size in pixels
        int rowSize; // number of textures in a row
        uint32_t format; // texture format // TODO: rewrite to use SDL_PixelFormatEnum
    };

    struct Texture
    {
        Texture(
                const std::string &filename,
                TextureFormat format,
                SDL_Renderer &renderer
        );

        ~Texture();

        TextureFormat format;
        // TODO: rewrite to use SDL_Texture or vector<uint32_t>
        SDL_Texture *img;
        // std::vector<uint32_t> img; // textures storage container

        [[nodiscard]] SDL_Rect GetRect(int ix) const;

        [[nodiscard]] SDL_Rect GetColumn(int textureIx, int i, int j) const;
    };
}