#include "BaseScreen.h"
#include "CommonData.h"

void BaseScreen::ButtonOperator(lv_obj_t* btnObj, lv_event_code_t btnEvent)
{
    for (const auto& item : ListButtonCallback)
    {
        if ((item.btnObj == btnObj) && (item.btnEvent == btnEvent))
        {
            item.btnCallback(btnObj);
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
