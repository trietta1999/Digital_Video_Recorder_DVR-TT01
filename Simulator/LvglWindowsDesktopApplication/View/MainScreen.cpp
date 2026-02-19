#include "ui.h"
#include "CommonData.h"
#include "ScreenMapping.h"
#include "MainScreen.h"

static lv_timer_t* timerDateTime = nullptr;

MainScreen::MainScreen(SCREEN_NAME screen) : BaseScreen(screen)
{
    ListButtonCallback = {
        { ui_btnNewVideo, OnClickNew, LV_EVENT_CLICKED },
    };

    ListDataUpdateCallback = {
        { []() { return system_data::CurrentDate.GetState(); }, UpdateDate },
        { []() { return system_data::CurrentTime.GetState(); }, UpdateTime },
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

MainScreen::~MainScreen()
{
    lv_timer_del(timerDateTime);
    timerDateTime = nullptr;
}

void MainScreen::OnClickNew(lv_obj_t* obj)
{
    ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::VIDEO_INPUT_SCREEN);
}

void MainScreen::UpdateDate()
{
    lv_label_set_text(ui_lblDate, system_data::CurrentDate.GetValue().c_str());
}

void MainScreen::UpdateTime()
{
    lv_label_set_text(ui_lblTime, system_data::CurrentTime.GetValue().c_str());
}
