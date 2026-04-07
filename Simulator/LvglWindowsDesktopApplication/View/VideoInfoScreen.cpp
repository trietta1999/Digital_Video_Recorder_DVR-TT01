#include "ui.h"
#include "CommonData.h"
#include "CommonLibrary.h"
#include "ScreenMapping.h"
#include "VideoInfoScreen.h"

VideoInfoScreen::VideoInfoScreen(SCREEN_NAME screen) : BaseScreen(screen)
{
    ListButtonCallback = {
        { ui_btnVideoInputCancel, OnClickCancel   , LV_EVENT_CLICKED       },
        { ui_btnVideoInputOK    , OnClickOK       , LV_EVENT_CLICKED       },
        { ui_btnVideoEvent      , OnClickInput    , LV_EVENT_SHORT_CLICKED },
        { ui_btnVideoName       , OnClickInput    , LV_EVENT_SHORT_CLICKED },
        { ui_btnVideoCategory   , OnClickInput    , LV_EVENT_SHORT_CLICKED },
        { ui_btnVideoDesc       , OnClickInput    , LV_EVENT_SHORT_CLICKED },
        { ui_btnVideoAuthor     , OnClickInput    , LV_EVENT_SHORT_CLICKED },
        { ui_btnVideoEvent      , OnLongPressInput, LV_EVENT_LONG_PRESSED  },
        { ui_btnVideoName       , OnLongPressInput, LV_EVENT_LONG_PRESSED  },
        { ui_btnVideoCategory   , OnLongPressInput, LV_EVENT_LONG_PRESSED  },
        { ui_btnVideoDesc       , OnLongPressInput, LV_EVENT_LONG_PRESSED  },
        { ui_btnVideoAuthor     , OnLongPressInput, LV_EVENT_LONG_PRESSED  },
    };

    ListDataUpdateCallback = {
        { []() { return temp_data::VideoEvent.GetState();    }, UpdateVideoInfo },
        { []() { return temp_data::VideoName.GetState();     }, UpdateVideoInfo },
        { []() { return temp_data::VideoCategory.GetState(); }, UpdateVideoInfo },
        { []() { return temp_data::VideoDesc.GetState();     }, UpdateVideoInfo },
        { []() { return temp_data::VideoAuthor.GetState();   }, UpdateVideoInfo },
    };
}

void VideoInfoScreen::VideoInfoScreenInit()
{
    COPY_CDATA_1TO2(input_data::VideoEvent, temp_data::VideoEvent);
    COPY_CDATA_1TO2(input_data::VideoName, temp_data::VideoName);
    COPY_CDATA_1TO2(input_data::VideoCategory, temp_data::VideoCategory);
    COPY_CDATA_1TO2(input_data::VideoDesc, temp_data::VideoDesc);
    COPY_CDATA_1TO2(input_data::VideoAuthor, temp_data::VideoAuthor);
}

void VideoInfoScreen::OnClickCancel(lv_event_t* event)
{
    ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::SCREEN_MAIN);
}

void VideoInfoScreen::OnClickOK(lv_event_t* event)
{
    COPY_CDATA_1TO2(temp_data::VideoEvent, input_data::VideoEvent);
    COPY_CDATA_1TO2(temp_data::VideoName, input_data::VideoName);
    COPY_CDATA_1TO2(temp_data::VideoCategory, input_data::VideoCategory);
    COPY_CDATA_1TO2(temp_data::VideoDesc, input_data::VideoDesc);
    COPY_CDATA_1TO2(temp_data::VideoAuthor, input_data::VideoAuthor);

    system_data::IsTempVideoInfo.SetValue(true);

    videoinfo_lib::CreateCurrentInfoFromInput();

    videoinfo_lib::videoinfo_t info = {
        current_videoinfo_data::VideoID.GetValue(),
        current_videoinfo_data::VideoEvent.GetValue(),
        current_videoinfo_data::VideoName.GetValue(),
        current_videoinfo_data::VideoCategory.GetValue(),
        current_videoinfo_data::VideoDesc.GetValue(),
        current_videoinfo_data::VideoAuthor.GetValue(),
        {
            current_videoinfo_data::VideoYear.GetValue(),
            current_videoinfo_data::VideoMonth.GetValue(),
            0,
            current_videoinfo_data::VideoDay.GetValue(),
            current_videoinfo_data::VideoHour.GetValue(),
            current_videoinfo_data::VideoMinute.GetValue(),
            current_videoinfo_data::VideoSecond.GetValue(),
            0
        }
    };

    videoinfo_lib::CreateTempData(info);

    ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::SCREEN_MAIN);
}

void VideoInfoScreen::OnClickInput(lv_event_t* event)
{
    if (event->current_target == ui_btnVideoEvent)
    {
        ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::KBSCREEN_VIDEO_EVENT);
    }
    else if (event->current_target == ui_btnVideoName)
    {
        ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::KBSCREEN_VIDEO_NAME);
    }
    else if (event->current_target == ui_btnVideoCategory)
    {
        ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::KBSCREEN_VIDEO_CATEGORY);
    }
    else if (event->current_target == ui_btnVideoDesc)
    {
        ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::KBSCREEN_VIDEO_DESC);
    }
    else if (event->current_target == ui_btnVideoAuthor)
    {
        ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::KBSCREEN_VIDEO_AUTHOR);
    }
}

void VideoInfoScreen::OnLongPressInput(lv_event_t* event)
{

}

void VideoInfoScreen::UpdateVideoInfo()
{
    if (temp_data::VideoEvent.GetState())
    {
        lv_label_set_text(ui_lblVideoEvent, temp_data::VideoEvent.GetValue().c_str());
    }

    if (temp_data::VideoName.GetState())
    {
        lv_label_set_text(ui_lblVideoName, temp_data::VideoName.GetValue().c_str());
    }

    if (temp_data::VideoCategory.GetState())
    {
        lv_label_set_text(ui_lblVideoCategory, temp_data::VideoCategory.GetValue().c_str());
    }

    if (temp_data::VideoDesc.GetState())
    {
        lv_label_set_text(ui_lblVideoDesc, temp_data::VideoDesc.GetValue().c_str());
    }

    if (temp_data::VideoAuthor.GetState())
    {
        lv_label_set_text(ui_lblVideoAuthor, temp_data::VideoAuthor.GetValue().c_str());
    }

    videoinfo_lib::videoinfo_t info = {
        "",
        temp_data::VideoEvent.GetValue(),
        temp_data::VideoName.GetValue(),
        temp_data::VideoCategory.GetValue(),
        temp_data::VideoDesc.GetValue(),
        temp_data::VideoAuthor.GetValue(),
    };

    if (info.IsAnyNoneForInput())
    {
        lv_obj_add_state(ui_btnVideoInputOK, LV_STATE_DISABLED);
    }
    else
    {
        lv_obj_remove_state(ui_btnVideoInputOK, LV_STATE_DISABLED);
    }
}
