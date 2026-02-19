#ifndef _COMMON_DATA_H
#define _COMMON_DATA_H

#include "CommonDataType.h"
#include "CData.h"

namespace system_data
{
    extern CData<HWND> WindowHandle;
    extern CData<std::string> CurrentDate;
    extern CData<std::string> CurrentTime;
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
