#include <functional>
#include <gui/raycaster/Picture.hpp>
#include <touchgfx/hal/Types.hpp>

namespace Picture
{
    void copySrcDest(const uint16_t *src, uint16_t *dest, Vec2 srcSize, Vec2 destSize, Vec2 srcPos, Vec2 destPos)
    {
        assert(src != nullptr &&
            dest != nullptr &&
            "Invalid src/dest buffer");
        for (uint16_t y = 0; y < destSize.y; y++)
        {
	        for (uint16_t x = 0; x < destSize.x; x++)
	        {
                float scaleX = static_cast<float>(srcSize.x) / destSize.x;
                float scaleY = static_cast<float>(srcSize.y) / destSize.y;
                uint16_t srcX = static_cast<uint16_t>(x * scaleX);
                uint16_t srcY = static_cast<uint16_t>(y * scaleY);

                assert(srcX + scaleX <= srcSize.x &&
                       srcY + scaleY <= srcSize.y &&
                       scaleX <= destSize.x &&
                       scaleY <= destSize.y &&
                       "Invalid coordinates");
                dest[(y + destPos.y) * destSize.x + (x + destPos.x)] = src[(srcY + srcPos.y) * srcSize.x + (srcX + srcPos.x)];
            }
        }
    }
    void copySrcDestRect(const uint16_t* src, uint16_t* dest, Vec2 srcSize, Vec2 destSize, touchgfx::Rect srcRect, touchgfx::Rect destRect)
    {
	    copySrcDestRect(src, dest, srcSize, destSize, srcRect, destRect, 1.0);
    }

    void copySrcDestRect(const uint16_t *src, uint16_t *dest, Vec2 srcSize, Vec2 destSize, touchgfx::Rect srcRect, touchgfx::Rect destRect, float brightness = 1.0)
    {
        float scaleX = static_cast<float>(srcRect.width) / destRect.width;
        float scaleY = static_cast<float>(srcRect.height) / destRect.height;

        assert(src != nullptr &&
			   dest != nullptr &&
			   "Invalid src/dest buffer");
        assert(srcRect.x + srcRect.width <= srcSize.x &&
			   srcRect.y + srcRect.height <= srcSize.y &&
			   destRect.x + destRect.width <= destSize.x &&
			   destRect.y + destRect.height <= destSize.y &&
			   "Invalid input coordinates");

        for (uint16_t destY = destRect.y; destY < destRect.y + destRect.height; destY++)
        {
            for (uint16_t destX = destRect.x; destX < destRect.x + destRect.width; destX++)
            {
                uint16_t srcX = static_cast<uint16_t>((destX - destRect.x) * scaleX) + srcRect.x;
                uint16_t srcY = static_cast<uint16_t>((destY - destRect.y) * scaleY) + srcRect.y;

                assert(srcX < srcSize.x &&
					   srcY < srcSize.y &&
					   destX < destSize.x &&
					   destY < destSize.y &&
					   "Invalid output coordinates");
                if (brightness < 1.0)
                {
                	dest[destY * destSize.x + destX] = applyBrightness(src[srcY * srcSize.x + srcX], brightness);
				}
				else
				{
					dest[destY * destSize.x + destX] = src[srcY * srcSize.x + srcX];
                }
            }
        }
    }

    uint16_t applyBrightness(uint16_t color, float brightness)
    {
    	uint8_t r = (color & 0xF800) >> 11;
		uint8_t g = (color & 0x07E0) >> 5;
		uint8_t b = color & 0x001F;

		r = static_cast<uint8_t>(r * brightness);
		g = static_cast<uint8_t>(g * brightness);
		b = static_cast<uint8_t>(b * brightness);

		return (r << 11) | (g << 5) | b;
    }
}
