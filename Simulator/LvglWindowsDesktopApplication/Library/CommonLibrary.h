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
    void SetKeyboardKeyState(int vk, bool state);
    bool GetKeyboardCapsState();
    void SetListVkCode(const std::vector<std::pair<lv_obj_t*, int>>& list);
    const std::vector<std::pair<lv_obj_t*, int>>& GetListVkCode();
    const std::vector<std::pair<std::pair<int, int>, char>>& GetListSpacialChar();
    void CALLBACK AutoConfirmKey(HWND hwnd, UINT uMsg, UINT_PTR timerId, DWORD dwTime);
    void HardwareKeyboardProcess(HWND hwnd, int uMsg, int wParam, lv_event_code_t lParam);
    void ResetAll();
}

namespace config_lib
{
    std::wstring GetWStringConfig(std::wstring section, std::wstring key, std::wstring file);
}

namespace common_lib
{
    bool CheckInRangeNumber(int check, int min, int max);
}

#endif // !_COMMON_LIBRARY_H
