#include "CData.h"
#include "ui.h"
#include "CommonData.h"
#include "CommonLibrary.h"
#include "ScreenMapping.h"
#include "KeyboardScreen.h"

typedef struct
{
    std::string title;
    CData<std::string>* data;
} KbInfo;

static std::unordered_map<SCREEN_NAME, KbInfo> mapKbInit = {};
static lv_timer_t* timerUpdateInput = nullptr;
static lv_obj_t* dummyConfirmKey = nullptr;
static lv_obj_t* dummyOkKey = nullptr;
static lv_obj_t* dummyCancelKey = nullptr;
static lv_obj_t* dummyCharKey = nullptr;
static lv_obj_t* dummySpaceKey = nullptr;

static char GetCloseBracket(char input)
{
    switch (input)
    {
    case '(':
        return ')';
    case '[':
        return ']';
    default:
        return 0;
    }
}

static void AutoCloseBracket(char input)
{
    if (setting_data::AutoCloseBracketState.GetValue())
    {
        auto closeBracket = GetCloseBracket(input);
        if (closeBracket)
        {
            lv_textarea_add_char(ui_txtKeyboardInput, closeBracket);
            lv_textarea_cursor_left(ui_txtKeyboardInput);

        }
    }
}

static void AutoAddSpaceAfterPeriod()
{
    if (setting_data::InsSpaceAfterPuncState.GetValue())
    {
        if (lv_textarea_get_current_char(ui_txtKeyboardInput) == '.')
        {
            lv_textarea_add_char(ui_txtKeyboardInput, ' ');
        }
    }
}

