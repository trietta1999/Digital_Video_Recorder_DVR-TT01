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
}

namespace temp_data
{
    CData<std::string> VideoID;
    CData<std::string> VideoName;
    CData<std::string> VideoDesc;
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

    temp_data::VideoID.ResetState();
    temp_data::VideoName.ResetState();
    temp_data::VideoDesc.ResetState();
}
