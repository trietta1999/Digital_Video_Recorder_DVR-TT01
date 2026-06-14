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

    bool SendVideoToNetwork(std::string id, std::string name)
    {
        char inputPath[MAX_CHARS] = { 0 };
        char command1[MAX_CHARS * 2] = { 0 };
        char command2[MAX_CHARS * 2] = { 0 };

        sprintf_s(inputPath, "%s\\video_data\\%s", common_lib::ConvertWStringToString(common_lib::GetSystemPath()).c_str(), id.c_str());
        sprintf_s(command1, "copy nul \"%s\\%s\"", inputPath, name.c_str());
        sprintf_s(command2, "%s\\python.exe -m ndrop --mode dukto --send %s \"%s\"", common_lib::ConvertWStringToString(common_lib::GetFullPath(L"python-3.7.9-embed-amd64")).c_str(), temp_data::DuktoIP.GetValue().c_str(), inputPath);

        ::system(command1); // Create video name empty file
        ::system(command2); // Send to network

        return true;
    }
}
