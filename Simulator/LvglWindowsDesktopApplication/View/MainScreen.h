#ifndef _MAIN_SCREEN_H
#define _MAIN_SCREEN_H

#include "BaseScreen.h"

class MainScreen : public BaseScreen
{
public:
    MainScreen(SCREEN_NAME screen);
    ~MainScreen();

private:
    // Event handlers for user interactions
    static void OnClickNew(lv_event_t* event); // Triggers creation of new video metadata entry
    static void OnClickClose(lv_event_t* event); // Placeholder for close operation
    static void OnClickOperator(lv_event_t* event); // Toggles recording/playback states
    static void OnClickVideoRecordList(lv_event_t* event); // Navigates to the video library screen
    static void OnClickStorageUSB(lv_event_t* event); // Navigates to external storage management
    static void OnClickSystemSetting(lv_event_t* event); // Navigates to the system settings screen
    static void OnClickVolume(lv_event_t* event); // Toggles system mute or volume overlay

    // UI synchronization methods
    static void UpdateDate(); // Updates the date label on the main dashboard
    static void UpdateTime(); // Updates the time label on the main dashboard
    static void UpdateStorage(); // Updates the internal storage capacity indicators
    static void UpdateButton(); // Updates dynamic labels/icons on action buttons (e.g., Record/Stop)
    static void UpdateExtDevice(); // Updates USB/External device connectivity icons
    static void UpdateSoundButton(); // Updates volume/mute button visual state
};

#endif // _MAIN_SCREEN_H
