#include "map.h"
#include "texture.h"
#include "text.h"

namespace KrisRaycaster
{
    Map::Map(Texture &texture, Text &text) : texture(texture)
    {
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