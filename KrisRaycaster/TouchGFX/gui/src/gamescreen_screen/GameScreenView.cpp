#include <cstdio>
#include <cmath>
#include <gui/gamescreen_screen/GameScreenView.hpp>
#include <gui/common/globals.hpp>
#include <gui/raycaster/Raycaster.hpp>
#include <images/BitmapDatabase.hpp>
#include <touchgfx/Utils.hpp>

#include "gui/raycaster/Picture.hpp"

char debugStringBuffer[30];

GameScreenView::GameScreenView()
{

}

void GameScreenView::setupScreen()
{
    GameScreenViewBase::setupScreen();

    // Map bitmap setup
    mapBmpId = Bitmap::dynamicBitmapCreate(KrisRaycaster::SCREEN_WIDTH, KrisRaycaster::SCREEN_HEIGHT, Bitmap::RGB565);
    if (mapBmpId == BITMAP_INVALID)
    {
        touchgfx_printf("Failed to create dynamic bitmap for map\n");
    }
    touchgfx_printf("Setting up map dynamic bitmap at 0x%p\n", Bitmap::dynamicBitmapGetAddress(mapBmpId));
    memset(
        Bitmap::dynamicBitmapGetAddress(mapBmpId),
        0,
        KrisRaycaster::SCREEN_WIDTH * KrisRaycaster::SCREEN_HEIGHT * sizeof(uint16_t)
    );
    // Map image setup
    Bitmap mapSource(BITMAP_WALL_ID);
    const uint16_t* sourceData = (const uint16_t*)mapSource.getData();
    uint16_t* destData = (uint16_t*)Bitmap::dynamicBitmapGetAddress(mapBmpId);
    if (!(sourceData && destData))
    {
	    touchgfx_printf("Failed to get source and destination data\n");
	}
	else
	{
        Raycaster::initMap(
            sourceData, 
            destData, 
            Vec2{ mapSource.getWidth(), mapSource.getHeight()}, 
            Vec2{KrisRaycaster::SCREEN_WIDTH, KrisRaycaster::SCREEN_HEIGHT}
        );
    }
    mapImg.setBitmap(Bitmap(mapBmpId));
    mapImg.setXY(0, 0);
    mapImg.setAlpha(255);
    mapImg.setWidthHeight(KrisRaycaster::SCREEN_WIDTH, KrisRaycaster::SCREEN_HEIGHT);
    add(mapImg);

    // Raycaster setup
    gameBmpId = Bitmap::dynamicBitmapCreate(KrisRaycaster::SCREEN_WIDTH, KrisRaycaster::SCREEN_HEIGHT, Bitmap::RGB565);
    if (gameBmpId == BITMAP_INVALID)
	{
		touchgfx_printf("Failed to create dynamic bitmap for game\n");
	}
    touchgfx_printf("Setting up game dynamic bitmap at 0x%p\n", Bitmap::dynamicBitmapGetAddress(gameBmpId));
    memset(
        Bitmap::dynamicBitmapGetAddress(gameBmpId), 
        0, 
        KrisRaycaster::SCREEN_WIDTH * KrisRaycaster::SCREEN_HEIGHT * sizeof(uint16_t)
    );
    gameImg.setBitmap(Bitmap(gameBmpId));
    gameImg.setXY(KrisRaycaster::SCREEN_WIDTH,0);
    gameImg.setAlpha(255);
    gameImg.setWidthHeight(KrisRaycaster::SCREEN_WIDTH, KrisRaycaster::SCREEN_HEIGHT);
    add(gameImg);
}

void GameScreenView::tearDownScreen()
{
    GameScreenViewBase::tearDownScreen();
    // TODO: Implement GameScreenView::tearDownScreen
}

void GameScreenView::handleTickEvent()
{
	uint8_t *gameFb = Bitmap::dynamicBitmapGetAddress(gameBmpId);
    Raycaster::playerPos.x += 0.01f;
    Raycaster::playerPos.y -= 0.01f;
	Raycaster::render(gameFb);
	gameImg.invalidate();

    short fps = 60 / HAL::getInstance()->getLCDRefreshCount();
    short ms = static_cast<short>(round((1000.0f / fps)));
    snprintf(debugStringBuffer, sizeof(debugStringBuffer), "%d fps (%d ms)", fps, ms);
    Application::getDebugPrinter()->setString(debugStringBuffer);
    Application::invalidateDebugRegion();
}