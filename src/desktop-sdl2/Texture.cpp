#include <iostream>
#include <cassert>
#include "SDL2/SDL.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_image.h"
#include "Texture.h"

namespace KrisRaycaster
{
    Texture::Texture(
            TextureFormat format
    ) : format(format)
    {
        surf = SDL_CreateRGBSurfaceWithFormat(
                0,
                format.spriteW * format.rowCount,

                format.spriteH * format.count / format.rowCount,
                32,
                format.format);
        if (!surf)
        {
            std::cerr << "Failed to create texture : " << SDL_GetError() << std::endl;
            return;
        }
    }

    Texture::Texture(
            const std::string &filename,
            TextureFormat format
    ) : format(format)
    {
        surf = IMG_Load(filename.c_str());
        if (!surf)
        {
            std::cerr << "Error in IMG_Load: " << IMG_GetError() << std::endl;
            return;
        }
    }

    Texture::Texture(
            const std::string &filename,
            TextureFormat format,
            SDL_Renderer &rend
    ) : format(format)
    {
        tx = IMG_LoadTexture(&rend, filename.c_str());
        if (!tx)
        {
            std::cerr << "Error in IMG_LoadTexture: " << IMG_GetError() << std::endl;
            return;
        }
    }

    Texture::Texture(
            TextureFormat format,
            SDL_Renderer &rend
    ) : format(format)
    {
        tx = SDL_CreateTexture(
                &rend,
                format.format,
                SDL_TEXTUREACCESS_STREAMING,
                format.spriteW,
                format.spriteH);
        if (!tx)
        {
            std::cerr << "Failed to create texture : " << SDL_GetError() << std::endl;
            return;
        }
    }

    Texture::~Texture()
    {
        //SDL_FreeSurface(surf);
        // TODO: figure out when freed
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