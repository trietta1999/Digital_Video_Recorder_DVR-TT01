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
    CData<bool> TempVideoInfo;
    CData<std::string> TempVideoID;
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
    system_data::TempVideoInfo.ResetState();
    system_data::TempVideoID.ResetState();

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
