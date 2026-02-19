#include "ui.h"
#include "CommonData.h"
#include "ScreenMapping.h"
#include "VideoInfoScreen.h"

static lv_timer_t* timerDateTime = nullptr;

VideoInfoScreen::VideoInfoScreen(SCREEN_NAME screen) : BaseScreen(screen)
{
    ListButtonCallback = {
        { ui_btnVideoInputCancel, OnClickCancel, LV_EVENT_CLICKED },
        { ui_btnVideoInputOK    , OnClickOK    , LV_EVENT_CLICKED },
    };

    ListDataUpdateCallback = {
        //{ []() { return system_data::CurrentDate.GetState(); }, UpdateDate },
        //{ []() { return system_data::CurrentTime.GetState(); }, UpdateTime },
    };

    // Get datetime
    timerDateTime = lv_timer_create([](lv_timer_t* timer) {
        SYSTEMTIME systime;
        char buff1[MAX_PATH] = { 0 };
        char buff2[MAX_PATH] = { 0 };

        GetLocalTime(&systime);

        sprintf(buff1, "%02d.%02d.%04d", systime.wDay, systime.wMonth, systime.wYear);
        sprintf(buff2, "%02d.%02d.%02d", systime.wHour, systime.wMinute, systime.wSecond);

        system_data::CurrentDate.SetValue(buff1);
        system_data::CurrentTime.SetValue(buff2);

        }, TIMECYCLE_1SEC, nullptr);
}

VideoInfoScreen::~VideoInfoScreen()
{
    lv_timer_del(timerDateTime);
    timerDateTime = nullptr;
}

void VideoInfoScreen::OnClickCancel(lv_obj_t* obj)
{
    ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::MAIN_SCREEN);
}

void VideoInfoScreen::OnClickOK(lv_obj_t* obj)
{
    ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::MAIN_SCREEN);
}

//void VideoInfoScreen::UpdateDate()
//{
//    lv_label_set_text(ui_lblDate, system_data::CurrentDate.GetValue().c_str());
//}
//
//void VideoInfoScreen::UpdateTime()
//{
//    lv_label_set_text(ui_lblTime, system_data::CurrentTime.GetValue().c_str());
//}
