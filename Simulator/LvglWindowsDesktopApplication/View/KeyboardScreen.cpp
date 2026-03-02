#include <ctype.h>
#include "ui.h"
#include "CommonData.h"
#include "CommonLibrary.h"
#include "ScreenMapping.h"
#include "KeyboardScreen.h"

std::unordered_map<SCREEN_NAME, KbInfo> KeyboardScreen::mapKbInit = {};
std::vector<std::pair<lv_obj_t*, int>> KeyboardScreen::listVkCode = {};
static lv_timer_t* timerUpdateInput = nullptr;
static lv_obj_t* dummyConfirmKey = nullptr;
static lv_obj_t* dummyOkKey = nullptr;
static lv_obj_t* dummyCancelKey = nullptr;
static lv_obj_t* dummyCharKey = nullptr;
static lv_obj_t* dummySpaceKey = nullptr;

KeyboardScreen::KeyboardScreen(SCREEN_NAME screen) : BaseScreen(screen)
{
    dummyConfirmKey = lv_button_create(nullptr);
    dummyOkKey = lv_button_create(nullptr);
    dummyCancelKey = lv_button_create(nullptr);
    dummyCharKey = lv_button_create(nullptr);
    dummySpaceKey = lv_button_create(nullptr);

    system_data::KeyboardType.SetValue(KEYBOARD_TYPE::STANDARD_KEYBOARD); //@todo: wait setting
    system_data::T9ConfirmTimeout.SetValue(750); //@todo: wait setting

    ListButtonCallback = {
        { ui_btnKeyboardCancel      , OnClickCancel       , LV_EVENT_CLICKED             },
        { ui_btnKeyboardOK          , OnClickOK           , LV_EVENT_CLICKED             },
        { ui_btnKeyboardKeyCaps     , OnClickKey          , LV_EVENT_CLICKED             },
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
        { dummyOkKey                , OnClickKey          , LV_EVENT_SHORT_CLICKED       },
        { dummyCancelKey            , OnClickKey          , LV_EVENT_SHORT_CLICKED       },
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
        { ui_btnKeyboardKey0        , OnReleaseKey        , LV_EVENT_RELEASED            },
        { ui_btnKeyboardKey1        , OnReleaseKey        , LV_EVENT_RELEASED            },
        { ui_btnKeyboardKey2        , OnReleaseKey        , LV_EVENT_RELEASED            },
        { ui_btnKeyboardKey3        , OnReleaseKey        , LV_EVENT_RELEASED            },
        { ui_btnKeyboardKey4        , OnReleaseKey        , LV_EVENT_RELEASED            },
        { ui_btnKeyboardKey5        , OnReleaseKey        , LV_EVENT_RELEASED            },
        { ui_btnKeyboardKey6        , OnReleaseKey        , LV_EVENT_RELEASED            },
        { ui_btnKeyboardKey7        , OnReleaseKey        , LV_EVENT_RELEASED            },
        { ui_btnKeyboardKey8        , OnReleaseKey        , LV_EVENT_RELEASED            },
        { ui_btnKeyboardKey9        , OnReleaseKey        , LV_EVENT_RELEASED            },
    };

    ListDataUpdateCallback = {};

    mapKbInit = {
        { SCREEN_NAME::VIDEO_ID_KBSCREEN  , { "Video ID"   , "._-#"    , []() { return temp_data::VideoID.GetValue(); }   } },
        { SCREEN_NAME::VIDEO_NAME_KBSCREEN, { "Video name" , ".-_#[]()", []() { return temp_data::VideoName.GetValue(); } } },
        { SCREEN_NAME::VIDEO_DESC_KBSCREEN, { "Description", ""        , []() { return temp_data::VideoDesc.GetValue(); } } },
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

    // Copy list VK code library
    keyboard_lib::SetListVkCode(listVkCode);

    // Init text input
    lv_label_set_text(ui_lblKeyboardTitle, mapKbInit[system_data::CurrentKbScreen.GetValue()].title.c_str());
    lv_textarea_set_text(ui_txtKeyboardInput, mapKbInit[system_data::CurrentKbScreen.GetValue()].cdataGetValueCallback().c_str());

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

    if ((system_data::KeyboardType.GetValue() == KEYBOARD_TYPE::INTERNAL_NUMPAD)
        || (system_data::KeyboardType.GetValue() == KEYBOARD_TYPE::STANDARD_NUMPAD))
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

    // Init acceptable punctuation
    lv_obj_add_flag(ui_imgAcceptPunct, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_txtAcceptPunct, LV_OBJ_FLAG_HIDDEN);

    if (mapKbInit[system_data::CurrentKbScreen.GetValue()].acceptedSpecialChars != "")
    {
        lv_obj_remove_flag(ui_imgAcceptPunct, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(ui_txtAcceptPunct, LV_OBJ_FLAG_HIDDEN);
        lv_textarea_set_text(ui_txtAcceptPunct, ("A-Z, a-z, 0-9, " + mapKbInit[system_data::CurrentKbScreen.GetValue()].acceptedSpecialChars).c_str());
    }

    // Auto update text input
    timerUpdateInput = lv_timer_create([](lv_timer_t* timer) {
        auto input = keyboard_lib::GetInputChar();
        auto combine = keyboard_lib::GetCombineChar();
        const auto& checklist = mapKbInit[system_data::CurrentKbScreen.GetValue()].acceptedSpecialChars;

        if ((system_data::KeyboardType.GetValue() == KEYBOARD_TYPE::INTERNAL_NUMPAD)
            || (system_data::KeyboardType.GetValue() == KEYBOARD_TYPE::STANDARD_NUMPAD))
        {
            if (input)
            {
                lv_textarea_add_char(ui_txtKeyboardTempInput, input);
            }

            if (combine)
            {
                if (ispunct(combine) && (checklist != "") && (checklist.find(combine) == std::string::npos))
                {
                    return;
                }

                lv_textarea_add_char(ui_txtKeyboardInput, combine);
            }
        }
        else
        {
            if (input)
            {
                if (ispunct(input) && (checklist != "") && (checklist.find(input) == std::string::npos))
                {
                    return;
                }

                lv_textarea_add_char(ui_txtKeyboardInput, input);
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
    // Apply input data
    auto input = std::string(lv_textarea_get_text(ui_txtKeyboardInput));

    SetKbData(system_data::CurrentKbScreen.GetValue(), (void*)&input);

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
    else if (obj == dummyOkKey)
    {
        OnClickOK(event);
    }
    else if (obj == dummyCancelKey)
    {
        OnClickCancel(event);
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
    for (const auto& item : listVkCode)
    {
        if (item.first == obj)
        {
            keyboard_lib::SendKeyMessage(item.second, LV_EVENT_SHORT_CLICKED);
            ::SetTimer(system_data::WindowHandle.GetValue(), TID_KEYDOWN, system_data::T9ConfirmTimeout.GetValue(), keyboard_lib::AutoConfirmKey);
            break;
        }
    }
}

void KeyboardScreen::OnLongPressKey(lv_event_t* event)
{
    // Send message to WINPROC
    for (const auto& item : listVkCode)
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

void KeyboardScreen::OnReleaseKey(lv_event_t* event)
{

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
