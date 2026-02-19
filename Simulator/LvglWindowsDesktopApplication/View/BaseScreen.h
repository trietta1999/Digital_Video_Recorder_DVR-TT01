#ifndef _BASE_SCREEN_H
#define _BASE_SCREEN_H

#include "CommonDataType.h"

class BaseScreen
{
protected:
    typedef struct
    {
        lv_obj_t* btnObj;
        std::function<void(lv_obj_t*)> btnCallback;
        lv_event_code_t btnEvent;
    } ButtonInfo;

    std::vector<ButtonInfo> ListButtonCallback;
    std::vector<std::pair<std::function<bool(void)>, std::function<void(void)>>> ListDataUpdateCallback;

    SCREEN_NAME screenName;

public:
    BaseScreen(SCREEN_NAME screen) { this->screenName = screen; };
    virtual ~BaseScreen() = default;

    SCREEN_NAME GetScreenName() const { return this->screenName; }

    void ButtonOperator(lv_obj_t* btnObj, lv_event_code_t btnEvent);
    void DataUpdateOperator();
};

#endif // _BASE_SCREEN_H
