#include "ui.h"
#include "CommonData.h"
#include "CommonLibrary.h"
#include "ScreenMapping.h"
#include "MainScreen.h"
#include "VideoInfoScreen.h"
#include "VideoInfoData.h" // @todo: wait video record fucntion

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
        { []() { return system_data::CurrentDate.GetState();     }, UpdateDate    },
        { []() { return system_data::CurrentTime.GetState();     }, UpdateTime    },
        { []() { return system_data::StorageInfo.GetState();     }, UpdateStorage },
        { []() { return system_data::CurrentState.GetState();    }, UpdateButton  },
        { []() { return system_data::IsTempVideoInfo.GetState(); }, UpdateButton  },
    };

    // Init button state
    lv_obj_add_state(ui_btnCloseVideo, LV_STATE_DISABLED);
    lv_obj_add_state(ui_btnRec, LV_STATE_DISABLED);
    lv_obj_add_state(ui_btnPlay, LV_STATE_DISABLED);
    lv_obj_add_state(ui_btnPause, LV_STATE_DISABLED);
    lv_obj_add_state(ui_btnStop, LV_STATE_DISABLED);

    if (!system_data::IsTempVideoInfo.GetValue())
    {
        // Get temp video info
        auto info = videoinfo_lib::GetTempData();

        // Data is available
        if (!info.IsAllNone())
        {
            videoinfo_lib::CreateCurrentInfoFromInfo(info);

            input_data::VideoEvent.SetValue(info.videoEvent);
            input_data::VideoName.SetValue(info.videoName);
            input_data::VideoCategory.SetValue(info.videoCategory);
            input_data::VideoDesc.SetValue(info.videoDesc);
            input_data::VideoAuthor.SetValue(info.videoAuthor);

            // Init temp video info
            system_data::IsTempVideoInfo.SetValue(true);
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

        systime = common_lib::GetSystemDateTime();
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

    // Clear temp video info
    videoinfo_lib::ClearTempData();

    // Clear current video info
    videoinfo_lib::videoinfo_t info;
    videoinfo_lib::CreateCurrentInfoFromInfo(info);

    system_data::IsTempVideoInfo.SetValue(false);

    // Call button event Stop
    event->current_target = ui_btnStop;
    OnClickOperator(event);
}

void MainScreen::OnClickOperator(lv_event_t* event)
{
    if (event->current_target == ui_btnRec)
    {
        videoinfo_lib::UpdateCurrentInfoDateTime();

        // Get current info
        videoinfo_lib::videoinfo_t info = {
            current_videoinfo_data::VideoID.GetValue(),
            current_videoinfo_data::VideoEvent.GetValue(),
            current_videoinfo_data::VideoName.GetValue(),
            current_videoinfo_data::VideoCategory.GetValue(),
            current_videoinfo_data::VideoDesc.GetValue(),
            current_videoinfo_data::VideoAuthor.GetValue(),
            {
                current_videoinfo_data::VideoYear.GetValue(),
                current_videoinfo_data::VideoMonth.GetValue(),
                0,
                current_videoinfo_data::VideoDay.GetValue(),
                current_videoinfo_data::VideoHour.GetValue(),
                current_videoinfo_data::VideoMinute.GetValue(),
                current_videoinfo_data::VideoSecond.GetValue(),
                0
            }
        };

        // Add new video info to data
        videoinfo_lib::CreateNewData(info.videoID, info);

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
    auto isTempVideoInfo = system_data::IsTempVideoInfo.GetValue();

    lv_obj_remove_state(ui_btnNewVideo, LV_STATE_DISABLED);
    lv_obj_remove_state(ui_btnCloseVideo, LV_STATE_DISABLED);
    lv_obj_remove_state(ui_btnRec, LV_STATE_DISABLED);
    lv_obj_remove_state(ui_btnPlay, LV_STATE_DISABLED);
    lv_obj_remove_state(ui_btnPause, LV_STATE_DISABLED);
    lv_obj_remove_state(ui_btnStop, LV_STATE_DISABLED);

    // Update New button
    if ((state == STATE_TYPE::S_RECORD)
        || (state == STATE_TYPE::S_PLAY)
        || (state == STATE_TYPE::S_PAUSE)
        )
    {
        lv_obj_add_state(ui_btnNewVideo, LV_STATE_DISABLED);
    }

    // Update Close button
    if (!isTempVideoInfo)
    {
        lv_obj_add_state(ui_btnCloseVideo, LV_STATE_DISABLED);
    }

    // Update Rec button
    if ((!isTempVideoInfo)
        || (state == STATE_TYPE::S_RECORD)
        || (state == STATE_TYPE::S_PLAY)
        || (state == STATE_TYPE::S_PAUSE)
        )
    {
        lv_obj_add_state(ui_btnRec, LV_STATE_DISABLED);
    }

    // Update Play button
    if (!videoinfo_lib::CheckCurrentVideoInfoPathExist()
        || (state == STATE_TYPE::S_RECORD)
        || (state == STATE_TYPE::S_PLAY)
        )
    {
        lv_obj_add_state(ui_btnPlay, LV_STATE_DISABLED);
    }

    // Update Pause button
    if ((state == STATE_TYPE::S_RECORD)
        || (state == STATE_TYPE::S_PAUSE)
        || (state == STATE_TYPE::S_STOP)
        )
    {
        lv_obj_add_state(ui_btnPause, LV_STATE_DISABLED);
    }

    // Update Stop button
    if (state == STATE_TYPE::S_STOP)
    {
        lv_obj_add_state(ui_btnStop, LV_STATE_DISABLED);
    }
}
