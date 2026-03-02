#ifndef _KEYBOARD_SCREEN_H
#define _KEYBOARD_SCREEN_H

#include "BaseScreen.h"

typedef struct
{
    std::string title;
    std::string acceptedSpecialChars;
    std::function<std::string(void)> cdataGetValueCallback;
} KbInfo;

class KeyboardScreen : public BaseScreen
{
public:
    KeyboardScreen(SCREEN_NAME screen);
    ~KeyboardScreen();

private:
    static std::unordered_map<SCREEN_NAME, KbInfo> mapKbInit;
    static std::vector<std::pair<lv_obj_t*, int>> listVkCode;

    static void OnClickCancel(lv_event_t* event);
    static void OnClickOK(lv_event_t* event);
    static void OnClickKey(lv_event_t* event);
    static void OnLongPressKey(lv_event_t* event);
    static void OnLongPressRepeatKey(lv_event_t* event);
    static void OnReleaseKey(lv_event_t* event);

    static void SetKbData(SCREEN_NAME screen, void* data);
};

#endif // _KEYBOARD_SCREEN_H
