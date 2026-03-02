#include "CommonData.h"
#include "CommonLibrary.h"
#include "ScreenMapping.h"

namespace keyboard_lib
{
    typedef struct
    {
        char lower;
        char upper;
    } TextType_t;

    std::vector<std::pair<int, char>> listNumber = {
        { VK_NUMPAD0, '0' },
        { VK_NUMPAD1, '1' },
        { VK_NUMPAD2, '2' },
        { VK_NUMPAD3, '3' },
        { VK_NUMPAD4, '4' },
        { VK_NUMPAD5, '5' },
        { VK_NUMPAD6, '6' },
        { VK_NUMPAD7, '7' },
        { VK_NUMPAD8, '8' },
        { VK_NUMPAD9, '9' },
    };

    std::vector<std::pair<std::string, TextType_t>> listCombineChar = {
        { "0"     , { ' ', ' ' } },
        { "00"    , { '(', '(' } },
        { "000"   , { ')', ')' } },
        { "1"     , { '.', '.' } },
        { "11"    , { '_', '_' } },
        { "111"   , { '-', '-' } },
        { "1111"  , { '#', '#' } },
        { "11111" , { '[', '[' } },
        { "111111", { ']', ']' } },
        { "2"     , { 'a', 'A' } },
        { "22"    , { 'b', 'B' } },
        { "222"   , { 'c', 'C' } },
        { "3"     , { 'd', 'D' } },
        { "33"    , { 'e', 'E' } },
        { "333"   , { 'f', 'F' } },
        { "4"     , { 'g', 'G' } },
        { "44"    , { 'h', 'H' } },
        { "444"   , { 'i', 'I' } },
        { "5"     , { 'j', 'J' } },
        { "55"    , { 'k', 'K' } },
        { "555"   , { 'l', 'L' } },
        { "6"     , { 'm', 'M' } },
        { "66"    , { 'n', 'N' } },
        { "666"   , { 'o', 'O' } },
        { "7"     , { 'p', 'P' } },
        { "77"    , { 'q', 'Q' } },
        { "777"   , { 'r', 'R' } },
        { "7777"  , { 's', 'S' } },
        { "8"     , { 't', 'T' } },
        { "88"    , { 'u', 'U' } },
        { "888"   , { 'v', 'V' } },
        { "9"     , { 'w', 'W' } },
        { "99"    , { 'x', 'X' } },
        { "999"   , { 'y', 'Y' } },
        { "9999"  , { 'z', 'Z' } },
    };

    std::vector<std::pair<std::pair<int, int>, char>> listSpecialChar = {
        { { 0       , VK_OEM_PERIOD }, '.' },
        { { VK_SHIFT, VK_OEM_MINUS  }, '_' },
        { { 0       , VK_OEM_MINUS  }, '-' },
        { { VK_SHIFT, '3'           }, '#' },
        { { 0       , VK_OEM_4      }, '[' },
        { { 0       , VK_OEM_6      }, ']' },
        { { VK_SHIFT, '9'           }, '(' },
        { { VK_SHIFT, '0'           }, ')' },
    };

    static std::string buff = "";
    static char inputChar = 0;
    static char combineChar = 0;
    static std::vector<std::pair<lv_obj_t*, int>> listVkCode = {};
    static bool isLongPress = false;
    static unsigned long long keydownTime = 0;
    static unsigned long long keydownTimeRepeat = 0;

    void SendKeyMessage(int keycode, lv_event_code_t event)
    {
        ::SendMessage(system_data::WindowHandle.GetValue(), WM_KB_KEYDOWN, keycode, event);
    }

    void CalculateInputChar(int keycode, lv_event_code_t event)
    {
        if (event == LV_EVENT_SHORT_CLICKED)
        {
            // Confirm key
            if (keycode == VK_CONVERT)
            {
                // Match number buffer to character
                for (const auto& item : listCombineChar)
                {
                    if (item.first == buff)
                    {
                        if (!GetKeyboardCapsState())
                        {
                            combineChar = item.second.lower;
                        }
                        else
                        {
                            combineChar = item.second.upper;
                        }

                        break;
                    }
                }

                buff.clear();
            }
            // Other key
            else
            {
                // Direct char input
                if (keycode == VK_CHAR)
                {
                    auto key = (int*)ScreenMapping::GetInstance().GetEvent().param; // Get char keycode from event param
                    inputChar = *key; // Default upper case or number

                    if (common_lib::CheckInRangeNumber(inputChar, VK_KEYA - 1, VK_KEYZ + 1)) // Char key
                    {
                        if (!GetKeyboardCapsState())
                        {
                            inputChar += 0x20; // Convert to lower case
                        }
                    }

                    delete key; // Delete heap
                }
                else
                {
                    // Get input key
                    for (const auto& item : listNumber)
                    {
                        if (item.first == keycode)
                        {
                            // Push input char num to buffer
                            inputChar = item.second;
                            buff.push_back(item.second);
                            break;
                        }
                    }
                }
            }
        }
        else if (event == LV_EVENT_LONG_PRESSED)
        {
            // Backspace key
            if (keycode == VK_BACK)
            {
                buff.clear();
            }
            // Other key
            else
            {
                // Set number output
                for (const auto& item : listNumber)
                {
                    if (item.first == keycode)
                    {
                        combineChar = item.second;
                        break;
                    }
                }
            }
        }
    }

    char GetInputChar()
    {
        auto temp = inputChar;
        inputChar = 0;
        return temp;
    }

    char GetCombineChar()
    {
        auto temp = combineChar;
        combineChar = 0;
        return temp;
    }

