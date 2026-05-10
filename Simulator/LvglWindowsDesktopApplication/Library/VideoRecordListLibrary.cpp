#include <fstream>
#include <shlwapi.h>
#include "CommonData.h"
#include "CommonLibrary.h"
#include "VideoInfoData.h"

namespace recordlist_lib
{
    static std::vector<std::string> GetAllVideoIDSection()
    {
        std::vector<std::string> results;
        std::string line;
        std::ifstream file(common_lib::ConvertWStringToString(common_lib::GetFullPath(VIDEOINFO_DATA)));

        if (!file.is_open())
        {
            return results;
        }

        while (std::getline(file, line)) {
            auto startPos = line.find("[{");
            auto endPos = line.find("}]");

            if ((startPos != std::string::npos) && (endPos != std::string::npos) && (endPos > startPos))
            {
                std::string tag = line.substr(startPos + 1, (endPos - startPos));
                results.push_back(tag);
            }
        }

        return results;
    }

    std::vector<videoinfo_lib::videoinfo_t> GetVideoInfoListData()
    {
        std::vector<videoinfo_lib::videoinfo_t> list;
        auto results = GetAllVideoIDSection();

        for (const auto& id : results)
        {
            list.push_back(videoinfo_lib::GetExistData(id));
        }

        return list;
    }

    bool ExportVideoToExternalDrive(std::string id, std::string name, std::string driveLetter)
    {
        wchar_t inputPath[MAX_CHARS] = { 0 };
        wchar_t outputPath[MAX_CHARS] = { 0 };
        swprintf_s(inputPath, L"%s\\video_data\\%s\\video.mp4", common_lib::GetSystemPath().c_str(), common_lib::ConvertStringToWString(id).c_str());
        swprintf_s(outputPath, L"%s%s.mp4", common_lib::ConvertStringToWString(driveLetter).c_str(), common_lib::ConvertStringToWString(name).c_str());

        return storage_lib::CopyFileWithProgress(inputPath, outputPath);
    }
}
