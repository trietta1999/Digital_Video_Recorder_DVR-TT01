#include "CommonData.h"

namespace system_data
{
    CData<HWND> WindowHandle;
    CData<SCREEN_NAME> CurrentKbScreen;
    CData<SCREEN_NAME> CurrentScreen;
    CData<std::string> CurrentDate;
    CData<std::string> CurrentTime;
    CData<std::string> FreeStorage;
    CData<std::string> FreeUSBStorage;
    CData<STATE_TYPE> CurrentState;
    CData<bool> IsTempVideoInfo;
    CData<double> TransferPercent;
    CData<bool> DeviceChange;
}

namespace current_videoinfo_data
{
    CData<std::string> VideoID;
    CData<std::string> VideoEvent;
    CData<std::string> VideoName;
    CData<std::string> VideoCategory;
    CData<std::string> VideoDesc;
    CData<std::string> VideoAuthor;
    CData<unsigned short> VideoDay;
    CData<unsigned short> VideoMonth;
    CData<unsigned short> VideoYear;
    CData<unsigned short> VideoHour;
    CData<unsigned short> VideoMinute;
    CData<unsigned short> VideoSecond;
}

namespace input_data
{
    CData<std::string> VideoEvent;
    CData<std::string> VideoName;
    CData<std::string> VideoCategory;
    CData<std::string> VideoDesc;
    CData<std::string> VideoAuthor;
    CData<std::string> VideoSearch;
    CData<short> VideoSearchType;
}

namespace setting_data
{
    CData<short> DateFormat;
    CData<short> DateSeparator;
    CData<short> InitAudState;
    CData<short> PreviewVideoState;
    CData<short> PlayVideoState;
    CData<short> KeyboardType;
    CData<short> T9AutoConfirmSpeed;
    CData<short> KeyboardExitShortcutState;
    CData<short> InsSpaceAfterPuncState;
    CData<short> AutoCloseBracketState;
    CData<short> NetworkType;
    CData<short> SSID;
    CData<short> DuktoIPPart12;
    CData<short> DuktoIPPart34;
}

namespace temp_data
{
    CData<std::string> VideoEvent;
    CData<std::string> VideoName;
    CData<std::string> VideoCategory;
    CData<std::string> VideoDesc;
    CData<std::string> VideoAuthor;
    CData<std::string> VideoSearch;

    CData<short> DateFormat;
    CData<short> DateSeparator;
    CData<short> InitAudState;
    CData<short> PreviewVideoState;
    CData<short> PlayVideoState;
    CData<short> KeyboardType;
    CData<short> T9AutoConfirmSpeed;
    CData<short> KeyboardExitShortcutState;
    CData<short> InsSpaceAfterPuncState;
    CData<short> AutoCloseBracketState;
    CData<short> NetworkType;
    CData<short> SSID;
    CData<std::string> WiFiPassword;
    CData<std::string> DuktoIP;
    CData<short> DuktoIPPart12;
    CData<short> DuktoIPPart34;
}

namespace debug_data
{
    CData<std::vector<std::string>> InputParamList;
}

void CommonDataUpdateAll()
{
    system_data::WindowHandle.ResetState();
    system_data::CurrentKbScreen.ResetState();
    system_data::CurrentScreen.ResetState();
    system_data::CurrentDate.ResetState();
    system_data::CurrentTime.ResetState();
    system_data::FreeStorage.ResetState();
    system_data::FreeUSBStorage.ResetState();
    system_data::CurrentState.ResetState();
    system_data::IsTempVideoInfo.ResetState();
    system_data::TransferPercent.ResetState();
    system_data::DeviceChange.ResetState();

    current_videoinfo_data::VideoID.ResetState();
    current_videoinfo_data::VideoEvent.ResetState();
    current_videoinfo_data::VideoName.ResetState();
    current_videoinfo_data::VideoCategory.ResetState();
    current_videoinfo_data::VideoDesc.ResetState();
    current_videoinfo_data::VideoAuthor.ResetState();
    current_videoinfo_data::VideoDay.ResetState();
    current_videoinfo_data::VideoMonth.ResetState();
    current_videoinfo_data::VideoYear.ResetState();
    current_videoinfo_data::VideoHour.ResetState();
    current_videoinfo_data::VideoMinute.ResetState();
    current_videoinfo_data::VideoSecond.ResetState();

    input_data::VideoEvent.ResetState();
    input_data::VideoName.ResetState();
    input_data::VideoCategory.ResetState();
    input_data::VideoDesc.ResetState();
    input_data::VideoAuthor.ResetState();
    input_data::VideoSearch.ResetState();
    input_data::VideoSearchType.ResetState();

    setting_data::DateFormat.ResetState();
    setting_data::DateSeparator.ResetState();
    setting_data::InitAudState.ResetState();
    setting_data::PreviewVideoState.ResetState();
    setting_data::PlayVideoState.ResetState();
    setting_data::KeyboardType.ResetState();
    setting_data::T9AutoConfirmSpeed.ResetState();
    setting_data::KeyboardExitShortcutState.ResetState();
    setting_data::InsSpaceAfterPuncState.ResetState();
    setting_data::AutoCloseBracketState.ResetState();
    setting_data::NetworkType.ResetState();
    setting_data::SSID.ResetState();
    setting_data::DuktoIPPart12.ResetState();
    setting_data::DuktoIPPart34.ResetState();

    temp_data::VideoEvent.ResetState();
    temp_data::VideoName.ResetState();
    temp_data::VideoCategory.ResetState();
    temp_data::VideoDesc.ResetState();
    temp_data::VideoAuthor.ResetState();

    temp_data::DateFormat.ResetState();
    temp_data::DateSeparator.ResetState();
    temp_data::InitAudState.ResetState();
    temp_data::PreviewVideoState.ResetState();
    temp_data::PlayVideoState.ResetState();
    temp_data::KeyboardType.ResetState();
    temp_data::T9AutoConfirmSpeed.ResetState();
    temp_data::KeyboardExitShortcutState.ResetState();
    temp_data::InsSpaceAfterPuncState.ResetState();
    temp_data::AutoCloseBracketState.ResetState();
    temp_data::NetworkType.ResetState();
    temp_data::SSID.ResetState();
    temp_data::WiFiPassword.ResetState();
    temp_data::DuktoIP.ResetState();
    temp_data::DuktoIPPart12.ResetState();
    temp_data::DuktoIPPart34.ResetState();
}
