#include <gui/common/FrontendApplication.hpp>
#include <gui/common/globals.hpp>

#include "touchgfx/Utils.hpp"

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : FrontendApplicationBase(m, heap)
{
    touchgfx_printf("Screen width: %d, screen height: %d\n", KrisRaycaster::SCREEN_WIDTH, KrisRaycaster::SCREEN_HEIGHT);
    // Initialze dynamic bitmap cache
    constexpr uint32_t cacheSize = KrisRaycaster::SCREEN_WIDTH * KrisRaycaster::SCREEN_HEIGHT * sizeof(uint16_t) + 0x1c; // 16bpp + overhead
    uint16_t *const cacheAddr = static_cast<uint16_t*>(malloc(cacheSize));
    // TouchGFX application is statically allocated, so we should be able to fit dynamic bitmap in remaining memory
    Bitmap::setCache(cacheAddr, cacheSize, 1);
    touchgfx_printf("Dynamic bitmap cache: 0x%p, size: %u B\n", cacheAddr, cacheSize);
}
