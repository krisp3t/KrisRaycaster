#include <cstdlib>
#include <gui/common/FrontendApplication.hpp>
#include <gui/common/globals.hpp>
#include <touchgfx/lcd/LCD16DebugPrinter.hpp>
#include <touchgfx/Utils.hpp>

LCD16DebugPrinter lcd16DebugPrinter;

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

    lcd16DebugPrinter.setColor(0xffffff);
    lcd16DebugPrinter.setPosition(10, 10, 240, 40);
    lcd16DebugPrinter.setScale(3);
    Application::setDebugPrinter(&lcd16DebugPrinter);
}
