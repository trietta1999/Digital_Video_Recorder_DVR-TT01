#ifndef _COMMON_DATA_H
#define _COMMON_DATA_H

#include "CommonDataType.h"
#include "CData.h"

#define COPY_CDATA_1TO2(a, b) b.SetValue(a.GetValue())

namespace system_data
{
    extern CData<HWND> WindowHandle; // Native handle of the main application window
    extern CData<SCREEN_NAME> CurrentKbScreen; // Current virtual keyboard screen context
    extern CData<SCREEN_NAME> CurrentScreen; // Current active application screen
    extern CData<std::string> CurrentDate; // Formatted current date string
    extern CData<std::string> CurrentTime; // Formatted current time string
    extern CData<std::string> FreeStorage; // Available internal storage space description
    extern CData<std::string> FreeUSBStorage; // Available external USB storage space description
    extern CData<STATE_TYPE> CurrentState; // Current system operation state (Record, Play, etc.)
    extern CData<bool> IsTempVideoInfo; // Flag indicating if current video info is non-persistent
    extern CData<double> TransferPercent; // Progress percentage for file transfer operations
    extern CData<bool> DeviceChange; // Flag indicating a hardware configuration change
}

namespace current_videoinfo_data
{
    extern CData<std::string> VideoID; // ID of the currently active or selected video
    extern CData<std::string> VideoEvent; // Event title of current video
    extern CData<std::string> VideoName; // Filename/Title of current video
    extern CData<std::string> VideoCategory; // Category of current video
    extern CData<std::string> VideoDesc; // Detailed description of current video
    extern CData<std::string> VideoAuthor; // Author/Operator of current video
    extern CData<unsigned short> VideoDay; // Recording day
    extern CData<unsigned short> VideoMonth; // Recording month
    extern CData<unsigned short> VideoYear; // Recording year
    extern CData<unsigned short> VideoHour; // Recording hour
    extern CData<unsigned short> VideoMinute; // Recording minute
    extern CData<unsigned short> VideoSecond; // Recording second
}

namespace input_data
{
    extern CData<std::string> VideoEvent; // UI input buffer for video event
    extern CData<std::string> VideoName; // UI input buffer for video name
    extern CData<std::string> VideoCategory; // UI input buffer for video category
    extern CData<std::string> VideoDesc; // UI input buffer for video description
    extern CData<std::string> VideoAuthor; // UI input buffer for video author
    extern CData<std::string> VideoSearch; // UI input buffer for search query
    extern CData<short> VideoSearchType; // Selected filter type for video search
}

namespace setting_data
{
    extern CData<short> DateFormat;
    extern CData<short> DateSeparator;
    extern CData<short> InitAudState;
    extern CData<short> PreviewVideoState;
    extern CData<short> PlayVideoState;
    extern CData<short> KeyboardType;
    extern CData<short> T9AutoConfirmSpeed;
    extern CData<short> KeyboardExitShortcutState;
    extern CData<short> InsSpaceAfterPuncState;
    extern CData<short> AutoCloseBracketState;
    extern CData<short> NetworkType;
    extern CData<short> SSID;
    extern CData<short> DuktoIPPart12;
    extern CData<short> DuktoIPPart34;
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
    extern CData<short> KeyboardType;
    extern CData<short> T9AutoConfirmSpeed;
    extern CData<short> KeyboardExitShortcutState;
    extern CData<short> InsSpaceAfterPuncState;
    extern CData<short> AutoCloseBracketState;
    extern CData<short> NetworkType;
    extern CData<short> SSID;
    extern CData<std::string> WiFiPassword;
    extern CData<std::string> DuktoIP;
    extern CData<short> DuktoIPPart12;
    extern CData<short> DuktoIPPart34;
}

namespace debug_data
{
    extern CData<std::vector<std::string>> InputParamList;
}

void CommonDataUpdateAll();

#endif // !_COMMON_DATA_H
