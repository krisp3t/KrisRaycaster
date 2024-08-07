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
        Renderer::Get().InitMinimap(mapPath, data);
    }

    int Map::Get(size_t x, size_t y) const
    {
        assert(y * rowLength + x < data.size());
        return data[y * rowLength + x];
    }

    SDL_Rect *Map::GetTexCol(int spriteIx, int x) const
    {
        //return floorTexture->GetColumn(spriteIx, x, 0);
        return nullptr;
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
        playerPos = playerPos + dir * step;
    }

    void Map::StrafePlayer(float angle)
    {
        dir.Rotate(angle);
        cameraPlane.Rotate(angle);
    }
}