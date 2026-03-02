#include "ui.h"
#include "ConfigMap.h"
#include "CommonData.h"
#include "CommonLibrary.h"
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
        { []() { return system_data::StorageInfo.GetState(); }, UpdateStorage },
    };

    // Get datetime
    timerDateTime = lv_timer_create([](lv_timer_t* timer) {
        SYSTEMTIME systime;
        ULARGE_INTEGER freeBytesAvailableToUser; // Available free space for the user
        ULARGE_INTEGER totalNumberOfBytes; // Total disk space
        ULARGE_INTEGER totalNumberOfFreeBytes; // Actual total free space
        double totalGB = 0;
        double usedGB = 0;
        char buffDate[MAX_PATH] = { 0 };
        char buffTime[MAX_PATH] = { 0 };
        auto drive = config_lib::GetWStringConfig(SYSTEM_SECTION, STORAGE_DRIVE, SYSTEM_CONFIG);

        GetLocalTime(&systime);

        if (GetDiskFreeSpaceExW(drive.c_str(), &freeBytesAvailableToUser, &totalNumberOfBytes, &totalNumberOfFreeBytes))
        {
            // Convert Bytes to GB (1 GB = 1024 * 1024 * 1024 Bytes)
            totalGB = (double)totalNumberOfBytes.QuadPart / (1024 * 1024 * 1024);
            usedGB = totalGB - (double)freeBytesAvailableToUser.QuadPart / (1024 * 1024 * 1024);
        }

        sprintf(buffDate, "%02d.%02d.%04d", systime.wDay, systime.wMonth, systime.wYear);
        sprintf(buffTime, "%02d.%02d.%02d", systime.wHour, systime.wMinute, systime.wSecond);

        system_data::CurrentDate.SetValue(buffDate);
        system_data::CurrentTime.SetValue(buffTime);
        system_data::StorageInfo.SetValue({ usedGB, totalGB });
        }, TIMECYCLE_1SEC, nullptr);
}

MainScreen::~MainScreen()
{
    lv_timer_del(timerDateTime);
    timerDateTime = nullptr;
}

void MainScreen::OnClickNew(lv_event_t* event)
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

void MainScreen::UpdateStorage()
{
    auto value = system_data::StorageInfo.GetValue();
    lv_label_set_text_fmt(ui_lblStorage, "%.1f/%.1f", value.first, value.second);
}
