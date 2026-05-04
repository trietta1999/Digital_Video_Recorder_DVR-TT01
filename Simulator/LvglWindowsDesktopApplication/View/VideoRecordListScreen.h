#ifndef _VIDEORECORDLIST_SCREEN_H
#define _VIDEORECORDLIST_SCREEN_H

#include "BaseScreen.h"

class VideoRecordListScreen : public BaseScreen
{
public:
    VideoRecordListScreen(SCREEN_NAME screen);
    ~VideoRecordListScreen();

    static void OnClickItem(lv_event_t* event);

private:
    static void OnClickAllItem(lv_event_t* event);
    static void OnClickBack(lv_event_t* event);
    static void OnClickPageChange(lv_event_t* event);
    static void OnClickOperator(lv_event_t* event);
    static void OnDelete(lv_event_t* event);

    static void UpdateButton();
};

#endif // _VIDEORECORDLIST_SCREEN_H
