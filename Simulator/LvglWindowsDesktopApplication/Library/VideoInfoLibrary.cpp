#include "CommonData.h"
#include "CommonLibrary.h"
#include "VideoInfoData.h"

namespace videoinfo_lib
{
    static void CreateData(std::wstring section, const videoinfo_t& info)
    {
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_ID, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoID).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_EVENT, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoEvent).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_NAME, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoName).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_CATEGORY, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoCategory).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_DESC, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoDesc).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_AUTHOR, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoAuthor).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_DAY, VIDEOINFO_DATA, std::to_wstring(info.datetime.wDay).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_MONTH, VIDEOINFO_DATA, std::to_wstring(info.datetime.wMonth).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_YEAR, VIDEOINFO_DATA, std::to_wstring(info.datetime.wYear).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_HOUR, VIDEOINFO_DATA, std::to_wstring(info.datetime.wHour).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_MINUTE, VIDEOINFO_DATA, std::to_wstring(info.datetime.wMinute).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_SECOND, VIDEOINFO_DATA, std::to_wstring(info.datetime.wSecond).c_str());

        if (section != VIF_TEMP_SECTION)
        {
            config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_PATH, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoID).c_str());
        }
    }

    static videoinfo_t GetData(std::wstring section)
    {
        auto id = common_lib::ConvertWStringToString(config_lib::GetWStringConfig(section.c_str(), VIF_VIDEO_ID, VIDEOINFO_DATA));
        auto event = common_lib::ConvertWStringToString(config_lib::GetWStringConfig(section.c_str(), VIF_VIDEO_EVENT, VIDEOINFO_DATA));
        auto name = common_lib::ConvertWStringToString(config_lib::GetWStringConfig(section.c_str(), VIF_VIDEO_NAME, VIDEOINFO_DATA));
        auto category = common_lib::ConvertWStringToString(config_lib::GetWStringConfig(section.c_str(), VIF_VIDEO_CATEGORY, VIDEOINFO_DATA));
        auto desc = common_lib::ConvertWStringToString(config_lib::GetWStringConfig(section.c_str(), VIF_VIDEO_DESC, VIDEOINFO_DATA));
        auto author = common_lib::ConvertWStringToString(config_lib::GetWStringConfig(section.c_str(), VIF_VIDEO_AUTHOR, VIDEOINFO_DATA));
        auto day = config_lib::GetUShortConfig(section.c_str(), VIF_VIDEO_DAY, VIDEOINFO_DATA);
        auto month = config_lib::GetUShortConfig(section.c_str(), VIF_VIDEO_MONTH, VIDEOINFO_DATA);
        auto year = config_lib::GetUShortConfig(section.c_str(), VIF_VIDEO_YEAR, VIDEOINFO_DATA);
        auto hour = config_lib::GetUShortConfig(section.c_str(), VIF_VIDEO_HOUR, VIDEOINFO_DATA);
        auto minute = config_lib::GetUShortConfig(section.c_str(), VIF_VIDEO_MINUTE, VIDEOINFO_DATA);
        auto second = config_lib::GetUShortConfig(section.c_str(), VIF_VIDEO_SECOND, VIDEOINFO_DATA);

        videoinfo_t info = {
            id, event, name, category, desc, author,
            { year, month, 0, day, hour, minute, second, 0 }
        };

        return info;
    }

    void CreateTempData(const videoinfo_t& info)
    {
        CreateData(VIF_TEMP_SECTION, info);
    }

    void CreateNewData(std::string id, const videoinfo_t& info)
    {
        CreateData(common_lib::ConvertStringToWString(id), info);
    }

    void ClearTempData()
    {
        config_lib::WriteWStringConfig(VIF_TEMP_SECTION, L"", VIDEOINFO_DATA, L"");
    }

    videoinfo_t GetTempData()
    {
        return GetData(VIF_TEMP_SECTION);
    }

    void CreateCurrentInfoFromInput()
    {
        current_videoinfo_data::VideoID.SetValue(common_lib::GenerateGUID());
        COPY_CDATA_1TO2(input_data::VideoEvent, current_videoinfo_data::VideoEvent);
        COPY_CDATA_1TO2(input_data::VideoName, current_videoinfo_data::VideoName);
        COPY_CDATA_1TO2(input_data::VideoCategory, current_videoinfo_data::VideoCategory);
        COPY_CDATA_1TO2(input_data::VideoDesc, current_videoinfo_data::VideoDesc);
        COPY_CDATA_1TO2(input_data::VideoAuthor, current_videoinfo_data::VideoAuthor);
        current_videoinfo_data::VideoDay.SetValue(0);
        current_videoinfo_data::VideoMonth.SetValue(0);
        current_videoinfo_data::VideoYear.SetValue(0);
        current_videoinfo_data::VideoHour.SetValue(0);
        current_videoinfo_data::VideoMinute.SetValue(0);
        current_videoinfo_data::VideoSecond.SetValue(0);
    }

    void CreateCurrentInfoFromInfo(const videoinfo_t& info)
    {
        current_videoinfo_data::VideoID.SetValue(info.videoID);
        current_videoinfo_data::VideoEvent.SetValue(info.videoEvent);
        current_videoinfo_data::VideoName.SetValue(info.videoName);
        current_videoinfo_data::VideoCategory.SetValue(info.videoCategory);
        current_videoinfo_data::VideoDesc.SetValue(info.videoDesc);
        current_videoinfo_data::VideoAuthor.SetValue(info.videoAuthor);
        current_videoinfo_data::VideoDay.SetValue(info.datetime.wDay);
        current_videoinfo_data::VideoMonth.SetValue(info.datetime.wMonth);
        current_videoinfo_data::VideoYear.SetValue(info.datetime.wYear);
        current_videoinfo_data::VideoHour.SetValue(info.datetime.wHour);
        current_videoinfo_data::VideoMinute.SetValue(info.datetime.wMinute);
        current_videoinfo_data::VideoSecond.SetValue(info.datetime.wSecond);
    }

    void UpdateCurrentInfoDateTime()
    {
        auto systime = common_lib::GetSystemDateTime();

        current_videoinfo_data::VideoDay.SetValue(systime.wDay);
        current_videoinfo_data::VideoMonth.SetValue(systime.wMonth);
        current_videoinfo_data::VideoYear.SetValue(systime.wYear);
        current_videoinfo_data::VideoHour.SetValue(systime.wHour);
        current_videoinfo_data::VideoMinute.SetValue(systime.wMinute);
        current_videoinfo_data::VideoSecond.SetValue(systime.wSecond);
    }

    bool CheckCurrentVideoInfoPathExist()
    {
        auto value = config_lib::GetWStringConfig(common_lib::ConvertStringToWString(current_videoinfo_data::VideoID.GetValue()).c_str(), VIF_VIDEO_PATH, VIDEOINFO_DATA);

        return (!value.empty()) ? true : false;
    }
}
