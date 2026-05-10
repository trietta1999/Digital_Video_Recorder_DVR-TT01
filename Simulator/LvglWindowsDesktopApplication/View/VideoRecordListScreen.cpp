#include <thread>
#include "ui.h"
#include "CommonData.h"
#include "CommonLibrary.h"
#include "ScreenMapping.h"
#include "VideoRecordListScreen.h"

struct RowInfo_t
{
    lv_obj_t* rowObj;
    lv_obj_t* checkboxObj;
    lv_obj_t* checkboxTransferObj;
    lv_obj_t* datetimeObj;
    lv_obj_t* nameObj;
    bool isCheck;
    bool isTransfered;
    std::string datetime;
    std::string name;
    std::string id;

    bool operator==(const RowInfo_t& other)
    {
        return ((other.isCheck == this->isCheck)
            && (other.datetime == this->datetime)
            && (other.name == this->name)
            && (other.id == this->id)
            );
    }

    void CreateUI(lv_obj_t* row, lv_obj_t* checkbox, lv_obj_t* checkboxTransfer, lv_obj_t* datetime, lv_obj_t* name)
    {
        this->rowObj = row;
        this->checkboxObj = checkbox;
        this->checkboxTransferObj = checkboxTransfer;
        this->datetimeObj = datetime;
        this->nameObj = name;
    }

    void CreateData(std::string datetime, std::string name, std::string id)
    {
        lv_label_set_text(this->datetimeObj, datetime.c_str());
        lv_label_set_text(this->nameObj, name.c_str());
        lv_obj_add_state(this->checkboxTransferObj, LV_STATE_CHECKED);

        this->isCheck = false;
        this->isTransfered = false;
        this->datetime = datetime;
        this->name = name;
        this->id = id;
    }

    void UpdateRow()
    {
        if ((lv_obj_get_state(rowObj) & LV_STATE_CHECKED) == LV_STATE_CHECKED)
        {
            isCheck = true;
        }
        else
        {
            isCheck = false;
        }

        if (isCheck)
        {
            lv_obj_add_state(checkboxObj, LV_STATE_CHECKED);
        }
        else
        {
            lv_obj_remove_state(checkboxObj, LV_STATE_CHECKED);
        }
    }

    void UpdateTransferState() const
    {
        if (!isTransfered)
        {
            lv_obj_add_state(checkboxTransferObj, LV_STATE_CHECKED);
        }
        else
        {
            lv_obj_remove_state(checkboxTransferObj, LV_STATE_CHECKED);
        }
    }

    void DeleteRow() const
    {
        lv_obj_delete(rowObj);
    }
};

static std::vector<RowInfo_t> listRowInfo = {};
static std::vector<RowInfo_t> listRowInfoSorted = {};
static std::vector<RowInfo_t> listRowInfoSelected = {};
static std::vector<std::pair<std::string, bool>> listTransferState = {};
static std::vector<storage_lib::RemovableDriveInfo_t> listExtDrive = {};
static std::vector<std::pair<lv_obj_t*, int>> listVkCode = {};
static lv_obj_t* dummyUpKey = nullptr;
static lv_obj_t* dummyDownKey = nullptr;
static lv_obj_t* dummyTopKey = nullptr;
static lv_obj_t* dummyBottomKey = nullptr;
static short totalRowHeight = 0;
static short deleteTimeCounter = 0;
static int copyItemCount = 0;

