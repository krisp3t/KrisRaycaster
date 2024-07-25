#include <cassert>
#include "Map.h"
#include "Texture.h"
#include "Text.h"
#include "Renderer.h"
#include "Game.h"

namespace KrisRaycaster
{
    Map::Map(const std::string &mapPath)
    {
        data = ReadBytes(mapPath + "/layout.txt");
        if (data.empty())
        {
            std::cerr << "Failed to read map layout file: " << mapPath << std::endl;
        }
        rowLength = floor(sqrt(data.size()));
        // TODO: don't hardcode
        floorTexture = Renderer::Get().CreateTexture(mapPath + "/wall.png",
                                                     TextureFormat{32, 32, 256, 16, SDL_PIXELFORMAT_ABGR8888});
        Renderer::Get().InitMinimap(*floorTexture, data);
    }

    int Map::Get(size_t x, size_t y) const
    {
        assert(y * rowLength + x < data.size());
        return data[y * rowLength + x];
    }

    int Map::GetSize() const
    {
        return rowLength;
    }

    bool Map::IsEmpty(size_t i, size_t j) const
    {
        return false;
    }

    Vec2f Map::GetPlayer()
    {
        return playerPos;
    }

    void Map::MovePlayer(float step)
    {
        playerPos.x += dir.y * step;
        playerPos.y += dir.x * step;
    }

    void Map::StrafePlayer(float angle)
    {
        dir = dir * angle;
    }
}