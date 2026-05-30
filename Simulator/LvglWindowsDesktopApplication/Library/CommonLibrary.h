#ifndef _COMMON_LIBRARY_H
#define _COMMON_LIBRARY_H

#include <lvgl.h>
#include "CData.h"
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
        std::string videoID = "";
        std::string videoEvent = "";
        std::string videoName = "";
        std::string videoCategory = "";
        std::string videoDesc = "";
        std::string videoAuthor = "";
        SYSTEMTIME datetime = { 0 };

        bool IsAllNone() const
        {
            if (videoID.empty()
                && videoEvent.empty()
                && videoName.empty()
                && videoCategory.empty()
                && videoDesc.empty()
                && videoAuthor.empty()
                && !datetime.wYear
                && !datetime.wMonth
                && !datetime.wDay
                && !datetime.wHour
                && !datetime.wMinute
                && !datetime.wSecond
                )
            {
                return true;
            }

            return false;
        }

        bool IsAnyNoneForInput() const
        {
            if (videoEvent.empty()
                && videoName.empty()
                && videoCategory.empty()
                && videoDesc.empty()
                && videoAuthor.empty()
                )
            {
                return true;
            }

            return false;
        }
    };

    void CreateTempData(const videoinfo_t& info);
    void CreateNewData(std::string id, const videoinfo_t& info);
    void ClearTempData();
    void DeleteData(std::string id);
    videoinfo_t GetTempData();
    videoinfo_t GetExistData(std::string id);
    void CreateCurrentInfoFromInput();
    void CreateCurrentInfoFromInfo(const videoinfo_t& info);
    void UpdateCurrentInfoDateTime();
    bool CheckCurrentVideoInfoPathExist();
}

namespace videorecord_lib
{
    void StartExternalWindow(lv_obj_t* wnd, std::wstring screenName, std::string videoID);
    void StopExternalWindow();
    void SetSoundState();
    void ExecuteSeek(bool rewind, bool forward);
    void ExecutePause();
    void KillAllProcess();
}

namespace recordlist_lib
{
    std::vector<videoinfo_lib::videoinfo_t> GetVideoInfoListData();
    bool ExportVideoToExternalDrive(std::string id, std::string name, std::string driveLetter);
}

namespace dropdownlist_lib
{
    struct dropdown_info_t
    {
        lv_obj_t* dropdownObj;
        std::vector<std::string> options;
        CData<std::string>* data;
    };

    void SetupDropdownList();
    const dropdown_info_t& GetDropdownInfo(DROPDOWNLIST_NAME name);
    std::string GetDropdownSelected(DROPDOWNLIST_NAME name);
    void UpdateDropdownData(lv_obj_t* obj);
}

namespace config_lib
{
    std::wstring GetWStringConfig(std::wstring section, std::wstring key, std::wstring file);
    unsigned int GetUIntConfig(std::wstring section, std::wstring key, std::wstring file);
    unsigned short GetUShortConfig(std::wstring section, std::wstring key, std::wstring file);
    void WriteWStringConfig(std::wstring section, std::wstring key, std::wstring file, std::wstring value);
}

namespace storage_lib
{
    struct RemovableDriveInfo_t
    {
        std::string letter = "";
        std::string label = "";
        std::string totalGB = "";
        std::string freeGB = "";
    };

    std::string GetDriveFreeStorage(std::string driveLetter);
    void CollectExternalDrivesList();
    std::vector<RemovableDriveInfo_t> GetExternalDrivesList();
    std::vector<std::string> GetExternalLabelList();
    bool CopyFileWithProgress(std::wstring source, std::wstring dest);
}

namespace common_lib
{
    bool CheckInRangeNumber(int check, int min, int max);
    std::string JoinString(std::string delimeter, const std::vector<std::string>& list);
    SYSTEMTIME GetSystemDateTime();
    std::wstring ConvertStringToWString(std::string input);
    std::string ConvertWStringToString(std::wstring input);
    std::string GenerateGUID();
    std::wstring GetFullPath(std::wstring file);
    std::wstring GetSystemPath();
}

#endif // !_COMMON_LIBRARY_H
