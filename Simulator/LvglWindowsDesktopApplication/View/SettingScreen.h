#ifndef _SETTING_SCREEN_H
#define _SETTING_SCREEN_H

#include "BaseScreen.h"

class SettingScreen : public BaseScreen
{
public:
    SettingScreen(SCREEN_NAME screen);
    ~SettingScreen() = default;

private:
    static void OnClickCancel(lv_event_t* event);
    static void OnClickSave(lv_event_t* event);

    void InitGeneralSetting();
    void InitKeyboardSetting();
    void InitNetworkSetting();
};

#endif // _SETTING_SCREEN_H
