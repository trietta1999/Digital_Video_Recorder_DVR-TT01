#include "ui.h"
#include "CommonData.h"
#include "CommonLibrary.h"
#include "ScreenMapping.h"
#include "SettingScreen.h"

#include <wlanapi.h>

#define SW_SET_DATA_CB(cdata) [](lv_event_t* e){ cdata.SetValue(IsButtonChecked((lv_obj_t*)(e->current_target))); }

static lv_timer_t* timerCheckWiFiConnect = nullptr;
static bool isUpdateWiFiIP = false;
static std::vector<std::pair<lv_obj_t*, int>> listVkCode = {};

static void UpdateIPInput(std::string targetAdapterName)
{
    auto ipInfo = network_lib::GetNetworkIPByName(targetAdapterName);

    lv_label_set_text(ui_lblIpv4Addr, ipInfo.ip.c_str());
    lv_label_set_text(ui_lblSubnetAddr, ipInfo.mask.c_str());
    lv_label_set_text(ui_lblDefaultGateway, ipInfo.gateway.c_str());
}

static void RunWifiConnectCheck()
{
    timerCheckWiFiConnect = lv_timer_create([](lv_timer_t* timer) {
        if (network_lib::IsWiFiConnectedTo())
        {
            lv_obj_remove_state(ui_lblConnectState, LV_STATE_DISABLED);

            if (!isUpdateWiFiIP)
            {
                isUpdateWiFiIP = true;
                UpdateIPInput(network_lib::listLanInfo[temp_data::NetworkType.GetValue()].description);
            }
        }
        else
        {
            lv_obj_add_state(ui_lblConnectState, LV_STATE_DISABLED);
        }
        }, TIMECYCLE_5SEC, nullptr);
}

static void StopWifiConnectCheck()
{
    if (timerCheckWiFiConnect)
    {
        lv_timer_del(timerCheckWiFiConnect);
        timerCheckWiFiConnect = nullptr;
    }

    isUpdateWiFiIP = false;
}

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

static void SetEnableWiFiInput(bool state)
{
    if (state)
    {
        lv_obj_remove_state(ui_lblSSID, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_dropSSID, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_lblTitleWiFiPass, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_lblWifiPass, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_btnWifiPass, LV_STATE_DISABLED);
    }
    else
    {
        lv_obj_add_state(ui_lblSSID, LV_STATE_DISABLED);
        lv_obj_add_state(ui_lblTitleWiFiPass, LV_STATE_DISABLED);
        lv_obj_add_state(ui_dropSSID, LV_STATE_DISABLED);
        lv_obj_add_state(ui_lblWifiPass, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnWifiPass, LV_STATE_DISABLED);
    }
}

SettingScreen::SettingScreen(SCREEN_NAME screen) : BaseScreen(screen)
{
    ListButtonCallback = {
        { ui_btnSettingCancel      , OnClickCancel                                       , LV_EVENT_CLICKED       },
        { ui_btnSettingSave        , OnClickSave                                         , LV_EVENT_CLICKED       },
        { ui_btnSettingCancel      , OnClickCancel                                       , LV_EVENT_SHORT_CLICKED },
        { ui_btnSettingSave        , OnClickSave                                         , LV_EVENT_SHORT_CLICKED },
        { ui_swInitAudState        , SW_SET_DATA_CB(temp_data::InitAudState)             , LV_EVENT_CLICKED       },
        { ui_swPreviewVideo        , SW_SET_DATA_CB(temp_data::PreviewVideoState)        , LV_EVENT_CLICKED       },
        { ui_swPlayVideo           , SW_SET_DATA_CB(temp_data::PlayVideoState)           , LV_EVENT_CLICKED       },
        { ui_swKeyboardExitShortcut, SW_SET_DATA_CB(temp_data::KeyboardExitShortcutState), LV_EVENT_CLICKED       },
        { ui_swInsSpaceAfterPunc   , SW_SET_DATA_CB(temp_data::InsSpaceAfterPuncState)   , LV_EVENT_CLICKED       },
        { ui_swAutoCloseBracket    , SW_SET_DATA_CB(temp_data::AutoCloseBracketState)    , LV_EVENT_CLICKED       },
        { ui_btnWifiPass           , OnClickEdit                                         , LV_EVENT_SHORT_CLICKED },
        { ui_btnDuktoAddr          , OnClickEdit                                         , LV_EVENT_SHORT_CLICKED },
    };

    ListDataUpdateCallback = {
        { []() { return temp_data::KeyboardType.GetState(); }, UpdateKeyboardSetting },
        { []() { return temp_data::NetworkType.GetState();  }, UpdateNetworkState    },
        { []() { return temp_data::SSID.GetState();         }, UpdateWiFiInfo        },
        { []() { return temp_data::WiFiPassword.GetState(); }, UpdateWiFiInfo        },
        { []() { return temp_data::DuktoIP.GetState();      }, SetDuktoServerIP      },
    };

    listVkCode = {
        { ui_btnSettingCancel, VK_BROWSER_BACK },
        { ui_btnSettingSave  , VK_APPS         },
    };

    // Copy list VK code library
    keyboard_lib::SetListVkCode(listVkCode);
}

SettingScreen::~SettingScreen()
{
    StopWifiConnectCheck();
}

