#ifndef _COMMON_DATA_H
#define _COMMON_DATA_H

#include "CommonDataType.h"
#include "CData.h"

#define COPY_CDATA_1TO2(a, b) b.SetValue(a.GetValue())

namespace system_data
{
    extern CData<HWND> WindowHandle;
    extern CData<SCREEN_NAME> CurrentKbScreen;
    extern CData<SCREEN_NAME> CurrentScreen;
    extern CData<std::string> CurrentDate;
    extern CData<std::string> CurrentTime;
    extern CData<KEYBOARD_TYPE> KeyboardType;
    extern CData<short> T9ConfirmTimeout;
    extern CData<std::string> FreeStorage;
    extern CData<std::string> FreeUSBStorage;
    extern CData<STATE_TYPE> CurrentState;
    extern CData<bool> IsTempVideoInfo;
    extern CData<bool> CurrentSoundState;
    extern CData<double> TransferPercent;
    extern CData<bool> DeviceChange;
}

namespace current_videoinfo_data
{
    extern CData<std::string> VideoID;
    extern CData<std::string> VideoEvent;
    extern CData<std::string> VideoName;
    extern CData<std::string> VideoCategory;
    extern CData<std::string> VideoDesc;
    extern CData<std::string> VideoAuthor;
    extern CData<unsigned short> VideoDay;
    extern CData<unsigned short> VideoMonth;
    extern CData<unsigned short> VideoYear;
    extern CData<unsigned short> VideoHour;
    extern CData<unsigned short> VideoMinute;
    extern CData<unsigned short> VideoSecond;
}

namespace input_data
{
    extern CData<std::string> VideoEvent;
    extern CData<std::string> VideoName;
    extern CData<std::string> VideoCategory;
    extern CData<std::string> VideoDesc;
    extern CData<std::string> VideoAuthor;
    extern CData<std::string> VideoSearch;
    extern CData<short> VideoSearchType;
}

namespace setting_data
{
    extern CData<short> DateFormat;
    extern CData<short> DateSeparator;
    extern CData<short> InitAudState;
    extern CData<short> PreviewVideoState;
    extern CData<short> PlayVideoState;
}

namespace temp_data
{
    extern CData<std::string> VideoEvent;
    extern CData<std::string> VideoName;
    extern CData<std::string> VideoCategory;
    extern CData<std::string> VideoDesc;
    extern CData<std::string> VideoAuthor;

    extern CData<short> DateFormat;
    extern CData<short> DateSeparator;
    extern CData<short> InitAudState;
    extern CData<short> PreviewVideoState;
    extern CData<short> PlayVideoState;
}

namespace debug_data
{
    extern CData<std::vector<std::string>> InputParamList;
}

void CommonDataUpdateAll();

#endif // !_COMMON_DATA_H
