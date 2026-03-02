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
    for (const auto& item : ListDataUpdateCallback)
    {
        if (item.first() == true)
        {
            item.second();
        }
    }
}
