#include "ui.h"
#include "CommonData.h"
#include "CommonLibrary.h"
#include "ScreenMapping.h"
#include "SettingScreen.h"

#define SW_SET_DATA_CB(cdata) [](lv_event_t* e){ cdata.SetValue(IsButtonChecked((lv_obj_t*)(e->current_target))); }

static void SetButtonState(lv_obj_t* obj, short value)
{
    if (value)
    {
        lv_obj_add_state(obj, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_remove_state(obj, LV_STATE_CHECKED);
    }
}

static bool IsButtonChecked(lv_obj_t* obj)
{
    if ((lv_obj_get_state(obj) & LV_STATE_CHECKED) == LV_STATE_CHECKED)
    {
        return true;
    }

    return false;
}

SettingScreen::SettingScreen(SCREEN_NAME screen) : BaseScreen(screen)
{
    ListButtonCallback = {
        { ui_btnSettingCancel      , OnClickCancel                                       , LV_EVENT_CLICKED },
        { ui_btnSettingSave        , OnClickSave                                         , LV_EVENT_CLICKED },
        { ui_swInitAudState        , SW_SET_DATA_CB(temp_data::InitAudState)             , LV_EVENT_CLICKED},
        { ui_swPreviewVideo        , SW_SET_DATA_CB(temp_data::PreviewVideoState)        , LV_EVENT_CLICKED },
        { ui_swPlayVideo           , SW_SET_DATA_CB(temp_data::PlayVideoState)           , LV_EVENT_CLICKED },
        { ui_swKeyboardExitShortcut, SW_SET_DATA_CB(temp_data::KeyboardExitShortcutState), LV_EVENT_CLICKED },
        { ui_swInsSpaceAfterPunc   , SW_SET_DATA_CB(temp_data::InsSpaceAfterPuncState)   , LV_EVENT_CLICKED },
        { ui_swAutoCloseBracket    , SW_SET_DATA_CB(temp_data::AutoCloseBracketState)    , LV_EVENT_CLICKED },
    };

    ListDataUpdateCallback = {
        { []() { return temp_data::KeyboardType.GetState(); }, UpdateKeyboardSetting },
    };

    InitGeneralSetting();
    InitKeyboardSetting();
    InitNetworkSetting();
}

void SettingScreen::InitGeneralSetting()
{
    dropdownlist_lib::UpdateDropdownList({
        DROPDOWNLIST_NAME::DD_DATE_FORMAT,
        DROPDOWNLIST_NAME::DD_DATE_SEPARATOR,
        });

    SetButtonState(ui_swInitAudState, temp_data::InitAudState.GetValue());
    SetButtonState(ui_swPreviewVideo, temp_data::PreviewVideoState.GetValue());
    SetButtonState(ui_swPlayVideo, temp_data::PlayVideoState.GetValue());
}

void SettingScreen::InitKeyboardSetting()
{
    dropdownlist_lib::UpdateDropdownList({
        DROPDOWNLIST_NAME::DD_KEYBOARD_TYPE,
        DROPDOWNLIST_NAME::DD_T9_AUTO_CONFIRM_SPEED,
        });

    SetButtonState(ui_swKeyboardExitShortcut, temp_data::KeyboardExitShortcutState.GetValue());
    SetButtonState(ui_swInsSpaceAfterPunc, temp_data::InsSpaceAfterPuncState.GetValue());
    SetButtonState(ui_swAutoCloseBracket, temp_data::AutoCloseBracketState.GetValue());
}

void SettingScreen::InitNetworkSetting()
{

}

void SettingScreen::OnClickCancel(lv_event_t* event)
{
    ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::SCREEN_MAIN);
}

void SettingScreen::OnClickSave(lv_event_t* event)
{
    systemsetting_lib::SaveSetting();
    ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::SCREEN_MAIN);
}

void SettingScreen::UpdateKeyboardSetting()
{
    if (temp_data::KeyboardType.GetValue() != (short)dropdownlist_lib::DD_KEYBOARD_TYPE_e::Standard_keyboard)
    {
        lv_obj_remove_state(ui_lblT9AutoConfirm, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_dropT9AutoConfirm, LV_STATE_DISABLED);
    }
    else
    {
        lv_obj_add_state(ui_lblT9AutoConfirm, LV_STATE_DISABLED);
        lv_obj_add_state(ui_dropT9AutoConfirm, LV_STATE_DISABLED);
    }
}
