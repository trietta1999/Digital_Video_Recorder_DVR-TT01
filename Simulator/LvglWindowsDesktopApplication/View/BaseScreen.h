#ifndef _BASE_SCREEN_H
#define _BASE_SCREEN_H

#include "CommonDataType.h"

class BaseScreen
{
protected:
    typedef struct
    {
        lv_obj_t* btnObj; // Pointer to the LVGL button object
        std::function<void(lv_event_t*)> btnCallback; // Callback function to execute on event
        lv_event_code_t btnEvent; // The LVGL event code to trigger on (e.g., LV_EVENT_CLICKED)
    } ButtonInfo;

    std::vector<ButtonInfo> ListButtonCallback; // Collection of registered button interactions
    std::vector<std::pair<std::function<bool(void)>, std::function<void(void)>>> ListDataUpdateCallback; // Pairs of [Condition -> Action] for reactive data binding
    std::vector<std::pair<lv_obj_t*, int>> ListButtonVkCode; // Mapping of UI buttons to virtual key codes for hardware keyboard support

    SCREEN_NAME screenName; // The unique name identifier for this screen
    bool isFirstInit; // Flag to track if the screen has completed its initial setup

public:
    BaseScreen(SCREEN_NAME screen) { this->screenName = screen; this->isFirstInit = true; };
    virtual ~BaseScreen() = default;

    void RegisterKeyboard() const; // Syncs screen-specific key mappings to the global keyboard library
    void ButtonOperator(lv_event_t* event); // Generic event dispatcher for registered button callbacks
    void DataUpdateOperator(); // Polls data binding conditions and executes actions on change
};

#endif // _BASE_SCREEN_H
