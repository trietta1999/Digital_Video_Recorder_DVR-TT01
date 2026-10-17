#include <shlwapi.h>
#include "CommonData.h"
#include "CommonLibrary.h"
#include "VideoInfoData.h"

namespace videoinfo_lib
{
    // Helper to write video metadata to a configuration file under a specific section
    static void CreateData(std::wstring section, const videoinfo_t& info)
    {
        // Write standard metadata strings
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_ID, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoID).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_EVENT, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoEvent).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_NAME, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoName).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_CATEGORY, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoCategory).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_DESC, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoDesc).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_AUTHOR, VIDEOINFO_DATA, common_lib::ConvertStringToWString(info.videoAuthor).c_str());

        // Write timestamp components
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_DAY, VIDEOINFO_DATA, std::to_wstring(info.datetime.wDay).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_MONTH, VIDEOINFO_DATA, std::to_wstring(info.datetime.wMonth).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_YEAR, VIDEOINFO_DATA, std::to_wstring(info.datetime.wYear).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_HOUR, VIDEOINFO_DATA, std::to_wstring(info.datetime.wHour).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_MINUTE, VIDEOINFO_DATA, std::to_wstring(info.datetime.wMinute).c_str());
        config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_SECOND, VIDEOINFO_DATA, std::to_wstring(info.datetime.wSecond).c_str());

        // For non-temporary entries, define the relative file path for the video content
        if (section != VIF_TEMP_SECTION)
        {
            config_lib::WriteWStringConfig(section.c_str(), VIF_VIDEO_PATH, VIDEOINFO_DATA, common_lib::ConvertStringToWString("video_data\\" + info.videoID).c_str());
        }

        // Ensure the directory for this video's data exists
        wchar_t path[MAX_CHARS] = { 0 };
        ::PathAppend(path, common_lib::GetSystemPath().c_str());
        ::PathAppend(path, VIDEOINFO_DATA_FOLDER);
        ::PathAppend(path, common_lib::ConvertStringToWString(info.videoID).c_str());
        ::CreateDirectory(path, NULL);
    }

    // Helper to retrieve and parse video metadata from a configuration file section
    static videoinfo_t GetData(std::wstring section)
    {
        // Extract strings
        auto id = common_lib::ConvertWStringToString(config_lib::GetWStringConfig(section.c_str(), VIF_VIDEO_ID, VIDEOINFO_DATA));
        auto event = common_lib::ConvertWStringToString(config_lib::GetWStringConfig(section.c_str(), VIF_VIDEO_EVENT, VIDEOINFO_DATA));
        auto name = common_lib::ConvertWStringToString(config_lib::GetWStringConfig(section.c_str(), VIF_VIDEO_NAME, VIDEOINFO_DATA));
        auto category = common_lib::ConvertWStringToString(config_lib::GetWStringConfig(section.c_str(), VIF_VIDEO_CATEGORY, VIDEOINFO_DATA));
        auto desc = common_lib::ConvertWStringToString(config_lib::GetWStringConfig(section.c_str(), VIF_VIDEO_DESC, VIDEOINFO_DATA));
        auto author = common_lib::ConvertWStringToString(config_lib::GetWStringConfig(section.c_str(), VIF_VIDEO_AUTHOR, VIDEOINFO_DATA));

        // Extract numeric timestamp parts
        auto day = config_lib::GetUShortConfig(section.c_str(), VIF_VIDEO_DAY, VIDEOINFO_DATA);
        auto month = config_lib::GetUShortConfig(section.c_str(), VIF_VIDEO_MONTH, VIDEOINFO_DATA);
        auto year = config_lib::GetUShortConfig(section.c_str(), VIF_VIDEO_YEAR, VIDEOINFO_DATA);
        auto hour = config_lib::GetUShortConfig(section.c_str(), VIF_VIDEO_HOUR, VIDEOINFO_DATA);
        auto minute = config_lib::GetUShortConfig(section.c_str(), VIF_VIDEO_MINUTE, VIDEOINFO_DATA);
        auto second = config_lib::GetUShortConfig(section.c_str(), VIF_VIDEO_SECOND, VIDEOINFO_DATA);

        // Construct and return the info structure
        videoinfo_t info = {
            id, event, name, category, desc, author,
            { year, month, 0, day, hour, minute, second, 0 }
        };

        return info;
    }

    // Stores metadata in a designated temporary section (usually for draft/staging)
    void CreateTempData(const videoinfo_t& info)
    {
        CreateData(VIF_TEMP_SECTION, info);
    }

    // Stores metadata under a unique persistent section ID
    void CreateNewData(std::string id, const videoinfo_t& info)
    {
        CreateData(common_lib::ConvertStringToWString(id), info);
    }

    // Resets the temporary metadata section by overwriting with empty data
    void ClearTempData()
    {
        videoinfo_t info;
        CreateData(VIF_TEMP_SECTION, info);
    }

    // Removes video metadata and deletes the associated physical data directory
    void DeleteData(std::string id)
    {
        // Delete section from INI
        config_lib::WriteWStringConfig(common_lib::ConvertStringToWString(id).c_str(), L"", VIDEOINFO_DATA, L"");

        // Prepare directory path for deletion
        wchar_t path[MAX_CHARS] = { 0 };

        ::PathAppend(path, common_lib::GetSystemPath().c_str());
        ::PathAppend(path, VIDEOINFO_DATA_FOLDER);
        ::PathAppend(path, common_lib::ConvertStringToWString(id).c_str());

        // Use shell operation to delete folder and contents quietly
        SHFILEOPSTRUCTW fileOp = { 0 };
        fileOp.wFunc = FO_DELETE;
        fileOp.pFrom = path;
        fileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;

        ::SHFileOperation(&fileOp);
    }

    // Retrieves metadata from the temporary section
    videoinfo_t GetTempData()
    {
        return GetData(VIF_TEMP_SECTION);
    }

    // Retrieves metadata for a specific existing video ID
    videoinfo_t GetExistData(std::string id)
    {
        return GetData(common_lib::ConvertStringToWString(id));
    }

    // Transfers buffered UI input data to the active video metadata context
    void CreateCurrentInfoFromInput()
    {
        if (current_videoinfo_data::VideoID.GetValue().empty())
        {
            current_videoinfo_data::VideoID.SetValue(common_lib::GenerateGUID());
        }

        COPY_CDATA_1TO2(input_data::VideoEvent, current_videoinfo_data::VideoEvent);
        COPY_CDATA_1TO2(input_data::VideoName, current_videoinfo_data::VideoName);
        COPY_CDATA_1TO2(input_data::VideoCategory, current_videoinfo_data::VideoCategory);
        COPY_CDATA_1TO2(input_data::VideoDesc, current_videoinfo_data::VideoDesc);
        COPY_CDATA_1TO2(input_data::VideoAuthor, current_videoinfo_data::VideoAuthor);

        // Reset timestamp for new record
        current_videoinfo_data::VideoDay.SetValue(0);
        current_videoinfo_data::VideoMonth.SetValue(0);
        current_videoinfo_data::VideoYear.SetValue(0);
        current_videoinfo_data::VideoHour.SetValue(0);
        current_videoinfo_data::VideoMinute.SetValue(0);
        current_videoinfo_data::VideoSecond.SetValue(0);
    }

    // Populates the active video context from an existing metadata structure
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

    // Stamps the active video metadata context with the current system time
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

    // Verifies if the file path record for the current video exists in config
    bool CheckCurrentVideoInfoPathExist()
    {
        auto value = config_lib::GetWStringConfig(common_lib::ConvertStringToWString(current_videoinfo_data::VideoID.GetValue()).c_str(), VIF_VIDEO_PATH, VIDEOINFO_DATA);

        return (!value.empty()) ? true : false;
    }
}
