#ifndef _SETTING_SCREEN_H
#define _SETTING_SCREEN_H

#include "BaseScreen.h"

class SettingScreen : public BaseScreen
{
public:
    SettingScreen(SCREEN_NAME screen);
    ~SettingScreen();

    static void InitSettingScreen();

private:
    static void OnClickCancel(lv_event_t* event);
    static void OnClickSave(lv_event_t* event);
    static void OnClickEdit(lv_event_t* event);

    static void InitGeneralSetting();
    static void InitKeyboardSetting();
    static void InitNetworkSetting();

    static void UpdateKeyboardSetting();
    static void UpdateNetworkState();
    static void UpdateWiFiInfo();

    static void SetDuktoServerIP();
};

#endif // _SETTING_SCREEN_H
