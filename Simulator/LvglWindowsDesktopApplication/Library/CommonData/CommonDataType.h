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

#define APP_NAME L"DVR-TT01"
#define RECORD_SCREENNAME L"Record window"
#define RECORD_PROCESSNAME L"Record process"
#define REVIEW_SCREENNAME L"Review window"
#define PLAY_SCREENNAME L"Play window"
#define DUKTO_SCREENNAME L"Dukto"

#define HORIZON_MAX_RESOLUTION 1280
#define VERTICAL_MAX_RESOLUTION 800

#define debug_println(a) std::cout << std::string(a) << "\n"

#define TIMECYCLE_10MS 10
#define TIMECYCLE_100MS 100
#define TIMECYCLE_200MS 200
#define TIMECYCLE_500MS 500
#define TIMECYCLE_700MS 700
#define TIMECYCLE_1SEC 1000
#define TIMECYCLE_3SEC 3000
#define TIMECYCLE_5SEC 5000

#define TID_KEYDOWN 1
#define TID_VOLUME_WND 2

#define VK_CHAR 0x100
#define VK_KEY0 '0'
#define VK_KEY9 '9'
#define VK_KEYA 'A'
#define VK_KEYZ 'Z'

#define ACCEPT_ALL_CHARS ""
#define ACCEPT_SPACE_ONLY " "

#define MAX_CHARS MAX_PATH * 2

#define RL_ITEM_PER_PAGE 5
#define SCROLL_Y_STEP 200
#define VOLUME_STEP 2

#define STANDARD_PERCENT 100

#define WIFI_CONNECT_CHECK_RETRY_MAX 10

#ifdef _DEBUG
#define SCRIPT_DIRECTORY L"debug_script"
#else
#define SCRIPT_DIRECTORY L"release_script"
#endif

//#define MAX_SIZE 1000
//#define BUFFER_SIZE sizeof(char) * MAX_SIZE
//
//#ifdef _WIN64
//#include <Windows.h>
//#define MILLISEC_GET ::GetTickCount64()
//
//#define SHARED_MEM L"SharedMemoryJson"
//#else
//#include <Arduino.h>
//#define WM_USER 0x0400
//#define HWND uint8_t
//
//#define debug_println(a) Serial.println(std::string(a).c_str())
//#define MILLISEC_GET millis()
//
//#define INPUT_PIN 22
//#define BUZZER_PIN 26
//
//#define IP_ADD_1 192
//#define IP_ADD_2 168
//#define IP_ADD_3 0
//#define IP_ADD_4 (uint8_t)MODULE_NAME::HostTimer
//
//#define NETMASK_1 255
//#define NETMASK_2 255
//#define NETMASK_3 255
//#define NETMASK_4 0
//
//#define HTTP_OK 200
//#define HTTP_NOT_FOUND 404
//
//struct data_pack_t {
//    char source[20];
//    uint32_t base_msg;
//    uint32_t msg;
//    char data[MAX_SIZE];
//};
//#endif

enum
{
    WM_USER_MIN = WM_USER,
    WM_KB_KEYDOWN,
    WM_DEVICECHANGE_DONE,
    WM_HOOK_NUMPAD_START,
    WM_HOOK_NUMPAD_STOP,
};

//#define STRIKE_NUM_MAX 3
//
//#define INCORRECT (uint8_t)(-1)
//
//#define MINUTE_POS 0
//#define SECOND_POS 1
//#define MILLIS_POS 2
//
//#define TIMECYCLE_0 1000
//#define TIMECYCLE_1 600
//#define TIMECYCLE_2 400
//
//#define BEEP_FRE 3000
//#define BEEP_INCREASE_DURATION 50
//#define BEEP_TIMEOUT 3000
//
//#define STATE_CHECKED 3
//#define STATE_UNCHECK 2

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
