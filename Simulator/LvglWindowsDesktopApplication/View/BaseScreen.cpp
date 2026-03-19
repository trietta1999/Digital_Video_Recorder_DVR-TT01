#include "BaseScreen.h"
#include "CommonData.h"

void BaseScreen::ButtonOperator(lv_event_t* event)
{
    for (const auto& item : ListButtonCallback)
    {
        if ((item.btnObj == event->current_target) && (item.btnEvent == event->code))
        {
            item.btnCallback(event);
        }
    }
}

void BaseScreen::DataUpdateOperator()
{
    // Update data when open new screen
    if (isFirstInit)
    {
        for (const auto& item : ListDataUpdateCallback)
        {
            item.second();
        }

        isFirstInit = false;
    }
    // Normal update data when state change
    else
    {
        for (const auto& item : ListDataUpdateCallback)
        {
            if (item.first() == true)
            {
                item.second();
            }
        }
    }
}
