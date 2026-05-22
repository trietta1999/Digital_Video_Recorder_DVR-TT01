#include "ui.h"
#include "CommonData.h"
#include "CommonLibrary.h"
#include "ScreenMapping.h"
#include "MainScreen.h"
#include "VideoInfoScreen.h"
#include "KeyboardScreen.h"
#include "VideoRecordListScreen.h"
#include "SettingScreen.h"

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
            SCREEN_NAME::SCREEN_MAIN,
            {
                []() { return new MainScreen(SCREEN_NAME::SCREEN_MAIN); },
                []() { _ui_screen_change(&ui_Main, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Main_screen_init); },
            },
        },
        {
            SCREEN_NAME::SCREEN_VIDEO_INPUT,
            {
                []() { return new VideoInfoScreen(SCREEN_NAME::SCREEN_VIDEO_INPUT); },
                []() { _ui_screen_change(&ui_VideoInput, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_VideoInput_screen_init); },
            },
        },
        {
            SCREEN_NAME::SCREEN_KEYBOARD,
            {
                []() { return new KeyboardScreen(SCREEN_NAME::SCREEN_KEYBOARD); },
                []() { _ui_screen_change(&ui_Keyboard, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Keyboard_screen_init); },
            },
        },
        {
            SCREEN_NAME::SCREEN_VIDEO_RECORDLIST,
            {
                []() { return new VideoRecordListScreen(SCREEN_NAME::SCREEN_VIDEO_RECORDLIST); },
                []() { _ui_screen_change(&ui_RecordList, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_RecordList_screen_init); },
            },
        },
        {
            SCREEN_NAME::SCREEN_SETTING,
            {
                []() { return new SettingScreen(SCREEN_NAME::SCREEN_SETTING); },
                []() { _ui_screen_change(&ui_Setting, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Setting_screen_init); },
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
        mapScreenInfo[screen].second(); // Call change screen
        currentScreenInfo.second = mapScreenInfo[screen].first(); // Create screen class
    }
    else if (common_lib::CheckInRangeNumber((int)screen, (int)SCREEN_NAME::MIN_KBSCREEN, (int)SCREEN_NAME::MAX_KBSCREEN))
    {
        system_data::CurrentKbScreen.SetValue(screen);

        // Create keyboard screen info
        currentScreenInfo.first = SCREEN_NAME::SCREEN_KEYBOARD;
        mapScreenInfo[SCREEN_NAME::SCREEN_KEYBOARD].second(); // Call change screen
        currentScreenInfo.second = mapScreenInfo[SCREEN_NAME::SCREEN_KEYBOARD].first(); // Create screen class
    }
}

void ScreenMapping::HandleScreen()
{
    // Handle operation
    currentScreenInfo.second->ButtonOperator(&event);
    currentScreenInfo.second->DataUpdateOperator();

    // Reset event data
    event = { 0 };
}