static RowInfo_t CreateRowUI()
{
    auto ui_conRLItemCustomRow1 = lv_obj_create(ui_conRL);
    lv_obj_remove_style_all(ui_conRLItemCustomRow1);
    lv_obj_set_height(ui_conRLItemCustomRow1, 110);
    lv_obj_set_width(ui_conRLItemCustomRow1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_conRLItemCustomRow1, -419);
    lv_obj_set_y(ui_conRLItemCustomRow1, -51);
    lv_obj_set_align(ui_conRLItemCustomRow1, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_conRLItemCustomRow1, LV_FLEX_FLOW_COLUMN_WRAP);
    lv_obj_set_flex_align(ui_conRLItemCustomRow1, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_obj_add_flag(ui_conRLItemCustomRow1, LV_OBJ_FLAG_CHECKABLE);     /// Flags
    lv_obj_remove_flag(ui_conRLItemCustomRow1,
        LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
        LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_border_color(ui_conRLItemCustomRow1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_conRLItemCustomRow1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_conRLItemCustomRow1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_conRLItemCustomRow1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_conRLItemCustomRow1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_conRLItemCustomRow1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_conRLItemCustomRow1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_conRLItemCustomRow1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_conRLItemCustomRow1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_conRLItemCustomRow1, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_opa(ui_conRLItemCustomRow1, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui_conRLItemCustomRow1, 5, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_add_event_cb(ui_conRLItemCustomRow1, [](lv_event_t* e) {
        lv_event_code_t event_code = lv_event_get_code(e);
        if (event_code == LV_EVENT_CLICKED)
        {
            VideoRecordListScreen::OnClickItem(e);
        }
        }, LV_EVENT_ALL, NULL);

    auto ui_cbRLItemCustom1 = lv_checkbox_create(ui_conRLItemCustomRow1);
    lv_obj_set_width(ui_cbRLItemCustom1, 50);
    lv_obj_set_height(ui_cbRLItemCustom1, 45);
    lv_obj_set_x(ui_cbRLItemCustom1, -106);
    lv_obj_set_y(ui_cbRLItemCustom1, -98);
    lv_obj_set_align(ui_cbRLItemCustom1, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_cbRLItemCustom1, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
        LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
        LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_text_font(ui_cbRLItemCustom1, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_image_src(ui_cbRLItemCustom1, &ui_img_check_png, LV_PART_INDICATOR | LV_STATE_CHECKED);

    auto ui_cbRLItemCustomTransfer1 = lv_checkbox_create(ui_conRLItemCustomRow1);
    lv_obj_set_width(ui_cbRLItemCustomTransfer1, 50);
    lv_obj_set_height(ui_cbRLItemCustomTransfer1, 45);
    lv_obj_set_x(ui_cbRLItemCustomTransfer1, -106);
    lv_obj_set_y(ui_cbRLItemCustomTransfer1, -99);
    lv_obj_set_align(ui_cbRLItemCustomTransfer1, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_cbRLItemCustomTransfer1,
        LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
        LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
        LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_text_font(ui_cbRLItemCustomTransfer1, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_cbRLItemCustomTransfer1, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_cbRLItemCustomTransfer1, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_src(ui_cbRLItemCustomTransfer1, &ui_img_folder_check_png, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_cbRLItemCustomTransfer1, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_cbRLItemCustomTransfer1, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_cbRLItemCustomTransfer1, 0, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_image_src(ui_cbRLItemCustomTransfer1, &ui_img_folder_limited_png, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_image_recolor(ui_cbRLItemCustomTransfer1, lv_color_hex(0xFF0000), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_image_recolor_opa(ui_cbRLItemCustomTransfer1, 255, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui_cbRLItemCustomTransfer1, 0, LV_PART_INDICATOR | LV_STATE_CHECKED);

    auto ui_lblRLItemCustomDateTime1 = lv_label_create(ui_conRLItemCustomRow1);
    lv_obj_set_width(ui_lblRLItemCustomDateTime1, 265);
    lv_obj_set_height(ui_lblRLItemCustomDateTime1, 100);
    lv_obj_set_x(ui_lblRLItemCustomDateTime1, 75);
    lv_obj_set_y(ui_lblRLItemCustomDateTime1, 184);
    lv_obj_set_align(ui_lblRLItemCustomDateTime1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_lblRLItemCustomDateTime1, "");
    lv_obj_remove_flag(ui_lblRLItemCustomDateTime1,
        LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
        LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
        LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_text_color(ui_lblRLItemCustomDateTime1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_lblRLItemCustomDateTime1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_lblRLItemCustomDateTime1, &lv_font_montserrat_44, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_lblRLItemCustomDateTime1, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_lblRLItemCustomDateTime1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    auto ui_lblRLItemCustomName1 = lv_label_create(ui_conRLItemCustomRow1);
    lv_obj_set_width(ui_lblRLItemCustomName1, 840);
    lv_obj_set_height(ui_lblRLItemCustomName1, 100);
    lv_obj_set_x(ui_lblRLItemCustomName1, 38);
    lv_obj_set_y(ui_lblRLItemCustomName1, 189);
    lv_obj_set_align(ui_lblRLItemCustomName1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_lblRLItemCustomName1, "");
    lv_obj_remove_flag(ui_lblRLItemCustomName1,
        LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
        LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
        LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_text_color(ui_lblRLItemCustomName1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_lblRLItemCustomName1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_lblRLItemCustomName1, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_lblRLItemCustomName1, lv_color_hex(0x424242), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_lblRLItemCustomName1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    RowInfo_t info = { 0 };
    info.CreateUI(ui_conRLItemCustomRow1, ui_cbRLItemCustom1, ui_cbRLItemCustomTransfer1, ui_lblRLItemCustomDateTime1, ui_lblRLItemCustomName1);

    return info;
}

VideoRecordListScreen::VideoRecordListScreen(SCREEN_NAME screen) : BaseScreen(screen)
{
    dummyUpKey = lv_button_create(nullptr);
    dummyDownKey = lv_button_create(nullptr);
    dummyTopKey = lv_button_create(nullptr);
    dummyBottomKey = lv_button_create(nullptr);

    ListButtonCallback = {
        { ui_btnRLBack       , OnClickBack      , LV_EVENT_CLICKED             },
        { ui_btnRLTopPage    , OnClickPageChange, LV_EVENT_CLICKED             },
        { ui_btnRLBottomPage , OnClickPageChange, LV_EVENT_CLICKED             },
        { ui_btnRLPrePage    , OnClickPageChange, LV_EVENT_SHORT_CLICKED       },
        { ui_btnRLNextPage   , OnClickPageChange, LV_EVENT_SHORT_CLICKED       },
        { ui_btnRLPrePage    , OnClickPageChange, LV_EVENT_LONG_PRESSED_REPEAT },
        { ui_btnRLNextPage   , OnClickPageChange, LV_EVENT_LONG_PRESSED_REPEAT },
        { ui_btnRLPlay       , OnClickOperator  , LV_EVENT_CLICKED             },
        { ui_btnRLPause      , OnClickOperator  , LV_EVENT_CLICKED             },
        { ui_btnRLStop       , OnClickOperator  , LV_EVENT_CLICKED             },
        { ui_btnRLSound      , OnClickOperator  , LV_EVENT_CLICKED             },
        { ui_btnRLFastForward, OnClickOperator  , LV_EVENT_CLICKED             },
        { ui_btnRLFastRewind , OnClickOperator  , LV_EVENT_CLICKED             },
        { ui_btnRLDukto      , OnClickOperator  , LV_EVENT_CLICKED             },
        { ui_btnRLTransfer   , OnClickTransfer  , LV_EVENT_CLICKED             },
        { ui_barDeleteWaiting, OnDelete         , LV_EVENT_LONG_PRESSED_REPEAT },
        { ui_barDeleteWaiting, OnDelete         , LV_EVENT_RELEASED            },
        { ui_cbRLItemAll     , OnClickAllItem   , LV_EVENT_CLICKED             },
        { dummyUpKey         , OnClickPageChange, LV_EVENT_LONG_PRESSED_REPEAT },
        { dummyDownKey       , OnClickPageChange, LV_EVENT_LONG_PRESSED_REPEAT },
        { dummyUpKey         , OnClickPageChange, LV_EVENT_SHORT_CLICKED       },
        { dummyDownKey       , OnClickPageChange, LV_EVENT_SHORT_CLICKED       },
        { dummyTopKey        , OnClickPageChange, LV_EVENT_SHORT_CLICKED       },
        { dummyBottomKey     , OnClickPageChange, LV_EVENT_SHORT_CLICKED       },
    };

    ListDataUpdateCallback = {
        { []() { return system_data::DeviceChange.GetState();      }, UpdateExtDevice        },
        { []() { return system_data::CurrentState.GetState();      }, UpdateButton           },
        { []() { return system_data::CurrentSoundState.GetState(); }, UpdateButton           },
        { []() { return system_data::TransferPercent.GetState();   }, UpdateTransferProgress },
    };

    listVkCode = {
        { dummyUpKey    , VK_UP    },
        { dummyDownKey  , VK_DOWN  },
        { dummyTopKey   , VK_PRIOR },
        { dummyBottomKey, VK_NEXT  },
    };

    // Copy list VK code library
    keyboard_lib::SetListVkCode(listVkCode);

    // Get video info from data
    auto listVideoInfo = recordlist_lib::GetVideoInfoListData();

    // Calculate row height
    auto rowSpacing = lv_obj_get_style_pad_row(ui_conRL, LV_STYLE_PAD_ROW);
    auto rowHeight = lv_obj_get_height(ui_lblRLItemName1);
    auto borderWidth = lv_obj_get_style_border_width(ui_conRLItemRow1, LV_PART_MAIN | LV_STATE_DEFAULT);
    auto totalHeight = rowHeight + borderWidth * 2 + rowSpacing;

    // Clear template item
    lv_obj_clean(ui_conRL);

    // Init video info list
    for (const auto& videoInfo : listVideoInfo)
    {
        char datetime[MAX_PATH] = { 0 };
        auto rowInfo = CreateRowUI();

        sprintf(datetime, "%02d.%02d.%02d\n%02d:%02d:%02d",
            videoInfo.datetime.wDay, videoInfo.datetime.wMonth, videoInfo.datetime.wYear,
            videoInfo.datetime.wHour, videoInfo.datetime.wMinute, videoInfo.datetime.wSecond); // @todo: wait setting

        rowInfo.CreateData(datetime, videoInfo.videoName, videoInfo.videoID);

        // Find if the latest transfer state is exist
        auto find = std::find_if(listTransferState.begin(), listTransferState.end(),
            [rowInfo](const std::pair<std::string, bool>& item) {
                return rowInfo.id == item.first;
            });

        // Update transfer state
        if (find != listTransferState.end())
        {
            rowInfo.isTransfered = find->second;
            rowInfo.UpdateTransferState();
        }

        listRowInfo.push_back(rowInfo);

        // Calculate the total height for one item
        totalRowHeight += totalHeight;
    }

    // Excluding the total height for the first page
    totalRowHeight -= totalHeight * RL_ITEM_PER_PAGE;

    // Init state
    system_data::CurrentState.SetValue(STATE_TYPE::S_STOP);
}

VideoRecordListScreen::~VideoRecordListScreen()
{
    listRowInfo.clear();
    listRowInfoSorted.clear();
    listRowInfoSelected.clear();
}

void VideoRecordListScreen::OnClickBack(lv_event_t* event)
{
    ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::SCREEN_MAIN);
}

void VideoRecordListScreen::OnClickPageChange(lv_event_t* event)
{
    // Get current vertical scroll position
    auto y = lv_obj_get_scroll_y(ui_conRL);

    // Handle Scroll Up or Previous Page action
    if ((event->current_target == ui_btnRLPrePage) || (event->current_target == dummyUpKey))
    {
        if (y > 0)
        {
            // If remaining scroll distance is less than a step, scroll to the top
            if (y < SCROLL_Y_STEP)
            {
                lv_obj_scroll_by(ui_conRL, 0, y, LV_ANIM_ON);
            }
            // Otherwise, scroll up by one standard step
            else
            {
                lv_obj_scroll_by(ui_conRL, 0, SCROLL_Y_STEP, LV_ANIM_ON);
            }
        }
    }
    // Handle Scroll Down or Next Page action
    else if ((event->current_target == ui_btnRLNextPage) || (event->current_target == dummyDownKey))
    {
        if (y < totalRowHeight)
        {
            // If next step exceeds total height, scroll exactly to the bottom
            if (y + SCROLL_Y_STEP > totalRowHeight)
            {
                lv_obj_scroll_to_y(ui_conRL, totalRowHeight, LV_ANIM_ON);
            }
            // Otherwise, scroll down by one standard step
            else
            {
                lv_obj_scroll_by(ui_conRL, 0, -SCROLL_Y_STEP, LV_ANIM_ON);
            }
        }
    }
    // Handle Scroll to Top action
    else if ((event->current_target == ui_btnRLTopPage) || (event->current_target == dummyTopKey))
    {
        lv_obj_scroll_to_y(ui_conRL, 0, LV_ANIM_ON);
    }
    // Handle Scroll to Bottom action
    else if ((event->current_target == ui_btnRLBottomPage) || (event->current_target == dummyBottomKey))
    {
        lv_obj_scroll_to_y(ui_conRL, totalRowHeight, LV_ANIM_ON);
    }
}

void VideoRecordListScreen::OnClickItem(lv_event_t* event)
{
    // Iterate through the list of row information to find the interacted row
    for (auto& rowInfo : listRowInfo)
    {
        // Check if the current event target matches the UI object of the row
        if (event->current_target == rowInfo.rowObj)
        {
            // Toggle or update the visual and data state of the selected row
            rowInfo.UpdateRow();

            // If the row is now marked as checked, add it to the selection list
            if (rowInfo.isCheck)
            {
                listRowInfoSelected.push_back(rowInfo);
            }
            // If unchecked, remove the corresponding entry from the selection list
            else
            {
                // Use a lambda to find and erase the matching row based on unique attributes
                std::erase_if(listRowInfoSelected, [&](const RowInfo_t& it) {
                    return ((it.isCheck != rowInfo.isCheck)
                        && (it.datetime == rowInfo.datetime)
                        && (it.name == rowInfo.name)
                        && (it.id == rowInfo.id)
                        );
                    });
            }

            // Refresh the state of related UI buttons
            UpdateButton();

            break;
        }
    }
}

void VideoRecordListScreen::OnClickAllItem(lv_event_t* event)
{
    // Check the current state of the "Select All" checkbox
    auto isCheck = (lv_obj_get_state(ui_cbRLItemAll) & LV_STATE_CHECKED) == LV_STATE_CHECKED;

    // Clear the current selection list to prepare for synchronization
    listRowInfoSelected.clear();

    for (auto& rowInfo : listRowInfo)
    {
        // Apply the master checkbox state to each row's UI object
        if (isCheck)
        {
            lv_obj_add_state(rowInfo.rowObj, LV_STATE_CHECKED);
        }
        else
        {
            lv_obj_remove_state(rowInfo.rowObj, LV_STATE_CHECKED);
        }

        // Synchronize internal data and visual elements for the row
        rowInfo.UpdateRow();

        // If "Select All" is active, add the updated row info to the selection list
        if (isCheck)
        {
            listRowInfoSelected.push_back(rowInfo);
        }
    }

    // Refresh the state of control buttons (e.g., Enable/Disable Delete)
    UpdateButton();
}

void VideoRecordListScreen::OnClickOperator(lv_event_t* event)
{

    if (event->current_target == ui_btnRLPlay)
    {
        if (system_data::CurrentState.GetValue() == STATE_TYPE::S_STOP)
        {
            // Show play sub-screen
            videorecord_lib::StartExternalWindow(ui_conRL, PLAY_SCREENNAME, listRowInfoSelected.front().id);
        }
        else if (system_data::CurrentState.GetValue() == STATE_TYPE::S_PAUSE)
        {
            // Send unpause
            videorecord_lib::ExecutePause();
        }

        system_data::CurrentState.SetValue(STATE_TYPE::S_PLAY);
    }
    else if (event->current_target == ui_btnRLPause)
    {
        // Send pause
        videorecord_lib::ExecutePause();

        system_data::CurrentState.SetValue(STATE_TYPE::S_PAUSE);
    }
    else if (event->current_target == ui_btnRLStop)
    {
        if ((system_data::CurrentState.GetValue() == STATE_TYPE::S_PLAY)
            || (system_data::CurrentState.GetValue() == STATE_TYPE::S_PAUSE)
            )
        {
            // Stop sub-screen
            videorecord_lib::StopExternalWindow();
        }

        system_data::CurrentState.SetValue(STATE_TYPE::S_STOP);
    }
    else if (event->current_target == ui_btnRLSound)
    {
        if ((lv_obj_get_state(ui_btnRLSound) & LV_STATE_CHECKED) == LV_STATE_CHECKED)
        {
            system_data::CurrentSoundState.SetValue(false);
        }
        else
        {
            system_data::CurrentSoundState.SetValue(true);
        }

        videorecord_lib::SetSoundState();
    }
    else if (event->current_target == ui_btnRLFastRewind)
    {
        videorecord_lib::ExecuteSeek(true, false);
    }
    else if (event->current_target == ui_btnRLFastForward)
    {
        videorecord_lib::ExecuteSeek(false, true);
    }
    else if (event->current_target == ui_btnRLDukto)
    {
        //system("start \"\" \"C:\\Program Files (x86)\\Dukto\\dukto.exe\"");
        //videorecord_lib::StartExternalWindow(ui_conRL, DUKTO_SCREENNAME, "");
        // @todo: wait setting
    }
}

void VideoRecordListScreen::OnClickTransfer(lv_event_t* event)
{
    system_data::CurrentState.SetValue(STATE_TYPE::S_TRANSFER);

    char selectDrive[MAX_PATH] = { 0 };
    std::string driveLetter = "";
    int totalPercent = listRowInfoSelected.size() * STANDARD_PERCENT;

    // Get selected drive
    lv_dropdown_get_selected_str(ui_dropPortableMemory, selectDrive, sizeof(selectDrive));

    // Get drive letter from list
    for (const auto& driveInfo : listExtDrive)
    {
        if (driveInfo.label == selectDrive)
        {
            driveLetter = driveInfo.letter;
            break;
        }
    }

    // Set total percent
    lv_bar_set_range(ui_barTransferProgress, 0, totalPercent);
    lv_obj_remove_flag(ui_barTransferProgress, LV_OBJ_FLAG_HIDDEN);

    // Execute copy
    std::thread([driveLetter]() {
        for (auto& rowInfo : listRowInfoSelected)
        {
            rowInfo.isTransfered = recordlist_lib::ExportVideoToExternalDrive(rowInfo.id, rowInfo.name, driveLetter);
            copyItemCount++;
        }

        copyItemCount = 0;
        system_data::CurrentState.SetValue(STATE_TYPE::S_STOP);
        }).detach();
}

void VideoRecordListScreen::OnDelete(lv_event_t* event)
{
    // Check if the input is a continuous long press event
    if (event->code == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        // Increment the counter based on the task cycle time (10ms)
        deleteTimeCounter += TIMECYCLE_10MS;

        // Check if the hold duration has reached the maximum threshold defined by the progress bar
        if (deleteTimeCounter >= lv_bar_get_max_value(ui_barDeleteWaiting))
        {
            // Iterate through the selected rows and perform deletion
            for (auto& rowInfo : listRowInfoSelected)
            {
                // Remove the row from the UI display
                rowInfo.DeleteRow();
                // Delete the actual data/file via the video information library
                videoinfo_lib::DeleteData(rowInfo.id);
            }

            // Clear the list after successful deletion
            listRowInfoSelected.clear();
            listRowInfo.clear();

            // Refresh UI buttons to reflect the new state
            UpdateButton();

            // Manually trigger a release event to reset the waiting bar state
            lv_obj_send_event(ui_barDeleteWaiting, LV_EVENT_RELEASED, nullptr);
        }
    }
    // Reset the counter if the user releases the button early
    else if (event->code == LV_EVENT_RELEASED)
    {
        deleteTimeCounter = 0;
    }

    // Update the progress bar visually to provide feedback during the long press
    lv_bar_set_value(ui_barDeleteWaiting, deleteTimeCounter, LV_ANIM_ON);
}

void VideoRecordListScreen::UpdateButton()
{
    auto oldState = system_data::CurrentState.GetOldValue();
    auto state = system_data::CurrentState.GetValue();

    lv_obj_remove_state(ui_btnVideoSearch, LV_STATE_DISABLED);
    lv_obj_remove_state(ui_dropVideoFilter, LV_STATE_DISABLED);
    lv_obj_remove_state(ui_btnRLPlay, LV_STATE_DISABLED);
    lv_obj_remove_state(ui_btnRLPause, LV_STATE_DISABLED);
    lv_obj_remove_state(ui_btnRLStop, LV_STATE_DISABLED);
    lv_obj_remove_state(ui_btnRLSound, LV_STATE_DISABLED);
    lv_obj_remove_state(ui_cbRLItemAll, LV_STATE_DISABLED);
    lv_obj_remove_state(ui_btnRLFastForward, LV_STATE_DISABLED);
    lv_obj_remove_state(ui_btnRLFastRewind, LV_STATE_DISABLED);
    lv_obj_remove_state(ui_barDeleteWaiting, LV_STATE_DISABLED);
    lv_obj_remove_state(ui_btnRLNewRecord, LV_STATE_DISABLED);
    lv_obj_remove_state(ui_dropPortableMemory, LV_STATE_DISABLED);
    lv_obj_remove_state(ui_btnRLTransfer, LV_STATE_DISABLED);
    lv_obj_remove_state(ui_btnRLDukto, LV_STATE_DISABLED);
    lv_obj_remove_state(ui_btnRLBack, LV_STATE_DISABLED);

    // Update Search button and Filter dropdown
    if (state != STATE_TYPE::S_STOP)
    {
        lv_obj_add_state(ui_btnVideoSearch, LV_STATE_DISABLED);
        lv_obj_add_state(ui_dropVideoFilter, LV_STATE_DISABLED);
    }

    // Update Play button
    if ((listRowInfoSelected.size() != 1)
        || (state == STATE_TYPE::S_PLAY)
        )
    {
        lv_obj_add_state(ui_btnRLPlay, LV_STATE_DISABLED);
    }

    // Update Pause button
    if ((state == STATE_TYPE::S_PAUSE)
        || (state == STATE_TYPE::S_STOP)
        )
    {
        lv_obj_add_state(ui_btnRLPause, LV_STATE_DISABLED);
    }

    // Update Stop button
    if (state == STATE_TYPE::S_STOP)
    {
        lv_obj_add_state(ui_btnRLStop, LV_STATE_DISABLED);
    }

    // Update Sound button
    if (system_data::CurrentSoundState.GetValue())
    {
        lv_obj_remove_state(ui_btnRLSound, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_add_state(ui_btnRLSound, LV_STATE_CHECKED);
    }

    // Update Seek button
    if (state != STATE_TYPE::S_PLAY)
    {
        lv_obj_add_state(ui_btnRLFastForward, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnRLFastRewind, LV_STATE_DISABLED);
    }

    // Update New Record button
    if ((listRowInfoSelected.size() != 1)
        || (state != STATE_TYPE::S_STOP)
        )
    {
        lv_obj_add_state(ui_btnRLNewRecord, LV_STATE_DISABLED);
    }

    // Update Delete button
    if ((!listRowInfoSelected.size())
        || (state != STATE_TYPE::S_STOP))
    {
        lv_obj_add_state(ui_barDeleteWaiting, LV_STATE_DISABLED);
    }

    // Update portable memory dropdown
    if (!listExtDrive.size())
    {
        lv_obj_add_state(ui_dropPortableMemory, LV_STATE_DISABLED);
    }

    // Update Transfer button
    if ((!listRowInfoSelected.size())
        || (state != STATE_TYPE::S_STOP)
        || (!listExtDrive.size())
        )
    {
        lv_obj_add_state(ui_btnRLTransfer, LV_STATE_DISABLED);
    }

    // Update Dukto button
    if (state != STATE_TYPE::S_STOP)
    {
        lv_obj_add_state(ui_btnRLTransfer, LV_STATE_DISABLED);
    }

    // Update Back button
    if (state != STATE_TYPE::S_STOP)
    {
        lv_obj_add_state(ui_btnRLBack, LV_STATE_DISABLED);
    }

    // Update Select All button
    if (!listRowInfo.size())
    {
        lv_obj_add_state(ui_cbRLItemAll, LV_STATE_DISABLED);
        lv_obj_remove_state(ui_cbRLItemAll, LV_STATE_CHECKED);
    }
    else
    {
        if (listRowInfoSelected.size() == listRowInfo.size())
        {
            lv_obj_add_state(ui_cbRLItemAll, LV_STATE_CHECKED);
        }
        else
        {
            lv_obj_remove_state(ui_cbRLItemAll, LV_STATE_CHECKED);
        }
    }

    // Disable all button when transfering
    if (state == STATE_TYPE::S_TRANSFER)
    {
        lv_obj_add_state(ui_btnVideoSearch, LV_STATE_DISABLED);
        lv_obj_add_state(ui_dropVideoFilter, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnRLPlay, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnRLPause, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnRLStop, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnRLSound, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnRLFastForward, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnRLFastRewind, LV_STATE_DISABLED);
        lv_obj_add_state(ui_barDeleteWaiting, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnRLNewRecord, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnRLTransfer, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnRLDukto, LV_STATE_DISABLED);
        lv_obj_add_state(ui_btnRLBack, LV_STATE_DISABLED);
    }

    // Finish transfer
    if ((state == STATE_TYPE::S_STOP) && (oldState == STATE_TYPE::S_TRANSFER))
    {
        // Clear transfer progress
        lv_bar_set_value(ui_barTransferProgress, 0, LV_ANIM_ON);
        lv_obj_add_flag(ui_barTransferProgress, LV_OBJ_FLAG_HIDDEN);

        // Update item transfer state
        for (auto& rowInfo : listRowInfoSelected)
        {
            // Find if the latest transfer state is exist
            auto find = std::find_if(listTransferState.begin(), listTransferState.end(),
                [rowInfo](const std::pair<std::string, bool>& item) {
                    return rowInfo.id == item.first;
                });

            // Update transfer state
            if (find != listTransferState.end())
            {
                find->second = rowInfo.isTransfered;
            }
            // Add new transfer state
            else
            {
                listTransferState.push_back({ rowInfo.id, rowInfo.isTransfered });
            }

            rowInfo.UpdateTransferState();
        }
    }
}

void VideoRecordListScreen::UpdateTransferProgress()
{
    int percent = system_data::TransferPercent.GetValue();
    int value = copyItemCount * STANDARD_PERCENT + percent;

    lv_bar_set_value(ui_barTransferProgress, value, LV_ANIM_ON);
}

void VideoRecordListScreen::UpdateExtDevice()
{
    listExtDrive = storage_lib::GetExternalDrivesList();

    if (listExtDrive.size())
    {
        lv_dropdown_set_options(ui_dropPortableMemory, common_lib::JoinString("\n", storage_lib::GetExternalLabelList()).c_str());
    }
    else
    {
        lv_dropdown_set_options(ui_dropPortableMemory, "");
    }

    UpdateButton();

    system_data::DeviceChange.SetValue(false);
}
