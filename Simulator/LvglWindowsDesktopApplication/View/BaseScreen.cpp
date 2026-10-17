#include "BaseScreen.h"
#include "CommonData.h"
#include "CommonLibrary.h"

void BaseScreen::ButtonOperator(lv_event_t* event)
{
    // Iterate through registered callbacks to match event target and code
    for (const auto& item : ListButtonCallback)
    {
        if ((item.btnObj == event->current_target) && (item.btnEvent == event->code))
        {
            // Only execute callback if the object is not disabled
            if ((lv_obj_get_state(item.btnObj) & LV_STATE_DISABLED) != LV_STATE_DISABLED)
            {
                item.btnCallback(event);
            }
        }
    }
}

void BaseScreen::DataUpdateOperator()
{
    // Execute all update callbacks once when the screen is first initialized
    if (isFirstInit)
    {
        for (const auto& item : ListDataUpdateCallback)
        {
            item.second();
        }

        isFirstInit = false;
    }
    // Perform conditional updates based on state changes in subsequent cycles
    else
    {
        for (const auto& item : ListDataUpdateCallback)
        {
            // Check if the state associated with the callback has changed
            if (item.first() == true)
            {
                item.second();
            }
        }
    }
}

void BaseScreen::RegisterKeyboard() const
{
    // Synchronize the hardware keyboard mapping with the current screen's button layout
    keyboard_lib::SetListVkCode(ListButtonVkCode);
}
