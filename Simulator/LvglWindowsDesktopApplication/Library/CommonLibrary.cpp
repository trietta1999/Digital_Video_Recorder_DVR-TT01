#include "CommonLibrary.h"
#include "CommonData.h"
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
        { "1"   , { ' ', ' ' } },
        { "2"   , { 'a', 'A' } },
        { "22"  , { 'b', 'B' } },
        { "222" , { 'c', 'C' } },
        { "3"   , { 'd', 'D' } },
        { "33"  , { 'e', 'E' } },
        { "333" , { 'f', 'F' } },
        { "4"   , { 'g', 'G' } },
        { "44"  , { 'h', 'H' } },
        { "444" , { 'i', 'I' } },
        { "5"   , { 'j', 'J' } },
        { "55"  , { 'k', 'K' } },
        { "555" , { 'l', 'L' } },
        { "6"   , { 'm', 'M' } },
        { "66"  , { 'n', 'N' } },
        { "666" , { 'o', 'O' } },
        { "7"   , { 'p', 'P' } },
        { "77"  , { 'q', 'Q' } },
        { "777" , { 'r', 'R' } },
        { "7777", { 's', 'S' } },
        { "8"   , { 't', 'T' } },
        { "88"  , { 'u', 'U' } },
        { "888" , { 'v', 'V' } },
        { "9"   , { 'w', 'W' } },
        { "99"  , { 'x', 'X' } },
        { "999" , { 'y', 'Y' } },
        { "9999", { 'z', 'Z' } },
    };

    static std::string buff = "";
    static char inputChar = 0;
    static char combineChar = 0;
    static bool isShift = false;
    static std::vector<std::pair<lv_obj_t*, int>> listVkCode = {};
    static bool isLongPress = false;
    static unsigned long long keydownTime = 0;

    void SendKeyMessage(int keycode, lv_event_code_t event)
    {
        ::SendMessage(system_data::WindowHandle.GetValue(), WM_KB_KEYDOWN, keycode, event);
    }

    void CalculateInputChar(int keycode, lv_event_code_t event)
    {
        if (event == LV_EVENT_SHORT_CLICKED)
        {
            // Confirm key
            if (keycode == VK_NUMPAD0)
            {
                // Match number buffer to character
                for (const auto& item : listCombineChar)
                {
                    if (item.first == buff)
                    {
                        if (!isShift)
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
                // Get input key
                for (const auto& item : listNumber)
                {
                    if (item.first == keycode)
                    {
                        inputChar = item.second;
                        buff.push_back(item.second);
                        break;
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

    void SetShiftState(bool state)
    {
        isShift = state;
    }

    void SetListVkCode(const std::vector<std::pair<lv_obj_t*, int>>& list)
    {
        listVkCode = list;
    }

    const std::vector<std::pair<lv_obj_t*, int>>& GetListVkCode()
    {
        return listVkCode;
    }

    void HardwareKeyboardProcess(int uMsg, int wParam, lv_event_code_t lParam)
    {
        if (system_data::CurrentKbScreen.GetValue() != SCREEN_NAME::MIN_KBSCREEN)
        {
            if ((system_data::KeyboardType.GetValue() == KEYBOARD_TYPE::INTERNAL_NUMPAD)
                || (system_data::KeyboardType.GetValue() == KEYBOARD_TYPE::STANDARD_NUMPAD))
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

                    if (!isRepeat) {
                        //e.code = LV_EVENT_SHORT_CLICKED;
                        //ScreenMapping::GetInstance().SetEvent(e);
                        keydownTime = ::GetTickCount64();
                    }
                    else {
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
                    if (::GetTickCount64() - keydownTime <= TIMECYCLE_500MS)
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

                        e.code = LV_EVENT_SHORT_CLICKED;
                        ScreenMapping::GetInstance().SetEvent(e);
                    }

                    isLongPress = false;
                }
                break;
                default:
                    break;
                }
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

namespace common_lib
{
    bool CheckInRangeNumber(int check, int min, int max)
    {
        return ((check > min) && (check < max));
    }
}

//std::queue<bool> queueTimerExist = { };
//
//int32_t RandomRangeNumber(int32_t a, int32_t b)
//{
//    return (a + rand() % (b + 1 - a));
//}
//
//bool CheckObjectState(lv_obj_t* obj, lv_state_t state)
//{
//    if ((lv_obj_get_state(obj) & state) == state)
//    {
//        return true;
//    }
//
//    return false;
//}
//
//std::vector<BULLET_TYPE> CreateBulletList(uint8_t maxNum)
//{
//    std::vector<BULLET_TYPE> sample(maxNum), temp(maxNum);
//
//    // Get number of Blank value
//    uint8_t blankCount = RandomRangeNumber(1, maxNum - 1); // Must have at least 1 Live value
//
//    // Assign Blank value to first element range
//    for (uint8_t i = 0; i < blankCount; i++) {
//        sample[i] = BULLET_TYPE::BLANK;
//    }
//
//    // Copy to temporary
//    temp = sample;
//
//    std::mt19937 gen(rand());
//
//    // The random set must be different from the sample
//    do
//    {
//        std::shuffle(temp.begin(), temp.end(), gen);
//    } while (temp == sample);
//
//    // Update sample
//    sample = temp;
//
//    // Fill Live value
//    for (auto& item : sample)
//    {
//        if (item == BULLET_TYPE::MIN)
//        {
//            item = BULLET_TYPE::LIVE;
//        }
//    }
//
//    return sample;
//}
//
//void GetOrdinalNumber(uint8_t num, std::string& suffix)
//{
//    if (num == 1)
//    {
//        suffix = "st";
//    }
//    else if (num == 2)
//    {
//        suffix = "nd";
//    }
//    else if (num == 3)
//    {
//        suffix = "rd";
//    }
//    else
//    {
//        suffix = "th";
//    }
//}
//
//void DelayCallback(std::function<void(const void*)> func, void* data, uint32_t ms)
//{
//    struct data_t
//    {
//        std::function<void(const void*)> func;
//        void* data = nullptr;
//    };
//
//    data_t* ldata = new data_t;
//
//    if (ldata)
//    {
//        ldata->func = func;
//        ldata->data = data;
//
//        BlockGui();
//
//        lv_timer_create([](lv_timer_t* timer) {
//            auto ldata = (data_t*)lv_timer_get_user_data(timer);
//
//            ldata->func(ldata->data);
//
//            delete ldata;
//
//            UnblockGui();
//            }, ms, ldata)->repeat_count = 1;
//    }
//}
//
//void BlockGui()
//{
//    GuiBlockState.SetValue(true);
//    queueTimerExist.push(true);
//}
//
//void UnblockGui()
//{
//    queueTimerExist.pop();
//
//    if (!queueTimerExist.size())
//    {
//        GuiBlockState.SetValue(false);
//    }
//}
//
//std::string JoinString(std::string delimeter, const std::vector<std::string>& list)
//{
//    std::string output;
//
//    for (uint32_t i = 0; i < list.size(); i++)
//    {
//        if (i < list.size() - 1)
//        {
//            output += list[i] + delimeter;
//        }
//        else
//        {
//            output += list[i];
//        }
//    }
//
//    return output;
//}
