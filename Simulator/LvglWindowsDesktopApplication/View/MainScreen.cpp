#include "ui.h"
#include "CommonData.h"
#include "CommonLibrary.h"
#include "ScreenMapping.h"
#include "MainScreen.h"
#include "VideoInfoScreen.h"

static lv_timer_t* timerGetSystemData = nullptr;

MainScreen::MainScreen(SCREEN_NAME screen) : BaseScreen(screen)
{
    ListButtonCallback = {
        { ui_btnNewVideo  , OnClickNew     , LV_EVENT_CLICKED },
        { ui_btnCloseVideo, OnClickClose   , LV_EVENT_CLICKED },
        { ui_btnRec       , OnClickOperator, LV_EVENT_CLICKED },
        { ui_btnPlay      , OnClickOperator, LV_EVENT_CLICKED },
        { ui_btnPause     , OnClickOperator, LV_EVENT_CLICKED },
        { ui_btnStop      , OnClickOperator, LV_EVENT_CLICKED },
    };

    ListDataUpdateCallback = {
        { []() { return system_data::CurrentDate.GetState();   }, UpdateDate    },
        { []() { return system_data::CurrentTime.GetState();   }, UpdateTime    },
        { []() { return system_data::StorageInfo.GetState();   }, UpdateStorage },
        { []() { return system_data::CurrentState.GetState();  }, UpdateButton  },
        { []() { return system_data::TempVideoInfo.GetState(); }, UpdateButton  },
    };

    // Init button state
    lv_obj_add_state(ui_btnCloseVideo, LV_STATE_DISABLED);
    lv_obj_add_state(ui_btnRec, LV_STATE_DISABLED);
    lv_obj_add_state(ui_btnPlay, LV_STATE_DISABLED);
    lv_obj_add_state(ui_btnPause, LV_STATE_DISABLED);
    lv_obj_add_state(ui_btnStop, LV_STATE_DISABLED);

    if (!system_data::TempVideoInfo.GetValue())
    {
        // Get temp video info
        auto info = videoinfo_lib::GetTempFromData();

        // Data is available
        if (!info.IsAllNone())
        {
            input_data::VideoEvent.SetValue(info.videoEvent);
            input_data::VideoName.SetValue(info.videoName);
            input_data::VideoCategory.SetValue(info.videoCategory);
            input_data::VideoDesc.SetValue(info.videoDesc);
            input_data::VideoAuthor.SetValue(info.videoAuthor);

            // Init temp video info
            system_data::TempVideoInfo.SetValue(true);
            system_data::TempVideoID.SetValue(videoinfo_lib::GetTempIDFromTempData());
        }
    }

    // Init state
    system_data::CurrentState.SetValue(STATE_TYPE::S_STOP);

    // Create get system timer
    timerGetSystemData = lv_timer_create([](lv_timer_t* timer) {
        SYSTEMTIME systime;
        double totalGB = 0;
        double usedGB = 0;
        char buffDate[MAX_PATH] = { 0 };
        char buffTime[MAX_PATH] = { 0 };

        GetLocalTime(&systime);
        common_lib::GetSystemStorageSize(totalGB, usedGB);

        sprintf(buffDate, "%02d.%02d.%04d", systime.wDay, systime.wMonth, systime.wYear); // @todo: wait setting
        sprintf(buffTime, "%02d.%02d.%02d", systime.wHour, systime.wMinute, systime.wSecond); // @todo: wait setting

        system_data::CurrentDate.SetValue(buffDate);
        system_data::CurrentTime.SetValue(buffTime);
        system_data::StorageInfo.SetValue({ usedGB, totalGB });
        }, TIMECYCLE_1SEC, nullptr);
}

MainScreen::~MainScreen()
{
    lv_timer_del(timerGetSystemData);
    timerGetSystemData = nullptr;
}

void MainScreen::OnClickNew(lv_event_t* event)
{
    ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::SCREEN_VIDEO_INPUT);
    VideoInfoScreen::VideoInfoScreenInit();
}

