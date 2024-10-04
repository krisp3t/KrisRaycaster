#ifndef RAYCASTER_HPP
#define RAYCASTER_HPP
#include <cstdint>
#include <gui/raycaster/Math.hpp>

namespace touchgfx
{
	class Rect;
}

namespace Raycaster
{
	touchgfx::Rect getMapRect(uint8_t cellType);
	void initMap(const uint16_t* src, uint16_t* dest, Vec2 srcSize, Vec2 destSize);
	void movePlayer(float speedMultiplier);
	void rotatePlayer(float angle);
	void render(uint8_t* framebuffer);
	void drawHLines(uint16_t* fb, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
	void drawHLines(uint16_t* fb, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t fromColor, uint16_t toColor);
	void drawVLine(const uint16_t* texFb, uint16_t* fb, touchgfx::Rect destRect, uint8_t wallType, float collisionAt, float brightness);
	extern Vec2f playerPos;
	extern Vec2f dir;
	extern Vec2f cameraPlane;
}
#endif