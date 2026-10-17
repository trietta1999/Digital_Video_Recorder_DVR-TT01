#include "ui.h"
#include "CommonData.h"
#include "CommonLibrary.h"
#include "ScreenMapping.h"
#include "MainScreen.h"
#include "VideoInfoScreen.h"
#include "KeyboardScreen.h"
#include "VideoRecordListScreen.h"
#include "SettingScreen.h"

// Singleton accessor for the ScreenMapping instance
ScreenMapping& ScreenMapping::GetInstance()
{
    static ScreenMapping instance;
    return instance;
}

// Constructor initializes the screen map with factory functions and LVGL transition logic
ScreenMapping::ScreenMapping()
{
    event = { 0 }; // Initialize empty event context

    // Map screen identifiers to their respective creation and transition lambdas
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

// Stores the last triggered LVGL event for asynchronous handling
void ScreenMapping::SetEvent(lv_event_t event)
{
    this->event = event;
}

// Orchestrates the transition from one screen to another
void ScreenMapping::ChangeScreen(SCREEN_NAME screen)
{
    // Cleanup previous screen instance
    delete currentScreenInfo.second;

    // Handle standard application screens
    if (common_lib::CheckInRangeNumber((int)screen, (int)SCREEN_NAME::MIN_SCREEN, (int)SCREEN_NAME::MAX_SCREEN))
    {
        system_data::CurrentScreen.SetValue(screen);
        system_data::CurrentKbScreen.SetValue(SCREEN_NAME::MIN_KBSCREEN);

        // Create new screen info
        currentScreenInfo.first = screen;
        mapScreenInfo[screen].second(); // Execute LVGL screen load logic
        lv_timer_handler(); // Force UI update to ensure objects are available
        currentScreenInfo.second = mapScreenInfo[screen].first(); // Instantiate the screen controller class
    }
    // Handle specialized keyboard entry screens
    else if (common_lib::CheckInRangeNumber((int)screen, (int)SCREEN_NAME::MIN_KBSCREEN, (int)SCREEN_NAME::MAX_KBSCREEN))
    {
        system_data::CurrentKbScreen.SetValue(screen);

        // Create keyboard screen info
        currentScreenInfo.first = SCREEN_NAME::SCREEN_KEYBOARD;
        mapScreenInfo[SCREEN_NAME::SCREEN_KEYBOARD].second(); // Shift to the generic keyboard UI
        lv_timer_handler();
        currentScreenInfo.second = mapScreenInfo[SCREEN_NAME::SCREEN_KEYBOARD].first(); // Instantiate keyboard controller
    }

    currentScreenInfo.second->RegisterKeyboard(); // Apply new key mappings for the active screen
}

// Main processing loop for screen-specific interactions and data reactive updates
void ScreenMapping::HandleScreen()
{
    // Execute interaction logic based on the stored event
    currentScreenInfo.second->ButtonOperator(&event);

    // Execute periodic data sync/UI updates
    currentScreenInfo.second->DataUpdateOperator();

    // Clear the event context after processing to prevent repeated triggers
    event = { 0 };
}
