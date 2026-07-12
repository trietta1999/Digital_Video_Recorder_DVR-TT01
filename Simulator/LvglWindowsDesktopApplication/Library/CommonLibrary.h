#ifndef _COMMON_LIBRARY_H
#define _COMMON_LIBRARY_H

#include <wlanapi.h>
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
    short GetAutoConfirmTimeMs();
    const std::vector<std::pair<lv_obj_t*, int>>& GetListVkCode();
    void SetListVkCode(const std::vector<std::pair<lv_obj_t*, int>>& list);
    void CALLBACK AutoConfirmKey(HWND hwnd, UINT uMsg, UINT_PTR timerId, DWORD dwTime);
    void HardwareKeyboardProcess(HWND hwnd, int uMsg, int wParam, lv_event_code_t lParam);
    LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    void SetupHookInternalNumpad();
    void StopHookInternalNumpad();
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
    void ExecuteSeek(bool rewind, bool forward);
    void ExecutePause();
    void KillAllProcess();
}

namespace recordlist_lib
{
    std::vector<videoinfo_lib::videoinfo_t> GetVideoInfoListData();
    bool ExportVideoToExternalDrive(std::string id, std::string name, std::string driveLetter);
    bool SendVideoToNetwork(std::string id, std::string name);
}

namespace dropdownlist_lib
{
    struct dropdown_info_t
    {
        lv_obj_t* dropdownObj;
        std::vector<std::string> options;
        CData<short>* data;
    };

    enum class DD_VIDEO_SEARCH_e
    {
        Event,
        Video_name,
        Category,
        Description,
        Author,
    };

    enum class DD_DATE_FORMAT_e
    {
        DDMMYYYY,
        MMDDYYYY,
        YYYYMMDD,
    };

    enum class DD_DATE_SEPARATOR_e
    {
        Period,
        Slash,
        Dash,
        Space,
    };

    enum class DD_KEYBOARD_TYPE_e
    {
        Internal_numpad,
        Standard_numpad,
        Standard_keyboard,
    };

    enum class DD_T9_AUTO_CONFIRM_SPEED_e
    {
        Slow,
        Normal,
        Fast,
    };

    void SetupDropdownList();
    void UpdateDropdownList(std::vector<DROPDOWNLIST_NAME> listName);
    dropdown_info_t& GetDropdownInfo(DROPDOWNLIST_NAME name);
    short GetDropdownSelectedIndex(DROPDOWNLIST_NAME name);
    void UpdateDropdownData(lv_obj_t* obj);
}

namespace systemsetting_lib
{
    struct setting_info_t
    {
        CData<short>* tempData;
        CData<short>* settingData;
        std::wstring configName;
        short configValue;
    };

    void SetupSystemData();
    void SetupSetting();
    void SetupTempSetting();
    void SaveSetting();
}

namespace network_lib
{
    struct ip_info_t
    {
        std::string ip = "0.0.0.0";
        std::string mask = "0.0.0.0";
        std::string gateway = "0.0.0.0";
    };

    struct lan_info_t
    {
        IFTYPE type;
        std::string name;
        std::string description;
        IF_OPER_STATUS status;
        unsigned long metric;
    };

    struct wifi_info_t
    {
        std::string ssid;
        std::string password;
        std::string auth;
        std::string cipher;
        WLAN_SIGNAL_QUALITY signalQuality;

        bool IsAnyNone() const
        {
            return (ssid.empty() || password.empty() || auth.empty() || cipher.empty());
        }
    };

    extern std::vector<lan_info_t> listLanInfo;
    extern std::vector<wifi_info_t> listWiFiInfo;

    void GetLANConnections();
    void GetWiFiSSIDs();
    std::string GetCipherAlgorithmString(DOT11_CIPHER_ALGORITHM cipher);
    std::string GetAuthAlgorithmString(DOT11_AUTH_ALGORITHM auth);
    ip_info_t GetNetworkIPByName(std::string targetAdapterName = "");
    std::string GetLocalHostName();
    std::vector<std::string> CreateNetworkTypeDropdownOptions();
    std::vector<std::string> CreateSSIDDropdownOptions();
    bool ConnectWiFiWithTemplate(const wifi_info_t& info);
    bool IsWiFiConnectedTo(std::string targetSsid = "");
    std::vector<short> CalculateIPFromCombinedNum(short part12, short part34);
    std::string CalculateIPStrFromCombinedNum(short part12, short part34);
};

namespace soundvolume_lib
{
    extern bool isMute;

    void CreateSoundVolumeWindow();
    void SetSystemVolume(int percent);
    int GetSystemVolume();
    void ChangeVolume(bool up, bool down);
    void SetMute(bool value);
    void ToggleMute();
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
    const std::vector<RemovableDriveInfo_t>& GetExternalDrivesList();
    const std::vector<std::string>& GetExternalLabelList();
    bool CopyFileWithProgress(std::wstring source, std::wstring dest);
}

namespace common_lib
{
    bool CheckInRangeNumber(long long check, long long min, long long max);
    bool CheckInRangeNumberEqual(long long check, long long min, long long max);
    std::string JoinString(std::string delimeter, const std::vector<std::string>& list);
    std::vector<std::string> SplitString(char delimiter, std::string input);
    std::string TrimString(std::string input);
    SYSTEMTIME GetSystemDateTime();
    std::wstring ConvertStringToWString(std::string input);
    std::string ConvertWStringToString(std::wstring input);
    std::string GenerateGUID();
    std::wstring GetFullPath(std::wstring file);
    std::wstring GetSystemPath();
    std::string ExecuteCMD(std::string cmd);
}

#endif // !_COMMON_LIBRARY_H
