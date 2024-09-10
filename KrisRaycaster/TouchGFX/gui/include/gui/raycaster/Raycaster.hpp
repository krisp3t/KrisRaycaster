#ifndef RAYCASTER_HPP
#define RAYCASTER_HPP
#include <cstdint>

class Raycaster
{
public:
	static void render(uint8_t *framebuffer);
private:
	void drawVLine();
};
#endif