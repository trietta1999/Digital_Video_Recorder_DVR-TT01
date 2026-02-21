#ifndef _COMMON_DATATYPE_H
#define _COMMON_DATATYPE_H

#include <lvgl.h>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <functional>

#define STR(a) #a
#define APP_NAME L"DVR-TT01"

#define HORIZON_MAX_RESOLUTION 1280
#define VERTICAL_MAX_RESOLUTION 768

#define debug_println(a) std::cout << std::string(a) << "\n"

#define TIMECYCLE_1SEC 1000
#define TIMECYCLE_500MS 500
#define TIMECYCLE_200MS 200
#define TIMECYCLE_100MS 100
#define TIMECYCLE_10MS 100

#define TID_KEYDOWN 1

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
        CREATE(e, MAIN_SCREEN) \
        CREATE(e, VIDEO_INPUT_SCREEN) \
        CREATE(e, KEYBOARD_SCREEN) \

#define DEF_KBSCREEN_NAME(e, CREATE) \
        CREATE(e, VIDEO_ID_KBSCREEN) \
        CREATE(e, VIDEO_NAME_KBSCREEN) \
        CREATE(e, VIDEO_DESC_KBSCREEN) \

#define DEF_KEYBOARD_TYPE(e, CREATE) \
        CREATE(e, INTERNAL_NUMPAD) \
        CREATE(e, STANDARD_NUMPAD) \
        CREATE(e, STANDARD_KEYBOARD) \

enum class SCREEN_NAME
{
    MIN_SCREEN,
    DEF_SCREEN_NAME(SCREEN_NAME, TO_ENUM)
    MAX_SCREEN,
    MIN_KBSCREEN,
    DEF_KBSCREEN_NAME(SCREEN_NAME, TO_ENUM)
    MAX_KBSCREEN,
};

enum class KEYBOARD_TYPE
{
    MIN,
    DEF_KEYBOARD_TYPE(KEYBOARD_TYPE, TO_ENUM)
    MAX,
};

EXTERN_MAP_ENUM_STR(SCREEN_NAME)
EXTERN_MAP_ENUM_STR(KEYBOARD_TYPE)

#endif // !_COMMON_DATATYPE_H
