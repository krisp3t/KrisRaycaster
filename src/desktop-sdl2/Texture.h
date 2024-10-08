#pragma once

#include "SDL2/SDL_render.h"
#include <vector>
#include <cstdint>
#include <string>
#include "Renderer.h"

namespace KrisRaycaster
{
    struct TextureFormat
    {
        int spriteW, spriteH; // sprite size
        int count;            // number of textures and size in pixels
        int rowCount;         // number of textures in a row
        uint32_t format;      // texture format // TODO: rewrite to use SDL_PixelFormatEnum
        SDL_Rect rect;
    };
    struct ScreenPos
    {
        int x, y;
        int w, h;
        int zIndex;
    };

    class Texture
    {
    public:
        Texture(
            const std::string &filename,
            TextureFormat format);

        Texture(TextureFormat format);

        Texture(
            TextureFormat format,
            SDL_Renderer &rend);

        Texture(const std::string &filename, TextureFormat format, SDL_Renderer &rend);

        ~Texture();

        TextureFormat format;
        // TODO: rewrite to use SDL_Texture or vector<uint32_t>
        // std::vector<uint32_t> img; // textures storage container

        [[nodiscard]] SDL_Rect GetRect(int ix) const;


        SDL_Texture *tx = nullptr;
        SDL_Surface *surf = nullptr; // TODO: private
        uint32_t Get() const;
    };
}