    void SetKeyboardKeyState(int vk, bool state)
    {
        BYTE keyState[256] = { 0 };

        (void)::GetKeyboardState(keyState);

        if (state)
        {
            keyState[vk] |= 0x01; // On
        }
        else
        {
            keyState[vk] &= ~0x01; // Off
        }

        ::SetKeyboardState(keyState);

        ::keybd_event(vk, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
        ::keybd_event(vk, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);

        ::Sleep(TIMECYCLE_10MS);
    }

    bool GetKeyboardCapsState()
    {
        auto isShiftPressed = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
        auto isCapsLockOn = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
        auto state = isCapsLockOn ^ isShiftPressed;

        return state;
    }

    void SetListVkCode(const std::vector<std::pair<lv_obj_t*, int>>& list)
    {
        listVkCode = list;
    }

    const std::vector<std::pair<lv_obj_t*, int>>& GetListVkCode()
    {
        return listVkCode;
    }

    const std::vector<std::pair<std::pair<int, int>, char>>& GetListSpacialChar()
    {
        return listSpecialChar;
    }

    void CALLBACK AutoConfirmKey(HWND hwnd, UINT uMsg, UINT_PTR timerId, DWORD dwTime)
    {
        KillTimer(hwnd, TID_KEYDOWN);

        lv_event_t e = { 0 };

        // Match VK code with real button on screen keypad
        for (const auto& item : keyboard_lib::GetListVkCode())
        {
            if (item.second == VK_CONVERT)
            {
                e.current_target = item.first;
                break;
            }
        }

        e.code = LV_EVENT_SHORT_CLICKED;
        ScreenMapping::GetInstance().SetEvent(e);
    }

    void HardwareKeyboardProcess(HWND hwnd, int uMsg, int wParam, lv_event_code_t lParam)
    {
        if (system_data::CurrentKbScreen.GetValue() != SCREEN_NAME::MIN_KBSCREEN)
        {
            lv_event_t e = { 0 };

            switch (uMsg)
            {
            case WM_KB_KEYDOWN:
                keyboard_lib::CalculateInputChar(wParam, lParam);
                break;
            case WM_KEYDOWN:
            {
                bool isRepeat = (lParam & (1 << 30)) != 0;

                // Match VK code with real button on screen keypad
                for (const auto& item : keyboard_lib::GetListVkCode())
                {
                    if (item.second == wParam)
                    {
                        e.current_target = item.first;
                        break;
                    }
                }

                if (!isRepeat)
                {
                    // Get current time when key is the first pressing
                    keydownTime = ::GetTickCount64();
                    keydownTimeRepeat = ::GetTickCount64();
                }
                else
                {
                    // Repeat timeout -> Repeat send event long press
                    if ((keydownTimeRepeat != 0) && (::GetTickCount64() - keydownTimeRepeat >= TIMECYCLE_100MS))
                    {
                        // Match VK code with real button on screen keypad
                        for (const auto& item : keyboard_lib::GetListVkCode())
                        {
                            if (item.second == wParam)
                            {
                                e.current_target = item.first;
                                break;
                            }
                        }

                        e.code = LV_EVENT_LONG_PRESSED_REPEAT;
                        ScreenMapping::GetInstance().SetEvent(e);

                        keydownTimeRepeat = ::GetTickCount64();
                    }

                    // Send event long press
                    if (!isLongPress)
                    {
                        isLongPress = true;
                        e.code = LV_EVENT_LONG_PRESSED;
                        ScreenMapping::GetInstance().SetEvent(e);
                    }
                }
            }
            break;
            case WM_KEYUP:
            {
                keydownTimeRepeat = 0;
                auto isShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

                // Key down timeout
                if (::GetTickCount64() - keydownTime <= TIMECYCLE_500MS)
                {
                    if ((common_lib::CheckInRangeNumber(wParam, VK_KEY0 - 1, VK_KEY9 + 1) && !isShiftPressed) // Number key
                        || common_lib::CheckInRangeNumber(wParam, VK_KEYA - 1, VK_KEYZ + 1) // Char key
                        )
                    {
                        int* keyCode = new int(wParam);
                        wParam = VK_CHAR;

                        e.param = (void*)keyCode; // Set char keycode to event param
                    }
                    else if (wParam == VK_CAPITAL)
                    {
                        bool* capsState = new bool(GetKeyboardCapsState());
                        e.param = (void*)capsState; // Set capslock state to event param
                    }
                    // Special key
                    else
                    {
                        // Match VK code to char
                        for (const auto& item : keyboard_lib::GetListSpacialChar())
                        {
                            if (item.first.second == wParam)
                            {
                                if ((isShiftPressed && (item.first.first == VK_SHIFT))
                                    || (!isShiftPressed && (item.first.first == 0)))
                                {
                                    int* keycode = new int(item.second);
                                    wParam = VK_CHAR;
                                    e.param = (void*)keycode; // Set char keycode to event param
                                    break;
                                }
                            }
                        }
                    }

                    // Match VK code with real button on screen keypad
                    for (const auto& item : keyboard_lib::GetListVkCode())
                    {
                        if (item.second == wParam)
                        {
                            e.current_target = item.first;
                            break;
                        }
                    }

                    e.code = LV_EVENT_SHORT_CLICKED;
                    ScreenMapping::GetInstance().SetEvent(e);

                    ::SetTimer(hwnd, TID_KEYDOWN, system_data::T9ConfirmTimeout.GetValue(), AutoConfirmKey);
                }

                isLongPress = false;
            }
            break;

            default:
                break;
            }
        }
    }

    void ResetAll()
    {
        inputChar = 0;
        combineChar = 0;
        buff.clear();
    }
}
