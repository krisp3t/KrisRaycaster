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
        img = SDL_CreateTexture(
                r,
                SDL_PIXELFORMAT_ABGR8888,
                SDL_TEXTUREACCESS_TARGET,
                format.imgW,
                format.imgH
        );
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
        img = IMG_LoadTexture(r, filename.c_str());
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
        assert(ix < format.count);
        SDL_Rect rect;
        rect.x = (ix % format.rowSize) * format.imgW;
        rect.y = (ix / format.rowSize) * format.imgH;
        rect.w = format.imgW;
        rect.h = format.imgH;
        return rect;
    }


}