void MainScreen::OnClickClose(lv_event_t* event)
{
    // Clear input data
    input_data::VideoEvent.SetValue("");
    input_data::VideoName.SetValue("");
    input_data::VideoCategory.SetValue("");
    input_data::VideoDesc.SetValue("");
    input_data::VideoAuthor.SetValue("");
    system_data::TempVideoInfo.SetValue(false);
    system_data::TempVideoID.SetValue("");

    // Clear temp video info
    videoinfo_lib::videoinfo_t info = { "", "", "", "", "", };
    videoinfo_lib::WriteToTempData(info);

    lv_obj_add_state(ui_btnCloseVideo, LV_STATE_DISABLED);
    lv_obj_add_state(ui_btnRec, LV_STATE_DISABLED);
}

void MainScreen::OnClickOperator(lv_event_t* event)
{
    if (event->current_target == ui_btnRec)
    {
        // Add new video info to data
        videoinfo_lib::videoinfo_t info = {
            input_data::VideoEvent.GetValue(),
            input_data::VideoName.GetValue(),
            input_data::VideoCategory.GetValue(),
            input_data::VideoDesc.GetValue(),
            input_data::VideoAuthor.GetValue(),
        };

        videoinfo_lib::WriteToNewData(info);

        system_data::TempVideoInfo.SetValue(false);
        system_data::CurrentState.SetValue(STATE_TYPE::S_RECORD);
    }
    else if (event->current_target == ui_btnPlay)
    {
        system_data::CurrentState.SetValue(STATE_TYPE::S_PLAY);
    }
    else if (event->current_target == ui_btnPause)
    {
        system_data::CurrentState.SetValue(STATE_TYPE::S_PAUSE);
    }
    else if (event->current_target == ui_btnStop)
    {
        system_data::CurrentState.SetValue(STATE_TYPE::S_STOP);
    }
}

void MainScreen::UpdateDate()
{
    lv_label_set_text(ui_lblDate, system_data::CurrentDate.GetValue().c_str());
}

void MainScreen::UpdateTime()
{
    lv_label_set_text(ui_lblTime, system_data::CurrentTime.GetValue().c_str());
}

void MainScreen::UpdateStorage()
{
    auto value = system_data::StorageInfo.GetValue();
    lv_label_set_text_fmt(ui_lblStorage, "%.1f/%.1f", value.first, value.second);
}

void MainScreen::UpdateButton()
{
    auto state = system_data::CurrentState.GetValue();
    auto tempVideoInfo = system_data::TempVideoInfo.GetValue();

    if (state == STATE_TYPE::S_RECORD)
    {
        lv_obj_add_state(ui_btnNewVideo, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnCloseVideo, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnRec, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnPlay, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnPause, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_btnStop, LV_STATE_DISABLED);
    }
    else if (state == STATE_TYPE::S_PLAY)
    {
        lv_obj_add_state(ui_btnNewVideo, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnCloseVideo, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnRec, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnPlay, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_btnPause, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_btnStop, LV_STATE_DISABLED);
    }
    else if (state == STATE_TYPE::S_PAUSE)
    {
        lv_obj_add_state(ui_btnNewVideo, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnCloseVideo, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnRec, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_btnPlay, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnPause, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_btnStop, LV_STATE_DISABLED);
    }
    else if (state == STATE_TYPE::S_STOP)
    {
        lv_obj_remove_state(ui_btnNewVideo, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_btnCloseVideo, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_btnRec, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_btnPlay, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnPause, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnStop, LV_STATE_DISABLED);
    }

    if (tempVideoInfo)
    {
        lv_obj_remove_state(ui_btnCloseVideo, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_btnRec, LV_STATE_DISABLED);
    }
    else
    {
        lv_obj_add_state(ui_btnCloseVideo, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnRec, LV_STATE_DISABLED);
    }
}
