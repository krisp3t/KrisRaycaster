#include <cstring>
#include <gui/raycaster/Raycaster.hpp>
#include <gui/common/globals.hpp>

void Raycaster::render(uint8_t *framebuffer)
{
	memset(framebuffer, 0x33, 1 * KrisRaycaster::SCREEN_HEIGHT * sizeof(uint16_t));
	//drawVLine();
}

void Raycaster::drawVLine()
{
}