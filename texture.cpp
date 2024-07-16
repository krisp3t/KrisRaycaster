#include <iostream>
#include <SDL_surface.h>
#include <SDL_image.h>
#include "texture.h"

namespace KrisRaycaster
{
    Texture::Texture(const std::string &filename, TextureFormat format) : format(format)
    {
        SDL_Surface *tmp = SDL_LoadBMP(filename.c_str());
        if (!tmp)
        {
            std::cerr << "Error in SDL_LoadBMP: " << SDL_GetError() << std::endl;
            return;
        }
    }
}