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
    touchgfx_printf("Map dynamic bitmap @ 0x%p\n", Bitmap::dynamicBitmapGetAddress(mapBmpId));
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
    // Player position on minimap
    playerIndicator = touchgfx::Box(10, 10, 0xFF0000, 255);
    playerIndicator.setXY(0, 0);
    add(playerIndicator);

    // Raycaster setup
    gameBmpId = Bitmap::dynamicBitmapCreate(KrisRaycaster::SCREEN_WIDTH, KrisRaycaster::SCREEN_HEIGHT, Bitmap::RGB565);
    if (gameBmpId == BITMAP_INVALID)
	{
		touchgfx_printf("Failed to create dynamic bitmap for game\n");
	}
    touchgfx_printf("Game dynamic bitmap @ 0x%p\n", Bitmap::dynamicBitmapGetAddress(gameBmpId));
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
    Raycaster::movePlayer(static_cast<float>(playerDir));
	Raycaster::render(gameFb);

	const Rect& playerRect = playerIndicator.getRect();
    Rect mapRect = Rect(playerRect.x, playerRect.y, playerRect.width, playerRect.height);
    mapImg.invalidateRect(mapRect);
    Vec2 boxPx = Raycaster::playerPos.toScreenSpace();
    playerIndicator.setXY(boxPx.x - playerRect.width / 2, boxPx.y - playerRect.width / 2);
    playerIndicator.invalidateContent();
    gameImg.invalidate();

    short fps = 60 / HAL::getInstance()->getLCDRefreshCount();
    short ms = static_cast<short>(round((1000.0f / fps)));
    snprintf(debugStringBuffer, sizeof(debugStringBuffer), "%d fps (%d ms)", fps, ms);
    Application::getDebugPrinter()->setString(debugStringBuffer);
    Application::invalidateDebugRegion();
}

void GameScreenView::TransformPlayer(Vec2 evt)
{
    if (previousTouchPos.x == 0 && previousTouchPos.y == 0)
	{
        previousTouchPos = evt;
        return;
	}
    Vec2f delta = { (evt.x - previousTouchPos.x) * 1.0f, (evt.y - previousTouchPos.y) * 1.0f };
    float length = sqrt(delta.x * delta.x + delta.y * delta.y);
    // Deadzone
    if (length < 3.0f)
    {
        return;
    }
    //touchgfx_printf("Prev: (%d, %d), evt: (%d, %d), Delta: (%f, %f), len: %f", previousTouchPos.x, previousTouchPos.y, evt.x, evt.y, delta.x, delta.y, length);
    previousTouchPos = evt;

    constexpr float rotateSensitivity = 0.02f;
    Raycaster::rotatePlayer(delta.x * rotateSensitivity);
    if (abs(delta.y) > 3)
    {
        playerDir = delta.y > 0 ? PlayerDirection::FORWARDS : PlayerDirection::BACKWARDS;
    }
}
void GameScreenView::handleClickEvent(const ClickEvent& event)
{
    if (event.getType() == ClickEvent::RELEASED)
	{
        touchgfx_printf("Click event released\n");
		previousTouchPos = { 0, 0 };
        playerDir = PlayerDirection::STOPPED;
	}
}

void GameScreenView::handleDragEvent(const DragEvent& event)
{
    Vec2 evt{ event.getNewX(), event.getNewY() };
    //touchgfx_printf("Drag event at (%d, %d)\n", evt.x, evt.y);
    TransformPlayer(evt);
}