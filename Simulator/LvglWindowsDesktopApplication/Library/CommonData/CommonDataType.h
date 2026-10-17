#ifndef _COMMON_DATATYPE_H
#define _COMMON_DATATYPE_H

#include <lvgl.h>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <iostream>

#define STR(a) #a

#define APP_NAME L"DVR-TT01" // Official application name
#define RECORD_SCREENNAME L"Record window" // Title for the recording window
#define RECORD_PROCESSNAME L"Record process" // Identifier for the recording process
#define REVIEW_SCREENNAME L"Review window" // Title for the review window
#define PLAY_SCREENNAME L"Play window" // Title for the playback window

#define HORIZON_MAX_RESOLUTION 1280 // Maximum horizontal resolution in pixels
#define VERTICAL_MAX_RESOLUTION 800 // Maximum vertical resolution in pixels

#define debug_println(a) std::cout << std::string(a) << "\n" // Helper for console logging

#define TIMECYCLE_10MS 10 // 10 millisecond interval
#define TIMECYCLE_100MS 100 // 100 millisecond interval
#define TIMECYCLE_200MS 200 // 200 millisecond interval
#define TIMECYCLE_500MS 500 // 500 millisecond interval
#define TIMECYCLE_700MS 700 // 700 millisecond interval
#define TIMECYCLE_1SEC 1000 // 1 second interval
#define TIMECYCLE_3SEC 3000 // 3 second interval
#define TIMECYCLE_5SEC 5000 // 5 second interval

#define TID_KEYDOWN 1 // Timer ID for key down events
#define TID_VOLUME_WND 2 // Timer ID for volume window auto-hide

#define VK_CHAR 0x100 // Custom flag for character-based input
#define VK_KEY0 '0' // Virtual key for '0'
#define VK_KEY9 '9' // Virtual key for '9'
#define VK_KEYA 'A' // Virtual key for 'A'
#define VK_KEYZ 'Z' // Virtual key for 'Z'

#define ACCEPT_ALL_CHARS "" // Filter string for accepting any input
#define ACCEPT_SPACE_ONLY " " // Filter string for accepting only spaces

#define MAX_CHARS MAX_PATH * 2 // Generic buffer size for character arrays

#define RL_ITEM_PER_PAGE 5 // Number of items per page in lists
#define SCROLL_Y_STEP 200 // Pixel increment for vertical scrolling
#define VOLUME_STEP 2 // Increment for system volume adjustment

#define STANDARD_PERCENT 100 // Scale factor for percentages

#define WIFI_CONNECT_CHECK_RETRY_MAX 10 // Max attempts to verify WiFi connection

#ifdef _DEBUG
#define SCRIPT_DIRECTORY L"debug_script" // Directory containing scripts for debug builds
#else
#define SCRIPT_DIRECTORY L"release_script" // Directory containing scripts for release builds
#endif

#define CREATE_OBJECT(name) if (!name) name = lv_button_create(nullptr); // Helper macro to instantiate LVGL button if null

enum
{
    WM_USER_MIN = WM_USER,
    WM_KB_KEYDOWN, // Custom message for virtual keyboard key presses
    WM_DEVICECHANGE_DONE, // Custom message for completion of device change handling
    WM_HOOK_NUMPAD_START, // Command to activate numpad hook
    WM_HOOK_NUMPAD_STOP, // Command to deactivate numpad hook
};

#define TO_ENUM(enum_name, item) item,
#define TO_STRING(enum_name, item) { enum_name::item, #item },
#define MAP_ENUM_STR(enum_name, enum_def) std::unordered_map<enum_name, std::string> mapStr_##enum_name = { enum_def(enum_name,TO_STRING) };
#define EXTERN_MAP_ENUM_STR(enum_name) extern std::unordered_map<enum_name, std::string> mapStr_##enum_name;

#define DEF_SCREEN_NAME(e, CREATE) \
        CREATE(e, SCREEN_MAIN) \
        CREATE(e, SCREEN_VIDEO_INPUT) \
        CREATE(e, SCREEN_KEYBOARD) \
        CREATE(e, SCREEN_VIDEO_RECORDLIST) \
        CREATE(e, SCREEN_SETTING) \

#define DEF_KBSCREEN_NAME(e, CREATE) \
        CREATE(e, KBSCREEN_VIDEO_EVENT) \
        CREATE(e, KBSCREEN_VIDEO_NAME) \
        CREATE(e, KBSCREEN_VIDEO_CATEGORY) \
        CREATE(e, KBSCREEN_VIDEO_DESC) \
        CREATE(e, KBSCREEN_VIDEO_AUTHOR) \
        CREATE(e, KBSCREEN_VIDEO_SEARCH) \
        CREATE(e, KBSCREEN_SSID_PASSWORD) \
        CREATE(e, KBSCREEN_DUKTO_ADDR) \

#define DEF_STATE_TYPE(e, CREATE) \
        CREATE(e, S_STOP) \
        CREATE(e, S_RECORD) \
        CREATE(e, S_PLAY) \
        CREATE(e, S_PAUSE) \
        CREATE(e, S_TRANSFER) \

#define DEF_DROPDOWNLIST_NAME(e, CREATE) \
        CREATE(e, DD_VIDEO_SEARCH) \
        CREATE(e, DD_DATE_FORMAT) \
        CREATE(e, DD_DATE_SEPARATOR) \
        CREATE(e, DD_KEYBOARD_TYPE) \
        CREATE(e, DD_T9_AUTO_CONFIRM_SPEED) \
        CREATE(e, DD_NETWORK_TYPE) \
        CREATE(e, DD_SSID) \

enum class SCREEN_NAME
{
    MIN_SCREEN,
    DEF_SCREEN_NAME(SCREEN_NAME, TO_ENUM)
    MAX_SCREEN,
    MIN_KBSCREEN,
    DEF_KBSCREEN_NAME(SCREEN_NAME, TO_ENUM)
    MAX_KBSCREEN,
};

enum class STATE_TYPE
{
    MIN,
    DEF_STATE_TYPE(STATE_TYPE, TO_ENUM)
    MAX,
};

enum class DROPDOWNLIST_NAME
{
    MIN,
    DEF_DROPDOWNLIST_NAME(DROPDOWNLIST_NAME, TO_ENUM)
    MAX,
};

EXTERN_MAP_ENUM_STR(SCREEN_NAME)
EXTERN_MAP_ENUM_STR(STATE_TYPE)
EXTERN_MAP_ENUM_STR(DROPDOWNLIST_NAME)

#endif // !_COMMON_DATATYPE_H
