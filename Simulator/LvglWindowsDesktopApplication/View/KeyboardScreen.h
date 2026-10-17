#ifndef _KEYBOARD_SCREEN_H
#define _KEYBOARD_SCREEN_H

#include "BaseScreen.h"

class KeyboardScreen : public BaseScreen
{
public:
    KeyboardScreen(SCREEN_NAME screen); // Constructor to initialize the keyboard screen context
    ~KeyboardScreen(); // Destructor for KeyboardScreen

private:
    static void OnClickCancel(lv_event_t* event); // Event handler for Cancel/Back action
    static void OnClickOK(lv_event_t* event); // Event handler for OK/Submit action
    static void OnClickKey(lv_event_t* event); // Event handler for standard key click events
    static void OnLongPressKey(lv_event_t* event); // Event handler for long press key events
    static void OnLongPressRepeatKey(lv_event_t* event); // Event handler for repeated events during long press
    static void OnShortcutKey(lv_event_t* event); // Event handler for keyboard shortcut combinations
};

#endif // _KEYBOARD_SCREEN_H
