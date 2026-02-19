#include "ui.h"
#include "CommonData.h"
#include "ScreenMapping.h"
#include "MainScreen.h"
#include "VideoInfoScreen.h"

ScreenMapping& ScreenMapping::GetInstance()
{
    static ScreenMapping instance;
    return instance;
}

ScreenMapping::ScreenMapping()
{
    this->event = { 0 };

    // Add screen info to map
    this->mapScreenInfo = {
        {
            SCREEN_NAME::MAIN_SCREEN,
            {
                []() { return new MainScreen(SCREEN_NAME::MAIN_SCREEN); },
                []() { _ui_screen_change(&ui_Main, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Main_screen_init); },
            },
        },
{
            SCREEN_NAME::VIDEO_INPUT_SCREEN,
            {
                []() { return new VideoInfoScreen(SCREEN_NAME::VIDEO_INPUT_SCREEN); },
                []() { _ui_screen_change(&ui_VideoInput, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_VideoInput_screen_init); },
            },
        },
    };
}

void ScreenMapping::SetEvent(lv_event_t event)
{
    this->event = event;
}

void ScreenMapping::ChangeScreen(SCREEN_NAME screen)
{
    // Delete current screen info
    delete this->currentScreenInfo.second;

    // Create new screen info
    this->currentScreenInfo.first = screen;
    this->currentScreenInfo.second = mapScreenInfo[screen].first();
    mapScreenInfo[screen].second();
}

void ScreenMapping::HandleScreen()
{
    // Handle operation
    this->currentScreenInfo.second->ButtonOperator((lv_obj_t*)(this->event.current_target), this->event.code);
    this->currentScreenInfo.second->DataUpdateOperator();

    // Reset event data
    this->event = { 0 };
}
