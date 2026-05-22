#ifndef _MAIN_SCREEN_H
#define _MAIN_SCREEN_H

#include "BaseScreen.h"

class MainScreen : public BaseScreen
{
public:
    MainScreen(SCREEN_NAME screen);
    ~MainScreen();

private:
    static void OnClickNew(lv_event_t* event);
    static void OnClickClose(lv_event_t* event);
    static void OnClickOperator(lv_event_t* event);
    static void OnClickVideoRecordList(lv_event_t* event);
    static void OnClickStorageUSB(lv_event_t* event);
    static void OnClickSystemSetting(lv_event_t* event);

    static void UpdateDate();
    static void UpdateTime();
    static void UpdateStorage();
    static void UpdateButton();
    static void UpdateExtDevice();
};

#endif // _MAIN_SCREEN_H
