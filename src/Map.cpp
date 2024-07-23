#include "Map.h"
#include "Texture.h"
#include "Text.h"
#include "Renderer.h"

namespace KrisRaycaster
{
    Map::Map(const std::string &mapPath)
    {
        data = ReadBytes(mapPath + "/layout.txt");
        if (data.empty())
        {
            std::cerr << "Failed to read map layout file: " << mapPath << std::endl;
        }
        // TODO: don't hardcode
        floorTexture = std::make_shared<Texture>(
                Renderer::Get().CreateTexture(mapPath + "/wall.png", {256, 256, 64})
        );
        minimapTexture = std::make_shared<Texture>(
                Renderer::Get().CreateTexture({256, 256, 64})
        );

        for (uint_fast8_t tile: data)
        {
            if (tile == 0)
            {
                continue;
            }
        }
        SDL_Rect r = floorTexture->GetRect(tile);

    }

    int Map::Get(size_t i, size_t j) const
    {
        return 0;
    }

    bool Map::IsEmpty(size_t i, size_t j) const
    {
        return false;
    }

    void Map::Render()
    {

    }
}