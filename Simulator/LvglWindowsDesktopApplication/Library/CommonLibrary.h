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

namespace videoinfo_lib
{
    struct videoinfo_t
    {
        std::string videoEvent;
        std::string videoName;
        std::string videoCategory;
        std::string videoDesc;
        std::string videoAuthor;

        bool IsAllNone() const
        {
            if ((videoEvent == "")
                && (videoName == "")
                && (videoCategory == "")
                && (videoDesc == "")
                && (videoAuthor == "")
                )
            {
                return true;
            }

            return false;
        }

        bool IsAnyNone() const
        {
            if ((videoEvent == "")
                || (videoName == "")
                || (videoCategory == "")
                || (videoDesc == "")
                || (videoAuthor == "")
                )
            {
                return true;
            }

            return false;
        }
    };

    void WriteToTempData(const videoinfo_t& info);
    std::string GetTempIDFromTempData();
    videoinfo_t GetTempFromData();
    void WriteToNewData(const videoinfo_t& info);
}

namespace config_lib
{
    std::wstring GetWStringConfig(std::wstring section, std::wstring key, std::wstring file);
    void WriteWStringConfig(std::wstring section, std::wstring key, std::wstring file, std::wstring value);
}

namespace common_lib
{
    bool CheckInRangeNumber(int check, int min, int max);
    std::string JoinString(std::string delimeter, const std::vector<std::string>& list);
    void GetSystemStorageSize(double& totalGB, double& usedGB);
    std::wstring ConvertStringToWString(std::string input);
    std::string ConvertWStringToString(std::wstring input);
    std::string GenerateGUID();
}

#endif // !_COMMON_LIBRARY_H
