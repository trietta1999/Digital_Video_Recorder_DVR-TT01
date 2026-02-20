#ifndef _VIDEOINFO_SCREEN_H
#define _VIDEOINFO_SCREEN_H

#include "BaseScreen.h"

class VideoInfoScreen : public BaseScreen
{
public:
    VideoInfoScreen(SCREEN_NAME screen);
    ~VideoInfoScreen() = default;

private:
    static std::vector<std::pair<lv_obj_t*, SCREEN_NAME>> listKbScreenName;

    static void OnClickCancel(lv_obj_t* obj);
    static void OnClickOK(lv_obj_t* obj);
    static void OnClickInput(lv_obj_t* obj);
    static void OnLongPressInput(lv_obj_t* obj);

    static void UpdateVideoInfo();
};

#endif // _VIDEOINFO_SCREEN_H
