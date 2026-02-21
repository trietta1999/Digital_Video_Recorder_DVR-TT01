#ifndef _COMMON_DATA_H
#define _COMMON_DATA_H

#include "CommonDataType.h"
#include "CData.h"

namespace system_data
{
    extern CData<HWND> WindowHandle;
    extern CData<SCREEN_NAME> CurrentKbScreen;
    extern CData<SCREEN_NAME> CurrentScreen;
    extern CData<std::string> CurrentDate;
    extern CData<std::string> CurrentTime;
    extern CData<KEYBOARD_TYPE> KeyboardType;
    extern CData<short> T9ConfirmTimeout;
}

namespace temp_data
{
    extern CData<std::string> VideoID;
    extern CData<std::string> VideoName;
    extern CData<std::string> VideoDesc;
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
