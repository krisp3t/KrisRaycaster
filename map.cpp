#include "map.h"
#include "texture.h"
#include "text.h"

namespace KrisRaycaster
{
    Map::Map(const Texture &texture, const std::string &layoutFilename) : texture(texture)
    {
        data = ReadBytes(layoutFilename);
        if (data.empty())
        {
            std::cerr << "Failed to read map layout file: " << layoutFilename << std::endl;
        }
    }

    int Map::Get(size_t i, size_t j) const
    {
        return 0;
    }

    bool Map::IsEmpty(size_t i, size_t j) const
    {
        return false;
    }
}