#ifndef RAYCASTER_HPP
#define RAYCASTER_HPP
#include <cstdint>
#include <gui/raycaster/Math.hpp>


namespace Raycaster
{
	void render(uint8_t* framebuffer);
	void drawHLines(uint16_t* fb, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
	void drawVLine(uint16_t* fb, uint16_t x, uint16_t y, uint16_t height, uint8_t wallType);
	extern Vec2f playerPos;
	extern Vec2f dir;
	extern Vec2f cameraPlane;
};
#endif