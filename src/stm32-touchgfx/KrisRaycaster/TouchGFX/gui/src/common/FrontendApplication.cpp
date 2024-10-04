#include <cstdlib>
#include <gui/common/FrontendApplication.hpp>
#include <gui/common/globals.hpp>
#include <touchgfx/lcd/LCD16DebugPrinter.hpp>
#include <touchgfx/Utils.hpp>
#include <touchgfx/Bitmap.hpp>

LCD16DebugPrinter lcd16DebugPrinter;

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : FrontendApplicationBase(m, heap)
{
    touchgfx_printf("Screen width: %d, screen height: %d\n", KrisRaycaster::SCREEN_WIDTH * 2, KrisRaycaster::SCREEN_HEIGHT);

    // Initialze dynamic bitmap cache
    constexpr short bitmapCount = 2;
    constexpr short overhead = (sizeof(Bitmap) + sizeof(Bitmap::BitmapData) + sizeof(Bitmap::CacheTableEntry) + sizeof(Bitmap::DynamicBitmapData)) * bitmapCount;
    constexpr uint32_t cacheSize =
        KrisRaycaster::SCREEN_WIDTH * 
        KrisRaycaster::SCREEN_HEIGHT * 
        sizeof(uint16_t) * 
        bitmapCount + 
        overhead;
    uint16_t *const cacheAddr = static_cast<uint16_t*>(malloc(cacheSize));
    // TouchGFX application is statically allocated, so we should be able to fit dynamic bitmap in remaining memory
    Bitmap::setCache(cacheAddr, cacheSize, bitmapCount);
    touchgfx_printf("Dynamic bitmap cache: 0x%p, size: %u B\n", cacheAddr, cacheSize);

    lcd16DebugPrinter.setColor(0xffffff);
    lcd16DebugPrinter.setPosition(KrisRaycaster::SCREEN_WIDTH + 10, 10, 240, 40);
    lcd16DebugPrinter.setScale(3);
    Application::setDebugPrinter(&lcd16DebugPrinter);
}
