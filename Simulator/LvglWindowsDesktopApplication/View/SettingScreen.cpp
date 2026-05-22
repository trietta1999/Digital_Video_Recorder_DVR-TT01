#include "ui.h"
#include "CommonData.h"
#include "CommonLibrary.h"
#include "ScreenMapping.h"
#include "SettingScreen.h"

SettingScreen::SettingScreen(SCREEN_NAME screen) : BaseScreen(screen)
{
    ListButtonCallback = {
        { ui_btnSettingCancel, OnClickCancel, LV_EVENT_CLICKED },
    };

    ListDataUpdateCallback = {
    };
}

SettingScreen::~SettingScreen()
{
}

void SettingScreen::OnClickCancel(lv_event_t* event)
{
    ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::SCREEN_MAIN);
}
