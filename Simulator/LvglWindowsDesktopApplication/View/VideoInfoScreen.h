#ifndef _VIDEOINFO_SCREEN_H
#define _VIDEOINFO_SCREEN_H

#include "BaseScreen.h"

class VideoInfoScreen : public BaseScreen
{
public:
    VideoInfoScreen(SCREEN_NAME screen); // Constructor for VideoInfoScreen
    ~VideoInfoScreen() = default; // Default destructor

    static void VideoInfoScreenInit(); // Initializes the video information viewing/editing screen

private:
    static void OnClickCancel(lv_event_t* event); // Event handler for Cancel/Back button
    static void OnClickOK(lv_event_t* event); // Event handler for OK/Confirm button
    static void OnClickInput(lv_event_t* event); // Event handler for clicking an input field (opens keyboard)
    static void OnLongPressInput(lv_event_t* event); // Event handler for long pressing an input field (clears content)

    static void UpdateVideoInfo(); // Synchronizes UI labels/fields with current video data
};

#endif // _VIDEOINFO_SCREEN_H
