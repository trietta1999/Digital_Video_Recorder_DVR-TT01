#ifndef _SCREEN_MAPPING_H
#define _SCREEN_MAPPING_H

#include "CommonDataType.h"
#include "BaseScreen.h"

class ScreenMapping
{
private:
    ScreenMapping(); // Private constructor for Singleton pattern

    // Maps screen names to factory functions and cleanup logic
    std::unordered_map<SCREEN_NAME, std::pair<std::function<BaseScreen* (void)>, std::function<void(void)>>> mapScreenInfo;
    std::pair<SCREEN_NAME, BaseScreen*> currentScreenInfo; // Tracks the active screen name and instance
    lv_event_t event; // Stores the last LVGL event context

public:
    static ScreenMapping& GetInstance(); // Singleton access point

    void SetEvent(lv_event_t event); // Stores an LVGL event for the next screen handling cycle
    const lv_event_t& GetEvent() { return event; } // Retrieves the stored LVGL event
    void ChangeScreen(SCREEN_NAME screen); // Orchestrates screen transition and instance management
    void HandleScreen(); // Main polling function for active screen logic
};

#endif
