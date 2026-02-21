#include "ui.h"
#include "CommonData.h"
#include "ScreenMapping.h"
#include "VideoInfoScreen.h"

std::vector<std::pair<lv_obj_t*, SCREEN_NAME>> VideoInfoScreen::listKbScreenName = {};

VideoInfoScreen::VideoInfoScreen(SCREEN_NAME screen) : BaseScreen(screen)
{
    ListButtonCallback = {
        { ui_btnVideoInputCancel, OnClickCancel   , LV_EVENT_CLICKED       },
        { ui_btnVideoInputOK    , OnClickOK       , LV_EVENT_CLICKED       },
        { ui_btnVideoId         , OnClickInput    , LV_EVENT_SHORT_CLICKED },
        { ui_btnVideoName       , OnClickInput    , LV_EVENT_SHORT_CLICKED },
        { ui_btnVideoInputDesc  , OnClickInput    , LV_EVENT_SHORT_CLICKED },
        { ui_btnVideoId         , OnLongPressInput, LV_EVENT_LONG_PRESSED  },
        { ui_btnVideoName       , OnLongPressInput, LV_EVENT_LONG_PRESSED  },
        { ui_btnVideoInputDesc  , OnLongPressInput, LV_EVENT_LONG_PRESSED  },
    };

    ListDataUpdateCallback = {
        { []() { return temp_data::VideoID.GetState(); }  , UpdateVideoInfo },
        { []() { return temp_data::VideoName.GetState(); }, UpdateVideoInfo },
        { []() { return temp_data::VideoDesc.GetState(); }, UpdateVideoInfo },
    };

    listKbScreenName = {
        { ui_btnVideoId       , SCREEN_NAME::VIDEO_ID_KBSCREEN   },
        { ui_btnVideoName     , SCREEN_NAME::VIDEO_NAME_KBSCREEN },
        { ui_btnVideoInputDesc, SCREEN_NAME::VIDEO_DESC_KBSCREEN },
    };

    temp_data::VideoID.SetValue("aaaaaaaaaaaaa"); //@todo: wait setting
}

void VideoInfoScreen::OnClickCancel(lv_obj_t* obj)
{
    ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::MAIN_SCREEN);
}

void VideoInfoScreen::OnClickOK(lv_obj_t* obj)
{
    ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::MAIN_SCREEN);
}

void VideoInfoScreen::OnClickInput(lv_obj_t* obj)
{
    for (const auto& item : listKbScreenName)
    {
        if (item.first == obj)
        {
            ScreenMapping::GetInstance().ChangeScreen(item.second);
            break;
        }
    }
}

void VideoInfoScreen::OnLongPressInput(lv_obj_t* obj)
{
    //ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::VIDEO_NAME_KBSCREEN);
}

void VideoInfoScreen::UpdateVideoInfo()
{
    if (temp_data::VideoID.GetState())
    {
        lv_label_set_text(ui_lblVideoId, temp_data::VideoID.GetValue().c_str());
    }
    if (temp_data::VideoName.GetState())
    {
        lv_label_set_text(ui_lblVideoName, temp_data::VideoName.GetValue().c_str());
    }
    if (temp_data::VideoDesc.GetState())
    {
        lv_label_set_text(ui_lblVideoInputDesc, temp_data::VideoDesc.GetValue().c_str());
    }
}
