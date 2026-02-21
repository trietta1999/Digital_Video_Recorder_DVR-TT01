#ifndef _KEYBOARD_SCREEN_H
#define _KEYBOARD_SCREEN_H

#include "BaseScreen.h"

class KeyboardScreen : public BaseScreen
{
public:
    KeyboardScreen(SCREEN_NAME screen);
    ~KeyboardScreen();

private:
    static std::unordered_map<SCREEN_NAME, std::pair<std::string, std::function<std::string(void)>>> mapKbInit;
    static std::vector<std::pair<lv_obj_t*, int>> listVkCode;

    static void OnClickCancel(lv_obj_t* obj);
    static void OnClickOK(lv_obj_t* obj);
    static void OnClickKey(lv_obj_t* obj);
    static void OnLongPressKey(lv_obj_t* obj);
    static void OnLongPressRepeatKey(lv_obj_t* obj);
    static void OnReleaseKey(lv_obj_t* obj);

    static void SetKbData(SCREEN_NAME screen, void* data);
};

#endif // _KEYBOARD_SCREEN_H
