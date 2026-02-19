#ifndef _SCREEN_MAPPING_H
#define _SCREEN_MAPPING_H

#include "CommonDataType.h"
#include "BaseScreen.h"

class ScreenMapping
{
private:
    ScreenMapping();

    std::unordered_map<SCREEN_NAME, std::pair<std::function<BaseScreen*(void)>, std::function<void(void)>>> mapScreenInfo;
    std::pair<SCREEN_NAME, BaseScreen*> currentScreenInfo;
    lv_event_t event;

public:
    static ScreenMapping& GetInstance();

    void SetEvent(lv_event_t event);
    void ChangeScreen(SCREEN_NAME screen);
    void HandleScreen();
};

#endif
