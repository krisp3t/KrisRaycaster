#include <cstdio>
#include <gui/gamescreen_screen/GameScreenView.hpp>
#include <gui/common/globals.hpp>
#include <gui/raycaster/Raycaster.hpp>
#include <touchgfx/Utils.hpp>

#ifdef SIMULATOR
#include <ctime>
#endif

char debugStringBuffer[30];

GameScreenView::GameScreenView()
{

}

void GameScreenView::setupScreen()
{
    GameScreenViewBase::setupScreen();
    bmpId = Bitmap::dynamicBitmapCreate(KrisRaycaster::SCREEN_WIDTH, KrisRaycaster::SCREEN_HEIGHT, Bitmap::RGB565);
    if (bmpId == BITMAP_INVALID)
	{
		touchgfx_printf("Failed to create dynamic bitmap\n");
	}
    touchgfx_printf("Setting up dynamic bitmap at 0x%p\n", Bitmap::dynamicBitmapGetAddress(bmpId));
    memset(
        Bitmap::dynamicBitmapGetAddress(bmpId), 
        0, 
        KrisRaycaster::SCREEN_WIDTH * KrisRaycaster::SCREEN_HEIGHT * sizeof(uint16_t)
    );
    image.setBitmap(Bitmap(bmpId));
    image.setXY(0,0);
    image.setAlpha(255);
    image.setWidthHeight(KrisRaycaster::SCREEN_WIDTH, KrisRaycaster::SCREEN_HEIGHT);
    add(image);
}

void GameScreenView::tearDownScreen()
{
    GameScreenViewBase::tearDownScreen();
    // TODO: Implement GameScreenView::tearDownScreen
}

void GameScreenView::handleTickEvent()
{
	uint8_t *fb = Bitmap::dynamicBitmapGetAddress(bmpId);
    Raycaster::playerPos.x += 0.01f;
    Raycaster::playerPos.y -= 0.01f;
	Raycaster::render(fb);
	image.invalidate();

    // get timer
    static int count = 0;
    count++;
    HAL_GetTick();
    snprintf(debugStringBuffer, sizeof(debugStringBuffer), "%d fps (ms)", clock());
    Application::getDebugPrinter()->setString(debugStringBuffer);
    Application::invalidateDebugRegion();
}