#include "CommonData.h"
#include "CommonLibrary.h"
#include "VideoInfoData.h"

namespace videoinfo_lib
{
    void WriteToTempData(const videoinfo_t& info)
    {
        config_lib::WriteWStringConfig(VIF_TEMP_SECTION, VIF_VIDEO_ID, VIDEOINFO_DATA, common_lib::ConvertStringToWString(system_data::TempVideoID.GetValue()).c_str());
        config_lib::WriteWStringConfig(VIF_TEMP_SECTION, VIF_VIDEO_EVENT, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoEvent).c_str());
        config_lib::WriteWStringConfig(VIF_TEMP_SECTION, VIF_VIDEO_NAME, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoName).c_str());
        config_lib::WriteWStringConfig(VIF_TEMP_SECTION, VIF_VIDEO_CATEGORY, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoCategory).c_str());
        config_lib::WriteWStringConfig(VIF_TEMP_SECTION, VIF_VIDEO_DESC, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoDesc).c_str());
        config_lib::WriteWStringConfig(VIF_TEMP_SECTION, VIF_VIDEO_AUTHOR, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoAuthor).c_str());
    }

    std::string GetTempIDFromTempData()
    {
        return common_lib::ConvertWStringToString(config_lib::GetWStringConfig(VIF_TEMP_SECTION, VIF_VIDEO_ID, VIDEOINFO_DATA));
    }

    videoinfo_t GetTempFromData()
    {
        videoinfo_t info = {
            common_lib::ConvertWStringToString(config_lib::GetWStringConfig(VIF_TEMP_SECTION, VIF_VIDEO_EVENT, VIDEOINFO_DATA)),
            common_lib::ConvertWStringToString(config_lib::GetWStringConfig(VIF_TEMP_SECTION, VIF_VIDEO_NAME, VIDEOINFO_DATA)),
            common_lib::ConvertWStringToString(config_lib::GetWStringConfig(VIF_TEMP_SECTION, VIF_VIDEO_CATEGORY, VIDEOINFO_DATA)),
            common_lib::ConvertWStringToString(config_lib::GetWStringConfig(VIF_TEMP_SECTION, VIF_VIDEO_DESC, VIDEOINFO_DATA)),
            common_lib::ConvertWStringToString(config_lib::GetWStringConfig(VIF_TEMP_SECTION, VIF_VIDEO_AUTHOR, VIDEOINFO_DATA)),
        };

        return info;
    }

    void WriteToNewData(const videoinfo_t& info)
    {
        config_lib::WriteWStringConfig(common_lib::ConvertStringToWString(system_data::TempVideoID.GetValue()), VIF_VIDEO_EVENT, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoEvent).c_str());
        config_lib::WriteWStringConfig(common_lib::ConvertStringToWString(system_data::TempVideoID.GetValue()), VIF_VIDEO_NAME, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoName).c_str());
        config_lib::WriteWStringConfig(common_lib::ConvertStringToWString(system_data::TempVideoID.GetValue()), VIF_VIDEO_CATEGORY, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoCategory).c_str());
        config_lib::WriteWStringConfig(common_lib::ConvertStringToWString(system_data::TempVideoID.GetValue()), VIF_VIDEO_DESC, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoDesc).c_str());
        config_lib::WriteWStringConfig(common_lib::ConvertStringToWString(system_data::TempVideoID.GetValue()), VIF_VIDEO_AUTHOR, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoAuthor).c_str());
    }
}
