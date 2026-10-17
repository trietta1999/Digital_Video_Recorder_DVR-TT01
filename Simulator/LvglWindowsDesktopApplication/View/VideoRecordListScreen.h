#ifndef _VIDEORECORDLIST_SCREEN_H
#define _VIDEORECORDLIST_SCREEN_H

#include "BaseScreen.h"

class VideoRecordListScreen : public BaseScreen
{
public:
    VideoRecordListScreen(SCREEN_NAME screen);
    ~VideoRecordListScreen();

    static void OnClickItem(lv_event_t* event); // Static handler for clicking individual list rows

private:
    // Event handlers for UI buttons
    static void OnClickAllItem(lv_event_t* event); // Toggles 'Select All' state
    static void OnClickBack(lv_event_t* event);
    static void OnClickPageChange(lv_event_t* event); // Handles scrolling logic
    static void OnClickOperator(lv_event_t* event); // Handles Play/Stop/Pause/Mute
    static void OnDelete(lv_event_t* event); // Long-press confirm for deletion
    static void OnClickTransfer(lv_event_t* event); // Export to USB
    static void OnClickNetworkTransfer(lv_event_t* event); // Send via network
    static void OnClickVolume(lv_event_t* event);

    // Reactive UI update methods
    static void UpdateButton(); // Enables/disables controls based on selection/state
    static void UpdateTransferProgress(); // Synchronizes bar during file operations
    static void UpdateExtDevice(); // Refreshes USB drive list
    static void UpdateVideoInfoList(); // Rebuilds the UI list based on filters/data
    static void UpdateSoundButton();
};

#endif // _VIDEORECORDLIST_SCREEN_H
