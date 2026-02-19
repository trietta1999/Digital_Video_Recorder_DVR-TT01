#ifndef _VIDEOINFO_SCREEN_H
#define _VIDEOINFO_SCREEN_H

#include "BaseScreen.h"

class VideoInfoScreen : public BaseScreen
{
public:
    VideoInfoScreen(SCREEN_NAME screen);
    ~VideoInfoScreen();

private:
    static void OnClickCancel(lv_obj_t* obj);
    static void OnClickOK(lv_obj_t* obj);

    //static void UpdateDate();
    //static void UpdateTime();
};

#endif // _VIDEOINFO_SCREEN_H
