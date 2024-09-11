#ifndef GAMESCREENVIEW_HPP
#define GAMESCREENVIEW_HPP

#include <gui_generated/gamescreen_screen/GameScreenViewBase.hpp>
#include <gui/gamescreen_screen/GameScreenPresenter.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <gui/raycaster/Math.hpp>
#include "gui/common/globals.hpp"

class GameScreenView : public GameScreenViewBase
{
public:
    GameScreenView();
    virtual ~GameScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
    void RotatePlayer(Vec2 evt);
    virtual void handleClickEvent(const ClickEvent& event) override;
    virtual void handleDragEvent(const DragEvent& event) override;
private:
    touchgfx::BitmapId mapBmpId;
    touchgfx::Image mapImg;

	touchgfx::BitmapId gameBmpId;
    touchgfx::Image gameImg;
    Vec2 previousTouchPos = { KrisRaycaster::SCREEN_WIDTH + KrisRaycaster::SCREEN_WIDTH / 2, KrisRaycaster::SCREEN_HEIGHT / 2 };
};

#endif // GAMESCREENVIEW_HPP
