#ifndef _SETTING_SCREEN_H
#define _SETTING_SCREEN_H

#include "BaseScreen.h"

class SettingScreen : public BaseScreen
{
public:
    SettingScreen(SCREEN_NAME screen);
    ~SettingScreen();

private:
    static void OnClickCancel(lv_event_t* event);
};

#endif // _SETTING_SCREEN_H
