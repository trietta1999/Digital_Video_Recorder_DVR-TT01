#ifndef _COMMON_LIBRARY_H
#define _COMMON_LIBRARY_H

#include <wlanapi.h>
#include "CData.h"
#include "CommonDataType.h"

namespace keyboard_lib
{
    void SendKeyMessage(int keycode, lv_event_code_t event); // Sends a custom keyboard message to the main window
    void CalculateInputChar(int keycode, lv_event_code_t event); // Logic to determine character input from key events
    char GetInputChar(); // Retrieves the last directly input character
    char GetCombineChar(); // Retrieves the character formed by T9 combination
    void SetKeyboardKeyState(int vk, bool state); // Programmatically sets the state (up/down) of a virtual key
    bool GetKeyboardCapsState(); // Checks the current status of Caps Lock/Shift logic
    short GetAutoConfirmTimeMs(); // Gets the timeout duration for T9 auto-confirmation
    const std::vector<std::pair<lv_obj_t*, int>>& GetListVkCode(); // Retrieves the map of LVGL objects to virtual key codes
    void SetListVkCode(const std::vector<std::pair<lv_obj_t*, int>>& list); // Sets the map of LVGL objects to virtual key codes
    void CALLBACK AutoConfirmKey(HWND hwnd, UINT uMsg, UINT_PTR timerId, DWORD dwTime); // Timer callback to confirm T9 input
    void HardwareKeyboardProcess(HWND hwnd, int uMsg, int wParam, lv_event_code_t lParam); // Handles physical keyboard messages
    LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam); // Hook procedure for intercepting keyboard input
    void SetupHookInternalNumpad(); // Installs the low-level keyboard hook
    void StopHookInternalNumpad(); // Uninstalls the low-level keyboard hook
    void ResetAll(); // Resets all keyboard input buffers and states
}

namespace videoinfo_lib
{
    struct videoinfo_t
    {
        std::string videoID = ""; // Unique identifier for the video
        std::string videoEvent = ""; // Event or title associated with the video
        std::string videoName = ""; // Filename or display name
        std::string videoCategory = ""; // Video classification
        std::string videoDesc = ""; // Detailed description
        std::string videoAuthor = ""; // Creator or author name
        SYSTEMTIME datetime = { 0 }; // Timestamp of the video

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

    void CreateTempData(const videoinfo_t& info); // Temporarily stores video info in the system
    void CreateNewData(std::string id, const videoinfo_t& info); // Saves video info persistently with a specific ID
    void ClearTempData(); // Clears temporary video info data
    void DeleteData(std::string id); // Deletes video info entry by ID
    videoinfo_t GetTempData(); // Retrieves currently stored temporary video info
    videoinfo_t GetExistData(std::string id); // Retrieves persistent video info by ID
    void CreateCurrentInfoFromInput(); // Populates current video data from UI input fields
    void CreateCurrentInfoFromInfo(const videoinfo_t& info); // Sets current video data from a struct
    void UpdateCurrentInfoDateTime(); // Updates the timestamp in current video info to now
    bool CheckCurrentVideoInfoPathExist(); // Checks if the physical video file exists on disk
}

namespace videorecord_lib
{
    void StartExternalWindow(lv_obj_t* wnd, std::wstring screenName, std::string videoID); // Launches recording process in a separate window
    void StopExternalWindow(); // Terminates the recording window/process
    void ExecuteSeek(bool rewind, bool forward); // Sends seek commands to the video player
    void ExecutePause(); // Sends pause/resume command to the video player
    void KillAllProcess(); // Forcibly closes all associated video processes
}

namespace recordlist_lib
{
    std::vector<videoinfo_lib::videoinfo_t> GetVideoInfoListData(); // Fetches the list of all recorded videos
    bool ExportVideoToExternalDrive(std::string id, std::string name, std::string driveLetter); // Copies video to a USB drive
    bool SendVideoToNetwork(std::string id, std::string name); // Uploads video via network
}

namespace dropdownlist_lib
{
    struct dropdown_info_t
    {
        lv_obj_t* dropdownObj; // LVGL dropdown widget object
        std::vector<std::string> options; // List of strings for dropdown options
        CData<short>* data; // Pointer to the data variable linked to selection
    };

