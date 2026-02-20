#include "ui.h"
#include "CommonData.h"
#include "CommonLibrary.h"
#include "ScreenMapping.h"
#include "MainScreen.h"
#include "VideoInfoScreen.h"
#include "KeyboardScreen.h"

ScreenMapping& ScreenMapping::GetInstance()
{
    static ScreenMapping instance;
    return instance;
}

ScreenMapping::ScreenMapping()
{
    event = { 0 };

    // Add screen info to map
    mapScreenInfo = {
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
        {
            SCREEN_NAME::KEYBOARD_SCREEN,
            {
                []() { return new KeyboardScreen(SCREEN_NAME::KEYBOARD_SCREEN); },
                []() { _ui_screen_change(&ui_Keyboard, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Keyboard_screen_init); },
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
    delete currentScreenInfo.second;

    if (common_lib::CheckInRangeNumber((int)screen, (int)SCREEN_NAME::MIN_SCREEN, (int)SCREEN_NAME::MAX_SCREEN))
    {
        system_data::CurrentScreen.SetValue(screen);
        system_data::CurrentKbScreen.SetValue(SCREEN_NAME::MIN_KBSCREEN);

        // Create new screen info
        currentScreenInfo.first = screen;
        currentScreenInfo.second = mapScreenInfo[screen].first();
        mapScreenInfo[screen].second();
    }
    else if (common_lib::CheckInRangeNumber((int)screen, (int)SCREEN_NAME::MIN_KBSCREEN, (int)SCREEN_NAME::MAX_KBSCREEN))
    {
        system_data::CurrentKbScreen.SetValue(screen);

        // Create keyboard screen info
        currentScreenInfo.first = SCREEN_NAME::KEYBOARD_SCREEN;
        currentScreenInfo.second = mapScreenInfo[SCREEN_NAME::KEYBOARD_SCREEN].first();
        mapScreenInfo[SCREEN_NAME::KEYBOARD_SCREEN].second();
    }
}

void ScreenMapping::HandleScreen()
{
    // Handle operation
    currentScreenInfo.second->ButtonOperator((lv_obj_t*)(event.current_target), event.code);
    currentScreenInfo.second->DataUpdateOperator();

    // Reset event data
    event = { 0 };
}
