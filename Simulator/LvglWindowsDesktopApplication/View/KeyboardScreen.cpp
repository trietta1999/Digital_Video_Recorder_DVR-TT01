#include "ui.h"
#include "CommonData.h"
#include "CommonLibrary.h"
#include "ScreenMapping.h"
#include "KeyboardScreen.h"

std::unordered_map<SCREEN_NAME, std::pair<std::string, std::function<std::string(void)>>> KeyboardScreen::mapKbInit = {};
std::vector<std::pair<lv_obj_t*, int>> KeyboardScreen::listVkCode = {};
static lv_timer_t* timerUpdateInput = nullptr;

KeyboardScreen::KeyboardScreen(SCREEN_NAME screen) : BaseScreen(screen)
{
    system_data::KeyboardType.SetValue(KEYBOARD_TYPE::INTERNAL_NUMPAD); //@todo: wait setting

    ListButtonCallback = {
        { ui_btnKeyboardCancel      , OnClickCancel , LV_EVENT_CLICKED       },
        { ui_btnKeyboardOK          , OnClickOK     , LV_EVENT_CLICKED       },
        { ui_btnKeyboardKey0        , OnClickKey    , LV_EVENT_SHORT_CLICKED },
        { ui_btnKeyboardKey1        , OnClickKey    , LV_EVENT_SHORT_CLICKED },
        { ui_btnKeyboardKey2        , OnClickKey    , LV_EVENT_SHORT_CLICKED },
        { ui_btnKeyboardKey3        , OnClickKey    , LV_EVENT_SHORT_CLICKED },
        { ui_btnKeyboardKey4        , OnClickKey    , LV_EVENT_SHORT_CLICKED },
        { ui_btnKeyboardKey5        , OnClickKey    , LV_EVENT_SHORT_CLICKED },
        { ui_btnKeyboardKey6        , OnClickKey    , LV_EVENT_SHORT_CLICKED },
        { ui_btnKeyboardKey7        , OnClickKey    , LV_EVENT_SHORT_CLICKED },
        { ui_btnKeyboardKey8        , OnClickKey    , LV_EVENT_SHORT_CLICKED },
        { ui_btnKeyboardKey9        , OnClickKey    , LV_EVENT_SHORT_CLICKED },
        { ui_btnKeyboardKeyShift    , OnClickKey    , LV_EVENT_SHORT_CLICKED },
        { ui_btnKeyboardKeyBackspace, OnClickKey    , LV_EVENT_SHORT_CLICKED },
        { ui_btnKeyboardKey0        , OnLongPressKey, LV_EVENT_LONG_PRESSED  },
        { ui_btnKeyboardKey1        , OnLongPressKey, LV_EVENT_LONG_PRESSED  },
        { ui_btnKeyboardKey2        , OnLongPressKey, LV_EVENT_LONG_PRESSED  },
        { ui_btnKeyboardKey3        , OnLongPressKey, LV_EVENT_LONG_PRESSED  },
        { ui_btnKeyboardKey4        , OnLongPressKey, LV_EVENT_LONG_PRESSED  },
        { ui_btnKeyboardKey5        , OnLongPressKey, LV_EVENT_LONG_PRESSED  },
        { ui_btnKeyboardKey6        , OnLongPressKey, LV_EVENT_LONG_PRESSED  },
        { ui_btnKeyboardKey7        , OnLongPressKey, LV_EVENT_LONG_PRESSED  },
        { ui_btnKeyboardKey8        , OnLongPressKey, LV_EVENT_LONG_PRESSED  },
        { ui_btnKeyboardKey9        , OnLongPressKey, LV_EVENT_LONG_PRESSED  },
        { ui_btnKeyboardKeyBackspace, OnLongPressKey, LV_EVENT_LONG_PRESSED  },
    };

    ListDataUpdateCallback = {};

    mapKbInit = {
        { SCREEN_NAME::VIDEO_ID_KBSCREEN  , { "Video ID"   , []() { return temp_data::VideoID.GetValue(); }   } },
        { SCREEN_NAME::VIDEO_NAME_KBSCREEN, { "Video name" , []() { return temp_data::VideoName.GetValue(); } } },
        { SCREEN_NAME::VIDEO_DESC_KBSCREEN, { "Description", []() { return temp_data::VideoDesc.GetValue(); } } },
    };

    listVkCode = {
        { ui_btnKeyboardKey0        , VK_NUMPAD0 },
        { ui_btnKeyboardKey1        , VK_NUMPAD1 },
        { ui_btnKeyboardKey2        , VK_NUMPAD2 },
        { ui_btnKeyboardKey3        , VK_NUMPAD3 },
        { ui_btnKeyboardKey4        , VK_NUMPAD4 },
        { ui_btnKeyboardKey5        , VK_NUMPAD5 },
        { ui_btnKeyboardKey6        , VK_NUMPAD6 },
        { ui_btnKeyboardKey7        , VK_NUMPAD7 },
        { ui_btnKeyboardKey8        , VK_NUMPAD8 },
        { ui_btnKeyboardKey9        , VK_NUMPAD9 },
        { ui_btnKeyboardKeyShift    , VK_SHIFT   },
        { ui_btnKeyboardKeyBackspace, VK_BACK    },
    };

    // Copy list VK code library
    keyboard_lib::SetListVkCode(listVkCode);

    // Init text input
    lv_label_set_text(ui_lblKeyboardTitle, mapKbInit[system_data::CurrentKbScreen.GetValue()].first.c_str());
    lv_textarea_set_text(ui_txtKeyboardInput, mapKbInit[system_data::CurrentKbScreen.GetValue()].second().c_str());

    // Init numpad area
    if ((system_data::KeyboardType.GetValue() == KEYBOARD_TYPE::INTERNAL_NUMPAD)
        || (system_data::KeyboardType.GetValue() == KEYBOARD_TYPE::STANDARD_NUMPAD))
    {
        lv_obj_remove_flag(ui_pnlNumpad, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(ui_pnlNumpad, LV_OBJ_FLAG_HIDDEN);
    }

    // Auto update text input
    timerUpdateInput = lv_timer_create([](lv_timer_t* timer) {
        auto input = keyboard_lib::GetInputChar();
        auto combine = keyboard_lib::GetCombineChar();

        if (input)
        {
            lv_textarea_add_char(ui_txtKeyboardTempInput, input);
        }

        if (combine)
        {
            lv_textarea_add_char(ui_txtKeyboardInput, combine);
        }
        }, TIMECYCLE_10MS, nullptr);
}

KeyboardScreen::~KeyboardScreen()
{
    lv_timer_del(timerUpdateInput);
    timerUpdateInput = nullptr;

    // Reset keyboard buffer
    keyboard_lib::ResetAll();

    // Clear all input text
    lv_textarea_set_text(ui_txtKeyboardTempInput, "");
    lv_textarea_set_text(ui_txtKeyboardInput, "");
}

void KeyboardScreen::OnClickCancel(lv_obj_t* obj)
{
    ScreenMapping::GetInstance().ChangeScreen(system_data::CurrentScreen.GetValue());
}

void KeyboardScreen::OnClickOK(lv_obj_t* obj)
{
    // Apply input data
    auto input = std::string(lv_textarea_get_text(ui_txtKeyboardInput));
    SetKbData(system_data::CurrentKbScreen.GetValue(), (void*)&input);

    ScreenMapping::GetInstance().ChangeScreen(system_data::CurrentScreen.GetValue());
}

void KeyboardScreen::OnClickKey(lv_obj_t* obj)
{
    if (obj == ui_btnKeyboardKey0)
    {
        // Clear temp input when click confirm key
        lv_textarea_set_text(ui_txtKeyboardTempInput, "");
    }
    else if (obj == ui_btnKeyboardKeyBackspace)
    {
        // Delete the latest character
        lv_textarea_delete_char(ui_txtKeyboardInput);
    }
    else if (obj == ui_btnKeyboardKeyShift)
    {
        // Toggle key
        if ((lv_obj_get_state(ui_btnKeyboardKeyShift) & LV_STATE_CHECKED) == LV_STATE_CHECKED)
        {
            lv_obj_remove_state(obj, LV_STATE_CHECKED);
        }
        else
        {
            lv_obj_add_state(obj, LV_STATE_CHECKED);
        }

        // Set state
        if ((lv_obj_get_state(ui_btnKeyboardKeyShift) & LV_STATE_CHECKED) == LV_STATE_CHECKED)
        {
            keyboard_lib::SetShiftState(true);
        }
        else
        {
            keyboard_lib::SetShiftState(false);
        }
    }

    // Send message to WINPROC
    for (const auto& item : listVkCode)
    {
        if (item.first == obj)
        {
            keyboard_lib::SendKeyMessage(item.second, LV_EVENT_SHORT_CLICKED);
            break;
        }
    }
}

void KeyboardScreen::OnLongPressKey(lv_obj_t* obj)
{
    if (obj == ui_btnKeyboardKeyBackspace)
    {
        // Clear temp input
        lv_textarea_set_text(ui_txtKeyboardTempInput, "");
    }

    // Send message to WINPROC
    for (const auto& item : listVkCode)
    {
        if (item.first == obj)
        {
            keyboard_lib::SendKeyMessage(item.second, LV_EVENT_LONG_PRESSED);
            break;
        }
    }
}

void KeyboardScreen::SetKbData(SCREEN_NAME screen, void* data)
{
    switch (screen)
    {
    case SCREEN_NAME::VIDEO_ID_KBSCREEN:
        temp_data::VideoID.SetValue(*(std::string*)data);
        break;
    case SCREEN_NAME::VIDEO_NAME_KBSCREEN:
        temp_data::VideoName.SetValue(*(std::string*)data);
        break;
    case SCREEN_NAME::VIDEO_DESC_KBSCREEN:
        temp_data::VideoDesc.SetValue(*(std::string*)data);
        break;
    default:
        break;
    }
}