void SettingScreen::InitSettingScreen()
{
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
    // Get network info
    network_lib::GetLANConnections();
    network_lib::GetWiFiSSIDs();

    // Init network type
    auto& dropNetworkTypeInfo = dropdownlist_lib::GetDropdownInfo(DROPDOWNLIST_NAME::DD_NETWORK_TYPE);
    dropNetworkTypeInfo.options = network_lib::CreateNetworkTypeDropdownOptions();

    dropdownlist_lib::UpdateDropdownList({
        DROPDOWNLIST_NAME::DD_NETWORK_TYPE,
        DROPDOWNLIST_NAME::DD_SSID,
        });

    if (temp_data::NetworkType.GetValue() >= network_lib::listLanInfo.size())
    {
        temp_data::NetworkType.SetValue(0);
    }

    // Init SSID
    auto& dropSSIDInfo = dropdownlist_lib::GetDropdownInfo(DROPDOWNLIST_NAME::DD_SSID);
    dropSSIDInfo.options = network_lib::CreateSSIDDropdownOptions();
    dropSSIDInfo.options.insert(dropSSIDInfo.options.begin(), "-- No change --");
    temp_data::SSID.SetValue(0);

    // Init host name input
    lv_label_set_text(ui_lblHostName, network_lib::GetLocalHostName().c_str());

    // Init Dukto server IP input
    auto strIP = network_lib::CalculateIPStrFromCombinedNum(temp_data::DuktoIPPart12.GetValue(), temp_data::DuktoIPPart34.GetValue());
    temp_data::DuktoIP.SetValue(strIP);
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

void SettingScreen::OnClickEdit(lv_event_t* event)
{
    auto obj = (lv_obj_t*)(event->current_target);

    if (obj == ui_btnWifiPass)
    {
        ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::KBSCREEN_SSID_PASSWORD);
    }
    else if (obj == ui_btnDuktoAddr)
    {
        ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::KBSCREEN_DUKTO_ADDR);
    }
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

void SettingScreen::UpdateNetworkState()
{
    const auto& info = network_lib::listLanInfo[temp_data::NetworkType.GetValue()];

    lv_obj_remove_state(ui_lblConnectState, LV_STATE_CHECKED);

    if (info.status == IfOperStatusUp)
    {
        lv_obj_remove_state(ui_lblConnectState, LV_STATE_DISABLED);
    }
    else
    {
        SetEnableWiFiInput(false);
        StopWifiConnectCheck();
        lv_obj_add_state(ui_lblConnectState, LV_STATE_DISABLED);
    }

    if (info.type == IF_TYPE_IEEE80211)
    {
        SetEnableWiFiInput(true);
        RunWifiConnectCheck();
    }
    else
    {
        SetEnableWiFiInput(false);
        StopWifiConnectCheck();
    }

    UpdateIPInput(info.description);
}

void SettingScreen::UpdateWiFiInfo()
{
    if (temp_data::SSID.GetValue() > 0)
    {
        auto pass = temp_data::WiFiPassword.GetValue();
        auto& wifiInfo = network_lib::listWiFiInfo[temp_data::SSID.GetValue() - 1];
        wifiInfo.password = pass;

        lv_label_set_text(ui_lblWifiPass, pass.c_str());

        StopWifiConnectCheck();
        UpdateIPInput(""); // Empty adapter name for clearing IP input
        network_lib::ConnectWiFiWithTemplate(wifiInfo);
        RunWifiConnectCheck();
    }
}

void SettingScreen::SetDuktoServerIP()
{
    auto value = temp_data::DuktoIP.GetValue();
    auto periodCount = std::count(value.begin(), value.end(), '.');
    bool isAllDigit = true;
    std::string part1 = "";
    std::string part2 = "";
    std::string part3 = "";
    std::string part4 = "";

    temp_data::DuktoIPPart12.SetValue(0);
    temp_data::DuktoIPPart34.SetValue(0);

    if (periodCount == 3)
    {
        auto splits = common_lib::SplitString('.', value);

        if (splits.size() == 4)
        {
            // Check if all part are digits
            for (const auto& part : splits)
            {
                auto temp = common_lib::TrimString(part);

                if (!std::all_of(temp.begin(), temp.end(), [](unsigned char c) { return std::isdigit(c); }))
                {
                    isAllDigit = false;
                    break;
                }
            }

            // Check part of IP to data
            if (isAllDigit)
            {
                part1 = common_lib::TrimString(splits[0]);
                part2 = common_lib::TrimString(splits[1]);
                part3 = common_lib::TrimString(splits[2]);
                part4 = common_lib::TrimString(splits[3]);

                short part12 = static_cast<short>((std::stoi(part1) << 8) + std::stoi(part2));
                short part34 = static_cast<short>((std::stoi(part3) << 8) + std::stoi(part4));

                temp_data::DuktoIPPart12.SetValue(part12);
                temp_data::DuktoIPPart34.SetValue(part34);
                temp_data::DuktoIP.SetValue(network_lib::CalculateIPStrFromCombinedNum(part12, part34));
            }
        }
    }

    lv_label_set_text_fmt(ui_lblDuktoAddr, "%s.%s.%s.%s", part1.c_str(), part2.c_str(), part3.c_str(), part4.c_str());
}
