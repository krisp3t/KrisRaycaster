#ifndef KRISRAYCASTER_PICTURE_HPP
#define KRISRAYCASTER_PICTURE_HPP
#include <cstdint>
#include <gui/raycaster/Math.hpp>

namespace touchgfx
{
	class Rect;
}

namespace Picture
{
	void copySrcDest(const uint16_t* src, uint16_t* dest, Vec2 srcSize, Vec2 destSize, Vec2 srcPos, Vec2 destPos);
	void copySrcDestRect(const uint16_t* src, uint16_t* dest, Vec2 srcSize, Vec2 destSize, touchgfx::Rect srcRect, touchgfx::Rect destRect);
}
#endif