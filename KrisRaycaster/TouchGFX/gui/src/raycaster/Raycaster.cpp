#include <cstring>
#include <cmath>
#include <gui/raycaster/Raycaster.hpp>
#include <gui/common/globals.hpp>
#include "touchgfx/Utils.hpp"

Vec2f Raycaster::playerPos = {2, 11};
Vec2f Raycaster::dir = {0, -1};
Vec2f Raycaster::cameraPlane = {0.66f, 0};

using namespace KrisRaycaster;

void Raycaster::render(uint8_t *framebuffer)
{
    auto fb = reinterpret_cast<uint16_t *>(framebuffer);
    // ceiling
    drawHLines(fb, 0, 0, KrisRaycaster::SCREEN_WIDTH, KrisRaycaster::SCREEN_HEIGHT / 2, 0xf800, 0xf7e0);
    // floor
    drawHLines(fb, 0, KrisRaycaster::SCREEN_HEIGHT / 2, KrisRaycaster::SCREEN_WIDTH, KrisRaycaster::SCREEN_HEIGHT / 2, 0x102c, 0x0841);

    Vec2f playerPos = Raycaster::playerPos;
    Vec2f dir = Raycaster::dir;                 // direction vector of player
    Vec2f cameraPlane = Raycaster::cameraPlane; // perpendicular to dir, magnitude defines FOV
    int mapSize = MAP_SIDE;
    for (int screenCol = 0; screenCol < SCREEN_WIDTH; screenCol++)
    {
        int wallType = 0;
        // if FOV=60 and fbWidth=720, then angle from [-30, 30] with increments of 60/720
        float cameraX = 2 * screenCol / static_cast<float>(SCREEN_WIDTH) -
            1; // screenCol in camera space [-1, 1]
        Vec2f rayDir = { dir.x + cameraPlane.x * cameraX,
                        dir.y + cameraPlane.y * cameraX };
        Vec2 step = {
            rayDir.x > 0 ? 1 : -1,
            rayDir.y > 0 ? 1 : -1 };
        bool isHitVertical = false;
        Vec2 tile = {
            static_cast<int>(playerPos.x),
            static_cast<int>(playerPos.y) };
        // We have a direction of the ray for one screen column.
        // 1. Figure out the scaling factor of rayDir so that x side == 1 / so that y side == 1.
        Vec2f deltaFactor = { std::abs(1.0f / rayDir.x), std::abs(1.0f / rayDir.y) };
        // 2. Figure out how much we need to travel to get to next x and y side.
        Vec2f sideDist = {};
        sideDist.x = rayDir.x > 0 ? (tile.x - playerPos.x + 1.0f) * deltaFactor.x : (playerPos.x - tile.x) * deltaFactor.x;
        sideDist.y = rayDir.y > 0 ? (tile.y - playerPos.y + 1.0f) * deltaFactor.y : (playerPos.y - tile.y) * deltaFactor.y;
        // 3. DDA - one step in x/y direction (whichever has shorter ray magnitude - scaling factor) until you hit a wall
        Vec2f totalDist = sideDist;
        while (true)
        {
            assert(tile.x >= 0 && tile.x < mapSize && tile.y >= 0 && tile.y < mapSize && "Map layout must be closed off at edges!");
            if (totalDist.x < totalDist.y)
            {
                totalDist.x += deltaFactor.x;
                tile.x += step.x;
                isHitVertical = false;
            }
            else
            {
                totalDist.y += deltaFactor.y;
                tile.y += step.y;
                isHitVertical = true;
            }
            wallType = MAP[tile.x + tile.y * MAP_SIDE];
            if (wallType != 0)
            {
                break;
            }
        }
        // 4. When collision found, calculate distance to wall.
        // Subtract one tile, because collision actually happens at edge of wall (position + size of tile).
        float distance = isHitVertical ? totalDist.y - deltaFactor.y : totalDist.x - deltaFactor.x;
        int wallHeight = static_cast<int>(floor(SCREEN_HEIGHT / fmax(distance, 1.0)));
        float collisionAt = isHitVertical ? playerPos.x + rayDir.x * distance : playerPos.y + rayDir.y * distance;
        collisionAt = collisionAt - floor(collisionAt); // [0.0f, 1.0f]
        // brightness at distance 1 max, across whole map almost 0
        //float brightness = pow(((mapSize + 1) - distance) / mapSize, 2);

        // walls
        drawVLine(fb, screenCol, SCREEN_HEIGHT / 2 - wallHeight / 2, wallHeight, 2);
    }
}

void Raycaster::drawHLines(uint16_t* fb, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
    assert(x < SCREEN_WIDTH && y < SCREEN_HEIGHT && "Invalid coordinates");
    assert(width <= SCREEN_WIDTH && width > 0 && "Invalid width");
    assert(height <= SCREEN_HEIGHT && height > 0 && "Invalid height");
    //memset(&fb[SCREEN_WIDTH * y + x], color, width * sizeof(uint16_t));
    for (uint16_t i = 0; i < height; i++) {
        for (uint16_t j = 0; j < width; j++) {
			fb[SCREEN_WIDTH * (y + i) + x + j] = color;
		}
	}
}

void Raycaster::drawHLines(uint16_t* fb, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t fromColor, uint16_t toColor)
{
	assert(x < SCREEN_WIDTH && y < SCREEN_HEIGHT && "Invalid coordinates");
	assert(width <= SCREEN_WIDTH && width > 0 && "Invalid width");
	assert(height <= SCREEN_HEIGHT && height > 0 && "Invalid height");

    // Assuming RGB565 format
    uint8_t fromR = (fromColor >> 11) & 0x1F; 
    uint8_t fromG = (fromColor >> 5) & 0x3F;
    uint8_t fromB = fromColor & 0x1F;

    uint8_t toR = (toColor >> 11) & 0x1F;
    uint8_t toG = (toColor >> 5) & 0x3F;
    uint8_t toB = toColor & 0x1F;
  
	for (uint16_t i = 0; i < height; i++) {
        float t = static_cast<float>(i) / (height - 1);
        uint8_t r = static_cast<int>(fromR + t * (toR - fromR));
        uint8_t g = static_cast<int>(fromG + t * (toG - fromG));
        uint8_t b = static_cast<int>(fromB + t * (toB - fromB));
        uint16_t interpolatedColor = (r << 11) | (g << 5) | b;
		for (uint16_t j = 0; j < width; j++) {
            fb[SCREEN_WIDTH * (y + i) + x + j] = interpolatedColor;
		}
	}
}

void Raycaster::drawVLine(uint16_t *fb, uint16_t x, uint16_t y, uint16_t height, uint8_t wallType)
{
    assert(x < SCREEN_WIDTH && y < SCREEN_HEIGHT && "Invalid coordinates");
    assert(height <= SCREEN_HEIGHT && height > 0 && "Invalid height");
    uint16_t color = 0;
    switch (wallType)
    {
    case 1:
        color = 0xF800;
        break;
    case 0:
        color = 0x07E0;
        break;
    case 2:
        color = 0x001F;
        break;
    }
    for (uint16_t i = 0; i < height; i++)
    {
        fb[KrisRaycaster::SCREEN_WIDTH * (y + i) + x] = color;
    }
}