#include "CommonData.h"
#include "CommonLibrary.h"
#include "ScreenMapping.h"

namespace keyboard_lib
{
    struct TextType_t
    {
        char lower; // Character in lowercase
        char upper; // Character in uppercase
    };

    struct NumpadKeyExchange_t
    {
        int keyOrg; // Original virtual key code
        int keyExchange; // Virtual key code to exchange with
        SCREEN_NAME screenName; // Target screen context for this exchange
    };

    // Mapping of virtual numpad keys to characters
    static std::vector<std::pair<int, char>> listNumber = {
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

    // Mapping of T9 numeric combinations to characters
    static std::vector<std::pair<std::string, TextType_t>> listCombineChar = {
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

    // Mapping of special key combinations to characters
    static std::vector<std::pair<std::pair<int, int>, char>> listSpecialChar = {
        { { 0       , VK_OEM_PERIOD }, '.' },
        { { VK_SHIFT, VK_OEM_MINUS  }, '_' },
        { { 0       , VK_OEM_MINUS  }, '-' },
        { { VK_SHIFT, '3'           }, '#' },
        { { 0       , VK_OEM_4      }, '[' },
        { { 0       , VK_OEM_6      }, ']' },
        { { VK_SHIFT, '9'           }, '(' },
        { { VK_SHIFT, '0'           }, ')' },
    };

    // Table for remapping physical numpad keys to application functions
    static std::vector<NumpadKeyExchange_t> listNumpadKeyExchange = {
        { VK_NUMPAD1, VK_NUMPAD9, SCREEN_NAME::MAX_KBSCREEN },
        { VK_NUMPAD2, VK_NUMPAD6, SCREEN_NAME::MAX_KBSCREEN },
        { VK_NUMPAD3, VK_NUMPAD3, SCREEN_NAME::MAX_KBSCREEN },
        { VK_NUMPAD4, VK_NUMPAD8, SCREEN_NAME::MAX_KBSCREEN },
        { VK_NUMPAD5, VK_NUMPAD5, SCREEN_NAME::MAX_KBSCREEN },
        { VK_NUMPAD6, VK_NUMPAD2, SCREEN_NAME::MAX_KBSCREEN },
        { VK_NUMPAD7, VK_NUMPAD7, SCREEN_NAME::MAX_KBSCREEN },
        { VK_NUMPAD8, VK_NUMPAD4, SCREEN_NAME::MAX_KBSCREEN },
        { VK_NUMPAD9, VK_NUMPAD1, SCREEN_NAME::MAX_KBSCREEN },

        { VK_NUMPAD0, VK_NUMPAD0, SCREEN_NAME::MAX_KBSCREEN },
        { VK_SPACE  , VK_SPACE  , SCREEN_NAME::MAX_KBSCREEN },
        { VK_DECIMAL, VK_CAPITAL, SCREEN_NAME::MAX_KBSCREEN }, // .

        { VK_DIVIDE  , VK_CANCEL, SCREEN_NAME::MAX_SCREEN   }, // /
        { VK_MULTIPLY, VK_ACCEPT, SCREEN_NAME::MAX_SCREEN   }, // *
        { VK_BACK    , VK_BACK  , SCREEN_NAME::MAX_KBSCREEN },

        { VK_ADD     , VK_VOLUME_UP  , SCREEN_NAME::MAX_SCREEN }, // +
        { VK_SUBTRACT, VK_VOLUME_DOWN, SCREEN_NAME::MAX_SCREEN }, // -

        { VK_BROWSER_BACK, VK_CANCEL, SCREEN_NAME::MAX_SCREEN },
        { VK_BROWSER_HOME, VK_F1    , SCREEN_NAME::MAX_SCREEN },
        { VK_TAB         , VK_F2    , SCREEN_NAME::MAX_SCREEN },
        { VK_LAUNCH_MAIL , VK_F3    , SCREEN_NAME::MAX_SCREEN },
        { VK_LAUNCH_APP2 , VK_F4    , SCREEN_NAME::MAX_SCREEN },
    };

    static std::vector<std::pair<lv_obj_t*, int>> listVkCode = {}; // List mapping UI buttons to virtual key codes

    static std::string buff = ""; // Buffer for T9 numeric sequence
    static char inputChar = 0; // Buffer for direct character input
    static char combineChar = 0; // Buffer for T9 combined character output
    static bool isLongPress = false; // Flag to track long press state
    static unsigned long long keydownTime = 0; // Timestamp for initial key down
    static unsigned long long keydownTimeRepeat = 0; // Timestamp for repeat interval tracking
    static HHOOK keyboardHook = NULL; // Handle to the low-level keyboard hook


        void SendKeyMessage(int keycode, lv_event_code_t event)
    {
        // Forwards the keyboard event to the main window message loop
        ::SendMessage(system_data::WindowHandle.GetValue(), WM_KB_KEYDOWN, keycode, event);
    }

    void CalculateInputChar(int keycode, lv_event_code_t event)
    {
        if (event == LV_EVENT_SHORT_CLICKED)
        {
            // Confirm key (T9 logic)
            if (keycode == VK_CONVERT)
            {
                // Match the accumulated numeric buffer to a character in the T9 map
                for (const auto& item : listCombineChar)
                {
                    if (item.first == buff)
                    {
                        // Assign uppercase or lowercase based on current keyboard state
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

                buff.clear(); // Reset buffer after confirmation
            }
            // Handling standard keys
            else
            {
                // Logic for direct character input (from a full keyboard layout)
                if (keycode == VK_CHAR)
                {
                    auto key = (int*)ScreenMapping::GetInstance().GetEvent().param; // Retrieve character code from event parameter
                    inputChar = *key; // Set input character (defaults to upper/number)

                    // Auto-convert to lowercase if Caps Lock logic allows
                    if (common_lib::CheckInRangeNumberEqual(inputChar, VK_KEYA, VK_KEYZ))
                    {
                        if (!GetKeyboardCapsState())
                        {
                            inputChar += 0x20; // ASCII offset for lowercase
                        }
                    }

                    delete key; // Free heap memory allocated for the parameter
                }
                else
                {
                    // Logic for numeric pad (T9 input collection)
                    for (const auto& item : listNumber)
                    {
                        if (item.first == keycode)
                        {
                            inputChar = item.second; // Store latest numeric char
                            buff.push_back(item.second); // Append to T9 sequence buffer
                            break;
                        }
                    }
                }
            }
        }
        else if (event == LV_EVENT_LONG_PRESSED)
        {
            // Backspace long press clears the entire T9 buffer
            if (keycode == VK_BACK)
            {
                buff.clear();
            }
            // Other keys long press might directly output the number instead of T9 sequence
            else
            {
                for (const auto& item : listNumber)
                {
                    if (item.first == keycode)
                    {
                        combineChar = item.second; // Direct numeric output
                        break;
                    }
                }
            }
        }
    }

    char GetInputChar()
    {
        // Consumes and returns the latest direct input character
        auto temp = inputChar;
        inputChar = 0;
        return temp;
    }

    char GetCombineChar()
    {
        // Consumes and returns the latest T9 combined character
        auto temp = combineChar;
        combineChar = 0;
        return temp;
    }

    void SetKeyboardKeyState(int vk, bool state)
    {
        BYTE keyState[256] = { 0 };

        (void)::GetKeyboardState(keyState); // Fetch current system-wide keyboard state

        // Modify the specific virtual key's toggled state
        if (state)
        {
            keyState[vk] |= 0x01; // Set bit 0 (toggled on)
        }
        else
        {
            keyState[vk] &= ~0x01; // Clear bit 0 (toggled off)
        }

        ::SetKeyboardState(keyState); // Apply the modified state back to the system

        // Simulate a physical key press/release to trigger system updates (like UI indicators)
        ::keybd_event(vk, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
        ::keybd_event(vk, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);

        ::Sleep(TIMECYCLE_10MS); // Short delay to allow system processing
    }

    bool GetKeyboardCapsState()
    {
        // Logic to determine if characters should be uppercase
        // Based on Caps Lock state XORed with Shift key state
        auto isShiftPressed = (::GetKeyState(VK_SHIFT) & 0x8000) != 0;
        auto isCapsLockOn = (::GetKeyState(VK_CAPITAL) & 0x0001) != 0;
        auto state = isCapsLockOn ^ isShiftPressed;

        return state;
    }

    short GetAutoConfirmTimeMs()
    {
        // Returns the timeout value based on the user's T9 speed setting
        switch (setting_data::T9AutoConfirmSpeed.GetValue())
        {
        case (short)dropdownlist_lib::DD_T9_AUTO_CONFIRM_SPEED_e::Slow:
            return TIMECYCLE_700MS + TIMECYCLE_200MS;
        case (short)dropdownlist_lib::DD_T9_AUTO_CONFIRM_SPEED_e::Normal:
            return TIMECYCLE_500MS + TIMECYCLE_100MS;
        case (short)dropdownlist_lib::DD_T9_AUTO_CONFIRM_SPEED_e::Fast:
            return TIMECYCLE_200MS;
        default:
            return 0;
        }
    }

    const std::vector<std::pair<lv_obj_t*, int>>& GetListVkCode()
    {
        return listVkCode; // Returns the mapping of UI objects to key codes
    }

    void SetListVkCode(const std::vector<std::pair<lv_obj_t*, int>>& list)
    {
        listVkCode = list; // Updates the mapping of UI objects to key codes
    }

    void CALLBACK AutoConfirmKey(HWND hwnd, UINT uMsg, UINT_PTR timerId, DWORD dwTime)
    {
        ::KillTimer(hwnd, TID_KEYDOWN); // Stop the confirmation timer

        // If on a keyboard screen and not using a standard keyboard, trigger auto-confirm
        if ((system_data::CurrentKbScreen.GetValue() != SCREEN_NAME::MIN_KBSCREEN)
            && (setting_data::KeyboardType.GetValue() != (short)dropdownlist_lib::DD_KEYBOARD_TYPE_e::Standard_keyboard))
        {
            lv_event_t e = { 0 };

            // Find the UI object corresponding to the 'Convert/Confirm' key
            for (const auto& item : listVkCode)
            {
                if (item.second == VK_CONVERT)
                {
                    e.current_target = item.first;
                    break;
                }
            }

            e.code = LV_EVENT_SHORT_CLICKED; // Trigger a click event
            ScreenMapping::GetInstance().SetEvent(e); // Dispatch event to LVGL
        }
    }

    void HardwareKeyboardProcess(HWND hwnd, int uMsg, int wParam, lv_event_code_t lParam)
    {
        lv_event_t e = { 0 };

        switch (uMsg)
        {
        case WM_KB_KEYDOWN:
            // Custom message handler for virtual key processing
            keyboard_lib::CalculateInputChar(wParam, lParam);
            break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            bool isRepeat = (lParam & (1 << 30)) != 0; // Check if this is an auto-repeat message

            // Map physical virtual key to corresponding UI button
            for (const auto& item : listVkCode)
            {
                if (item.second == wParam)
                {
                    e.current_target = item.first;
                    break;
                }
            }

            if (!isRepeat)
            {
                // Record time of first press
                keydownTime = ::GetTickCount64();
                keydownTimeRepeat = ::GetTickCount64();
            }
            else
            {
                // Handle continuous repeat for long presses
                if ((keydownTimeRepeat != 0) && (::GetTickCount64() - keydownTimeRepeat >= TIMECYCLE_100MS))
                {
                    for (const auto& item : listVkCode)
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

                // Initial long press detection (after first repeat message)
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
        case WM_SYSKEYUP:
        {
            keydownTimeRepeat = 0;
            auto isShiftPressed = (::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

            // Handle short click if key was released within the timeout and wasn't a long press
            if (::GetTickCount64() - keydownTime <= TIMECYCLE_500MS)
            {
                if ((common_lib::CheckInRangeNumberEqual(wParam, VK_KEY0, VK_KEY9) && !isShiftPressed) // Number key
                    || common_lib::CheckInRangeNumberEqual(wParam, VK_KEYA, VK_KEYZ) // Char key
                    )
                {
                    int* keyCode = new int(wParam);
                    wParam = VK_CHAR; // Re-route as a character event

                    e.param = (void*)keyCode; // Set char keycode to event param
                }
                // Check for special characters using Shift state
                else
                {
                    // Match VK code to char
                    for (const auto& item : listSpecialChar)
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

                // Find target UI object for the key
                for (const auto& item : listVkCode)
                {
                    if (item.second == wParam)
                    {
                        e.current_target = item.first;
                        break;
                    }
                }

                e.code = LV_EVENT_SHORT_CLICKED;
                ScreenMapping::GetInstance().SetEvent(e);

                // Start timer for T9 auto-confirmation
                ::SetTimer(hwnd, TID_KEYDOWN, GetAutoConfirmTimeMs(), AutoConfirmKey);
            }

            isLongPress = false; // Reset long press state on key up
        }
        break;
        case WM_RBUTTONUP:
        {
            // Special handling for right-click mapped to a UI button
            for (const auto& item : listVkCode)
            {
                if (item.second == VK_RBUTTON)
                {
                    e.current_target = item.first;
                    e.code = LV_EVENT_SHORT_CLICKED;
                    ScreenMapping::GetInstance().SetEvent(e);

                    break;
                }
            }
        }
        break;
        default:
            break;
        }
    }

    LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
    {
        if (nCode == HC_ACTION)
        {
            KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
            int targetKey = 0;

            // Ignore synthetic keystrokes injected by this application
            if (p->flags & LLKHF_INJECTED)
            {
                return ::CallNextHookEx(keyboardHook, nCode, wParam, lParam);
            }

            // Remap physical keys based on current application context
            for (const auto& keyInfo : listNumpadKeyExchange)
            {
                if (p->vkCode == keyInfo.keyOrg)
                {
                    // Remap if in Internal Numpad mode or if key is a global system action
                    if (((keyInfo.screenName == SCREEN_NAME::MAX_KBSCREEN)
                        && (setting_data::KeyboardType.GetValue() == (short)dropdownlist_lib::DD_KEYBOARD_TYPE_e::Internal_numpad))
                        || (keyInfo.screenName == SCREEN_NAME::MAX_SCREEN)
                        )
                    {
                        targetKey = keyInfo.keyExchange;
                    }

                    break;
                }
            }

            // If a remapping is found, suppress original key and inject the new one
            if (targetKey)
            {
                if ((wParam == WM_KEYDOWN) || (wParam == WM_SYSKEYDOWN))
                {
                    INPUT inputDown = {};
                    inputDown.type = INPUT_KEYBOARD;
                    inputDown.ki.wVk = targetKey;

                    ::SendInput(1, &inputDown, sizeof(INPUT));
                }
                else if ((wParam == WM_KEYUP) || (wParam == WM_SYSKEYUP))
                {
                    INPUT inputUp = {};
                    inputUp.type = INPUT_KEYBOARD;
                    inputUp.ki.wVk = targetKey;
                    inputUp.ki.dwFlags = KEYEVENTF_KEYUP;

                    ::SendInput(1, &inputUp, sizeof(INPUT));
                }

                return 1; // Return non-zero to block original input
            }
        }

        return ::CallNextHookEx(keyboardHook, nCode, wParam, lParam);
    }

    void SetupHookInternalNumpad()
    {
        // Installs a low-level hook to monitor keyboard input system-wide
        keyboardHook = ::SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, ::GetModuleHandle(NULL), 0);

        if (!keyboardHook)
        {
            return;
        }
    }

    void StopHookInternalNumpad()
    {
        // Removes the previously installed keyboard hook
        ::UnhookWindowsHookEx(keyboardHook);
    }

    void ResetAll()
    {
        // Clears all character buffers and T9 state
        inputChar = 0;
        combineChar = 0;
        buff.clear();
    }
}