KeyboardScreen::KeyboardScreen(SCREEN_NAME screen) : BaseScreen(screen)
{
    dummyConfirmKey = lv_button_create(nullptr);
    dummyOkKey = lv_button_create(nullptr);
    dummyCancelKey = lv_button_create(nullptr);
    dummyCharKey = lv_button_create(nullptr);
    dummySpaceKey = lv_button_create(nullptr);

    ListButtonCallback = {
        { ui_btnKeyboardCancel      , OnClickCancel       , LV_EVENT_CLICKED             },
        { ui_btnKeyboardOK          , OnClickOK           , LV_EVENT_CLICKED             },
        { ui_btnKeyboardKeyCaps     , OnClickKey          , LV_EVENT_CLICKED             },
        { ui_btnKeyboardKeyCaps     , OnClickKey          , LV_EVENT_SHORT_CLICKED       },
        { ui_btnKeyboardKey0        , OnClickKey          , LV_EVENT_SHORT_CLICKED       },
        { ui_btnKeyboardKey1        , OnClickKey          , LV_EVENT_SHORT_CLICKED       },
        { ui_btnKeyboardKey2        , OnClickKey          , LV_EVENT_SHORT_CLICKED       },
        { ui_btnKeyboardKey3        , OnClickKey          , LV_EVENT_SHORT_CLICKED       },
        { ui_btnKeyboardKey4        , OnClickKey          , LV_EVENT_SHORT_CLICKED       },
        { ui_btnKeyboardKey5        , OnClickKey          , LV_EVENT_SHORT_CLICKED       },
        { ui_btnKeyboardKey6        , OnClickKey          , LV_EVENT_SHORT_CLICKED       },
        { ui_btnKeyboardKey7        , OnClickKey          , LV_EVENT_SHORT_CLICKED       },
        { ui_btnKeyboardKey8        , OnClickKey          , LV_EVENT_SHORT_CLICKED       },
        { ui_btnKeyboardKey9        , OnClickKey          , LV_EVENT_SHORT_CLICKED       },
        { ui_btnKeyboardKeyBackspace, OnClickKey          , LV_EVENT_SHORT_CLICKED       },
        { ui_btnKeyboardKeyBack     , OnClickKey          , LV_EVENT_SHORT_CLICKED       },
        { ui_btnKeyboardKeyForward  , OnClickKey          , LV_EVENT_SHORT_CLICKED       },
        { dummyConfirmKey           , OnClickKey          , LV_EVENT_SHORT_CLICKED       },
        { dummyOkKey                , OnShortcutKey       , LV_EVENT_SHORT_CLICKED       },
        { dummyCancelKey            , OnShortcutKey       , LV_EVENT_SHORT_CLICKED       },
        { dummyCharKey              , OnClickKey          , LV_EVENT_SHORT_CLICKED       },
        { dummySpaceKey             , OnClickKey          , LV_EVENT_SHORT_CLICKED       },
        { ui_btnKeyboardKey0        , OnLongPressKey      , LV_EVENT_LONG_PRESSED        },
        { ui_btnKeyboardKey1        , OnLongPressKey      , LV_EVENT_LONG_PRESSED        },
        { ui_btnKeyboardKey2        , OnLongPressKey      , LV_EVENT_LONG_PRESSED        },
        { ui_btnKeyboardKey3        , OnLongPressKey      , LV_EVENT_LONG_PRESSED        },
        { ui_btnKeyboardKey4        , OnLongPressKey      , LV_EVENT_LONG_PRESSED        },
        { ui_btnKeyboardKey5        , OnLongPressKey      , LV_EVENT_LONG_PRESSED        },
        { ui_btnKeyboardKey6        , OnLongPressKey      , LV_EVENT_LONG_PRESSED        },
        { ui_btnKeyboardKey7        , OnLongPressKey      , LV_EVENT_LONG_PRESSED        },
        { ui_btnKeyboardKey8        , OnLongPressKey      , LV_EVENT_LONG_PRESSED        },
        { ui_btnKeyboardKey9        , OnLongPressKey      , LV_EVENT_LONG_PRESSED        },
        { ui_btnKeyboardKeyBackspace, OnLongPressRepeatKey, LV_EVENT_LONG_PRESSED_REPEAT },
        { ui_btnKeyboardKeyBack     , OnLongPressRepeatKey, LV_EVENT_LONG_PRESSED_REPEAT },
        { ui_btnKeyboardKeyForward  , OnLongPressRepeatKey, LV_EVENT_LONG_PRESSED_REPEAT },
    };

    ListDataUpdateCallback = {};

    mapKbInit = {
        { SCREEN_NAME::KBSCREEN_VIDEO_EVENT   , { "Event"          , &temp_data::VideoEvent    } },
        { SCREEN_NAME::KBSCREEN_VIDEO_NAME    , { "Video name"     , &temp_data::VideoName     } },
        { SCREEN_NAME::KBSCREEN_VIDEO_CATEGORY, { "New category"   , &temp_data::VideoCategory } },
        { SCREEN_NAME::KBSCREEN_VIDEO_DESC    , { "Description"    , &temp_data::VideoDesc     } },
        { SCREEN_NAME::KBSCREEN_VIDEO_AUTHOR  , { "Author"         , &temp_data::VideoAuthor   } },
        { SCREEN_NAME::KBSCREEN_VIDEO_SEARCH  , { "Search video"   , &input_data::VideoSearch  } },
        { SCREEN_NAME::KBSCREEN_SSID_PASSWORD , { "Password"       , &temp_data::WiFiPassword  } },
        { SCREEN_NAME::KBSCREEN_DUKTO_ADDR    , { "Dukto server IP", &temp_data::DuktoIP       } },
    };

    ListButtonVkCode = {
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
        { ui_btnKeyboardKeyCaps     , VK_CAPITAL },
        { ui_btnKeyboardKeyBackspace, VK_BACK    },
        { ui_btnKeyboardKeyBack     , VK_LEFT    },
        { ui_btnKeyboardKeyForward  , VK_RIGHT   },
        { dummyConfirmKey           , VK_CONVERT },
        { dummyOkKey                , VK_RETURN  },
        { dummyCancelKey            , VK_ESCAPE  },
        { dummyCharKey              , VK_CHAR    },
        { dummySpaceKey             , VK_SPACE   },
    };

    // Init text input
    lv_label_set_text(ui_lblKeyboardTitle, mapKbInit[system_data::CurrentKbScreen.GetValue()].title.c_str());
    lv_textarea_set_text(ui_txtKeyboardInput, mapKbInit[system_data::CurrentKbScreen.GetValue()].data->GetValue().c_str());

    // Init numpad key
    lv_obj_add_state(ui_btnKeyboardKey0, LV_STATE_DISABLED);
    lv_obj_add_state(ui_btnKeyboardKey1, LV_STATE_DISABLED);
    lv_obj_add_state(ui_btnKeyboardKey2, LV_STATE_DISABLED);
    lv_obj_add_state(ui_btnKeyboardKey3, LV_STATE_DISABLED);
    lv_obj_add_state(ui_btnKeyboardKey4, LV_STATE_DISABLED);
    lv_obj_add_state(ui_btnKeyboardKey5, LV_STATE_DISABLED);
    lv_obj_add_state(ui_btnKeyboardKey6, LV_STATE_DISABLED);
    lv_obj_add_state(ui_btnKeyboardKey7, LV_STATE_DISABLED);
    lv_obj_add_state(ui_btnKeyboardKey8, LV_STATE_DISABLED);
    lv_obj_add_state(ui_btnKeyboardKey9, LV_STATE_DISABLED);

    if ((setting_data::KeyboardType.GetValue() == (short)dropdownlist_lib::DD_KEYBOARD_TYPE_e::Internal_numpad)
        || (setting_data::KeyboardType.GetValue() == (short)dropdownlist_lib::DD_KEYBOARD_TYPE_e::Standard_numpad)
        )
    {
        lv_obj_remove_state(ui_btnKeyboardKey0, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_btnKeyboardKey1, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_btnKeyboardKey2, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_btnKeyboardKey3, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_btnKeyboardKey4, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_btnKeyboardKey5, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_btnKeyboardKey6, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_btnKeyboardKey7, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_btnKeyboardKey8, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_btnKeyboardKey9, LV_STATE_DISABLED);
    }

    // Init caps key
    if (keyboard_lib::GetKeyboardCapsState())
    {
        lv_obj_add_state(ui_btnKeyboardKeyCaps, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_remove_state(ui_btnKeyboardKeyCaps, LV_STATE_CHECKED);
    }

    // Init shortcut image
    if (setting_data::KeyboardExitShortcutState.GetValue())
    {
        lv_obj_remove_flag(ui_imgCancelKey, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(ui_imgEnterKey, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(ui_imgCancelKey, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_imgEnterKey, LV_OBJ_FLAG_HIDDEN);
    }

    // Auto update text input
    timerUpdateInput = lv_timer_create([](lv_timer_t* timer) {
        auto input = keyboard_lib::GetInputChar();
        auto combine = keyboard_lib::GetCombineChar();

        if ((setting_data::KeyboardType.GetValue() == (short)dropdownlist_lib::DD_KEYBOARD_TYPE_e::Internal_numpad)
            || (setting_data::KeyboardType.GetValue() == (short)dropdownlist_lib::DD_KEYBOARD_TYPE_e::Standard_numpad)
            )
        {
            if (input)
            {
                lv_textarea_add_char(ui_txtKeyboardTempInput, input);
            }

            if (combine)
            {
                lv_textarea_add_char(ui_txtKeyboardInput, combine);
                AutoCloseBracket(combine);
                AutoAddSpaceAfterPeriod();
            }
        }
        else
        {
            if (input)
            {
                lv_textarea_add_char(ui_txtKeyboardInput, input);
                AutoCloseBracket(input);
                AutoAddSpaceAfterPeriod();
            }
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

void KeyboardScreen::OnClickCancel(lv_event_t* event)
{
    ScreenMapping::GetInstance().ChangeScreen(system_data::CurrentScreen.GetValue());
}

void KeyboardScreen::OnClickOK(lv_event_t* event)
{
    auto input = lv_textarea_get_text(ui_txtKeyboardInput);

    // Set input to common data
    mapKbInit[system_data::CurrentKbScreen.GetValue()].data->SetValue(input);

    ScreenMapping::GetInstance().ChangeScreen(system_data::CurrentScreen.GetValue());
}

void KeyboardScreen::OnClickKey(lv_event_t* event)
{
    auto obj = (lv_obj_t*)(event->current_target);

    if (obj == dummyConfirmKey)
    {
        // Clear temp input
        lv_textarea_set_text(ui_txtKeyboardTempInput, "");
    }
    else if (obj == ui_btnKeyboardKeyBackspace)
    {
        // Delete the latest character
        lv_textarea_delete_char(ui_txtKeyboardInput);
    }
    else if (obj == ui_btnKeyboardKeyBack)
    {
        lv_textarea_cursor_left(ui_txtKeyboardInput);
    }
    else if (obj == ui_btnKeyboardKeyForward)
    {
        lv_textarea_cursor_right(ui_txtKeyboardInput);
    }
    else if (obj == ui_btnKeyboardKeyCaps)
    {
        // Toggle key
        if ((lv_obj_get_state(ui_btnKeyboardKeyCaps) & LV_STATE_CHECKED) == LV_STATE_CHECKED)
        {
            lv_obj_remove_state(obj, LV_STATE_CHECKED);
            keyboard_lib::SetKeyboardKeyState(VK_CAPITAL, false);
        }
        else
        {
            lv_obj_add_state(obj, LV_STATE_CHECKED);
            keyboard_lib::SetKeyboardKeyState(VK_CAPITAL, true);
        }
    }
    else if (obj == dummySpaceKey)
    {
        // Add space
        lv_textarea_add_char(ui_txtKeyboardInput, ' ');
    }

    // Send message to WINPROC
    for (const auto& item : keyboard_lib::GetListVkCode())
    {
        if (item.first == obj)
        {
            keyboard_lib::SendKeyMessage(item.second, LV_EVENT_SHORT_CLICKED);
            ::SetTimer(system_data::WindowHandle.GetValue(), TID_KEYDOWN, keyboard_lib::GetAutoConfirmTimeMs(), keyboard_lib::AutoConfirmKey);
            break;
        }
    }
}

void KeyboardScreen::OnLongPressKey(lv_event_t* event)
{
    // Send message to WINPROC
    for (const auto& item : keyboard_lib::GetListVkCode())
    {
        if (item.first == event->current_target)
        {
            keyboard_lib::SendKeyMessage(item.second, LV_EVENT_LONG_PRESSED);
            break;
        }
    }
}

void KeyboardScreen::OnLongPressRepeatKey(lv_event_t* event)
{
    auto obj = (lv_obj_t*)(event->current_target);

    if (obj == ui_btnKeyboardKeyBackspace)
    {
        // Delete the latest character
        lv_textarea_delete_char(ui_txtKeyboardInput);
    }
    else if (obj == ui_btnKeyboardKeyBack)
    {
        lv_textarea_cursor_left(ui_txtKeyboardInput);
    }
    else if (obj == ui_btnKeyboardKeyForward)
    {
        lv_textarea_cursor_right(ui_txtKeyboardInput);
    }
}

void KeyboardScreen::OnShortcutKey(lv_event_t* event)
{
    if (setting_data::KeyboardExitShortcutState.GetValue())
    {
        auto obj = (lv_obj_t*)(event->current_target);

        if (obj == dummyOkKey)
        {
            OnClickOK(event);
        }
        else if (obj == dummyCancelKey)
        {
            OnClickCancel(event);
        }
    }
}
