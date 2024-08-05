#include <iostream>
#include <cassert>
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_image.h"
#include "Texture.h"

namespace KrisRaycaster
{
    // TODO: don't hardcode target
    Texture::Texture(
            TextureFormat format,
            SDL_Renderer *r
    ) : format(format)
    {
        img = SDL_CreateRGBSurfaceWithFormat(
                0,
                format.spriteW * format.rowCount,

                format.spriteH * format.count / format.rowCount,
                32,
                format.format);
        if (!img)
        {
            std::cerr << "Failed to create texture : " << SDL_GetError() << std::endl;
            return;
        }
    }

    Texture::Texture(
            const std::string &filename,
            TextureFormat format,
            SDL_Renderer *r
    ) : format(format)
    {
        img = IMG_Load(filename.c_str());
        if (!img)
        {
            std::cerr << "Error in IMG_Load: " << IMG_GetError() << std::endl;
            return;
        }
    }

    Texture::~Texture()
    {
        // TODO: Destroy img

    }

    SDL_Rect Texture::GetRect(int ix) const
    {
        int i = ix - 1; // 0 means empty space, 1 is first sprite in layout
        assert(i >= 0);
        assert(i < format.count);
        SDL_Rect rect;
        rect.x = (i % format.rowCount) * format.spriteW;
        rect.y = (i / format.rowCount) * format.spriteH;
        rect.w = format.spriteW;
        rect.h = format.spriteH;
        return rect;
    }
}