    enum class DD_VIDEO_SEARCH_e
    {
        Event, // Search by event name
        Video_name, // Search by file/video name
        Category, // Search by category
        Description, // Search by description text
        Author, // Search by author name
    };

    enum class DD_DATE_FORMAT_e
    {
        DDMMYYYY, // Day-Month-Year
        MMDDYYYY, // Month-Day-Year
        YYYYMMDD, // Year-Month-Day
    };

    enum class DD_DATE_SEPARATOR_e
    {
        Period, // Separator '.'
        Slash, // Separator '/'
        Dash, // Separator '-'
        Space, // Separator ' '
    };

    enum class DD_KEYBOARD_TYPE_e
    {
        Internal_numpad, // Custom 3x3 layout
        Standard_numpad, // Physical numpad layout
        Standard_keyboard, // Full QWERTY layout
    };

    enum class DD_T9_AUTO_CONFIRM_SPEED_e
    {
        Slow, // Long timeout for confirmation
        Normal, // Standard timeout for confirmation
        Fast, // Short timeout for confirmation
    };

    void SetupDropdownList(); // Initializes all dropdown components
    void UpdateDropdownList(std::vector<DROPDOWNLIST_NAME> listName); // Refreshes content of specific dropdowns
    dropdown_info_t& GetDropdownInfo(DROPDOWNLIST_NAME name); // Retrieves metadata for a specific dropdown
    short GetDropdownSelectedIndex(DROPDOWNLIST_NAME name); // Gets the current index of a dropdown's selection
    void UpdateDropdownData(lv_obj_t* obj); // Synchronizes CData with current UI selection
}

namespace systemsetting_lib
{
    struct setting_info_t
    {
        CData<short>* tempData; // Pointer to temporary/unsaved setting data
        CData<short>* settingData; // Pointer to persistent setting data
        std::wstring configName; // Key name in config file
        short configValue; // Default value
    };

    void SetupSystemData(); // Initializes system-level data structures
    void SetupSetting(); // Loads persistent settings from config
    void SetupTempSetting(); // Clones persistent settings to temporary buffers
    void SaveSetting(); // Commits temporary settings to persistent storage and file
}

namespace network_lib
{
    struct ip_info_t
    {
        std::string ip = "0.0.0.0"; // IP address string
        std::string mask = "0.0.0.0"; // Subnet mask string
        std::string gateway = "0.0.0.0"; // Default gateway string
    };

    struct lan_info_t
    {
        IFTYPE type; // Interface type (Ethernet, etc.)
        std::string name; // Adapter name
        std::string description; // Adapter description
        IF_OPER_STATUS status; // Connection status
        unsigned long metric; // Network metric priority
    };

    struct wifi_info_t
    {
        std::string ssid; // Service Set Identifier (Network Name)
        std::string password; // Network security key
        std::string auth; // Authentication algorithm
        std::string cipher; // Cipher algorithm
        WLAN_SIGNAL_QUALITY signalQuality; // Signal strength percentage

        bool IsAnyNone() const
        {
            return (ssid.empty() || password.empty() || auth.empty() || cipher.empty());
        }
    };

    extern std::vector<lan_info_t> listLanInfo; // Global list of LAN interfaces
    extern std::vector<wifi_info_t> listWiFiInfo; // Global list of available WiFi networks

