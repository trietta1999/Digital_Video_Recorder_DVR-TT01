#ifndef _COMMON_LIBRARY_H
#define _COMMON_LIBRARY_H

#include <lvgl.h>
#include "CommonDataType.h"

namespace keyboard_lib
{
    void SendKeyMessage(int keycode, lv_event_code_t event);
    void CalculateInputChar(int keycode, lv_event_code_t event);
    char GetInputChar();
    char GetCombineChar();
    void SetShiftState(bool state);
    void SetListVkCode(const std::vector<std::pair<lv_obj_t*, int>>& list);
    const std::vector<std::pair<lv_obj_t*, int>>& GetListVkCode();
    void HardwareKeyboardProcess(int uMsg, int wParam, lv_event_code_t lParam);
    void ResetAll();
}

namespace common_lib
{
    bool CheckInRangeNumber(int check, int min, int max);
}

#endif // !_COMMON_LIBRARY_H
