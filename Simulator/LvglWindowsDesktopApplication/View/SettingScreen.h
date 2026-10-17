#ifndef _SETTING_SCREEN_H
#define _SETTING_SCREEN_H

#include "BaseScreen.h"

class SettingScreen : public BaseScreen
{
public:
    SettingScreen(SCREEN_NAME screen); // Constructor for SettingScreen
    ~SettingScreen(); // Destructor for SettingScreen

    static void InitSettingScreen(); // Initializes the main setting screen layout

private:
    static void OnClickCancel(lv_event_t* event); // Event handler for Cancel button click
    static void OnClickSave(lv_event_t* event); // Event handler for Save button click
    static void OnClickEdit(lv_event_t* event); // Event handler for initiating edit on a setting field

    static void InitGeneralSetting(); // Initializes UI components for general system settings
    static void InitKeyboardSetting(); // Initializes UI components for keyboard/input settings
    static void InitNetworkSetting(); // Initializes UI components for network/connectivity settings

    static void UpdateKeyboardSetting(); // Refreshes keyboard setting UI from data
    static void UpdateNetworkState(); // Refreshes network status indicators (IP, connection status)
    static void UpdateWiFiInfo(); // Updates WiFi specific UI elements (SSID list, etc.)

    static void SetDuktoServerIP(); // Logic to apply the configured Dukto server IP
};

#endif // _SETTING_SCREEN_H