    void GetLANConnections(); // Enumerates physical network adapters
    void GetWiFiSSIDs(); // Scans for available WiFi networks
    std::string GetCipherAlgorithmString(DOT11_CIPHER_ALGORITHM cipher); // Converts enum to string
    std::string GetAuthAlgorithmString(DOT11_AUTH_ALGORITHM auth); // Converts enum to string
    ip_info_t GetNetworkIPByName(std::string targetAdapterName = ""); // Retrieves IP info for a specific adapter
    std::string GetLocalHostName(); // Retrieves the machine's hostname
    std::vector<std::string> CreateNetworkTypeDropdownOptions(); // Generates options list for network UI
    std::vector<std::string> CreateSSIDDropdownOptions(); // Generates SSID list for dropdown UI
    bool ConnectWiFiWithTemplate(const wifi_info_t& info); // Connects to WiFi using a generated profile
    bool IsWiFiConnectedTo(std::string targetSsid = ""); // Checks if connected to a specific SSID
    std::vector<short> CalculateIPFromCombinedNum(short part12, short part34); // Decomposes packed IP parts
    std::string CalculateIPStrFromCombinedNum(short part12, short part34); // Converts packed IP to string format
};

namespace soundvolume_lib
{
    extern bool isMute; // Global mute state

    void CreateSoundVolumeWindow(); // Displays the volume control UI
    void SetSystemVolume(int percent); // Sets system-wide audio level (0-100)
    int GetSystemVolume(); // Reads current system-wide audio level
    void ChangeVolume(bool up, bool down); // Increments or decrements volume
    void SetMute(bool value); // Sets system mute state
    void ToggleMute(); // Toggles between mute and sound
}

namespace config_lib
{
    std::wstring GetWStringConfig(std::wstring section, std::wstring key, std::wstring file); // Reads a string from INI config
    unsigned int GetUIntConfig(std::wstring section, std::wstring key, std::wstring file); // Reads an unsigned int from INI config
    unsigned short GetUShortConfig(std::wstring section, std::wstring key, std::wstring file); // Reads an unsigned short from INI config
    void WriteWStringConfig(std::wstring section, std::wstring key, std::wstring file, std::wstring value); // Writes a string to INI config
}

namespace storage_lib
{
    struct RemovableDriveInfo_t
    {
        std::string letter = ""; // Drive letter (e.g., E:)
        std::string label = ""; // Drive volume label
        std::string totalGB = ""; // Total capacity in GB string
        std::string freeGB = ""; // Available space in GB string
    };

    std::string GetDriveFreeStorage(std::string driveLetter); // Gets free space for a specific drive
    void CollectExternalDrivesList(); // Enumerates all removable drives
    const std::vector<RemovableDriveInfo_t>& GetExternalDrivesList(); // Retrieves the list of removable drives
    const std::vector<std::string>& GetExternalLabelList(); // Retrieves the labels of removable drives
    bool CopyFileWithProgress(std::wstring source, std::wstring dest); // Copies file and updates global progress
}

namespace common_lib
{
    bool CheckInRangeNumber(long long check, long long min, long long max); // Validates number is within range (exclusive)
    bool CheckInRangeNumberEqual(long long check, long long min, long long max); // Validates number is within range (inclusive)
    std::string JoinString(std::string delimeter, const std::vector<std::string>& list); // Joins vector into a string
    std::vector<std::string> SplitString(char delimiter, std::string input); // Splits string into a vector
    std::string TrimString(std::string input); // Removes leading/trailing whitespace
    SYSTEMTIME GetSystemDateTime(); // Gets current local system time
    std::wstring ConvertStringToWString(std::string input); // Converts UTF-8 string to wide string
    std::string ConvertWStringToString(std::wstring input); // Converts wide string to UTF-8 string
    std::string GenerateGUID(); // Generates a unique GUID string
    std::wstring GetFullPath(std::wstring file); // Gets absolute path of a file in application directory
    std::wstring GetSystemPath(); // Gets the application's base directory
    std::string ExecuteCMD(std::string cmd); // Executes a shell command and returns output
}

#endif // !_COMMON_LIBRARY_H
