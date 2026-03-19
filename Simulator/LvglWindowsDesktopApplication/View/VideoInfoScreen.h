#ifndef _VIDEOINFO_SCREEN_H
#define _VIDEOINFO_SCREEN_H

#include "BaseScreen.h"

class VideoInfoScreen : public BaseScreen
{
public:
    VideoInfoScreen(SCREEN_NAME screen);
    ~VideoInfoScreen() = default;

    static void VideoInfoScreenInit();

private:
    static void OnClickCancel(lv_event_t* event);
    static void OnClickOK(lv_event_t* event);
    static void OnClickInput(lv_event_t* event);
    static void OnLongPressInput(lv_event_t* event);

    static void UpdateVideoInfo();
};

#endif // _VIDEOINFO_SCREEN_H
