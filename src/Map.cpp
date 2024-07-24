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
        floorTexture = Renderer::Get().CreateTexture(mapPath + "/wall.png",
                                                     TextureFormat{256, 256, 64, 16, SDL_PIXELFORMAT_ABGR8888});
        Renderer::Get().InitMinimap(*floorTexture, data);
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