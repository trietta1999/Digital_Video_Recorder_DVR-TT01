#include "CommonData.h"

namespace system_data
{
    CData<HWND> WindowHandle;
    CData<SCREEN_NAME> CurrentKbScreen;
    CData<SCREEN_NAME> CurrentScreen;
    CData<std::string> CurrentDate;
    CData<std::string> CurrentTime;
    CData<KEYBOARD_TYPE> KeyboardType;
    CData<short> T9ConfirmTimeout;
    CData<std::pair<double, double>> StorageInfo;
    CData<STATE_TYPE> CurrentState;
    CData<bool> IsTempVideoInfo;
    CData<bool> CurrentSoundState;
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
}

namespace temp_data
{
    CData<std::string> VideoEvent;
    CData<std::string> VideoName;
    CData<std::string> VideoCategory;
    CData<std::string> VideoDesc;
    CData<std::string> VideoAuthor;
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
    system_data::KeyboardType.ResetState();
    system_data::T9ConfirmTimeout.ResetState();
    system_data::StorageInfo.ResetState();
    system_data::CurrentState.ResetState();
    system_data::IsTempVideoInfo.ResetState();
    system_data::CurrentSoundState.ResetState();

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

    temp_data::VideoEvent.ResetState();
    temp_data::VideoName.ResetState();
    temp_data::VideoCategory.ResetState();
    temp_data::VideoDesc.ResetState();
    temp_data::VideoAuthor.ResetState();
}
