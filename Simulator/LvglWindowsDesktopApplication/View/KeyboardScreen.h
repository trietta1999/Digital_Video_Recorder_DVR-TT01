#ifndef _KEYBOARD_SCREEN_H
#define _KEYBOARD_SCREEN_H

#include "BaseScreen.h"

class KeyboardScreen : public BaseScreen
{
public:
    KeyboardScreen(SCREEN_NAME screen);
    ~KeyboardScreen();

private:
    static void OnClickCancel(lv_event_t* event);
    static void OnClickOK(lv_event_t* event);
    static void OnClickKey(lv_event_t* event);
    static void OnLongPressKey(lv_event_t* event);
    static void OnLongPressRepeatKey(lv_event_t* event);
    static void OnShortcutKey(lv_event_t* event);
};

#endif // _KEYBOARD_SCREEN_H
