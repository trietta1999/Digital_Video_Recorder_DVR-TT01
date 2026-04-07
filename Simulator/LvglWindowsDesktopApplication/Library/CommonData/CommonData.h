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
    extern CData<std::pair<double, double>> StorageInfo;
    extern CData<STATE_TYPE> CurrentState;
    extern CData<bool> IsTempVideoInfo;
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
}

namespace temp_data
{
    extern CData<std::string> VideoEvent;
    extern CData<std::string> VideoName;
    extern CData<std::string> VideoCategory;
    extern CData<std::string> VideoDesc;
    extern CData<std::string> VideoAuthor;
}

namespace debug_data
{
    extern CData<std::vector<std::string>> InputParamList;
}

//extern CData<uint8_t> Brightness;
//extern CData<STATE_TYPE> CurrentState;
//extern CData<bool> GuiBlockState;
//extern CData<bool> SoundEnable;
//extern CData<bool> MusicEnable;
//extern CData<uint32_t> CurrentMusicIndex;
//extern CData<uint8_t> CurrentMusicPercent;
//extern CData<MUSIC_STATE_TYPE> MusicState;
//extern CData<std::vector<std::string>> Playlist;

void CommonDataUpdateAll();

#endif // !_COMMON_DATA_H
