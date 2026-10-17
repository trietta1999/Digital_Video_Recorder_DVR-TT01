#include <fstream>
#include <shlwapi.h>
#include "CommonData.h"
#include "CommonLibrary.h"
#include "VideoInfoData.h"

namespace recordlist_lib
{
    // Parses the INI file to find all sections representing a video ID (format: [{GUID}])
    static std::vector<std::string> GetAllVideoIDSection()
    {
        std::vector<std::string> results;
        std::string line; // Buffer for line-by-line reading
        std::ifstream file(common_lib::ConvertWStringToString(common_lib::GetFullPath(VIDEOINFO_DATA)));

        if (!file.is_open())
        {
            return results;
        }

        // Scan file for [GUID] style tags manually as Win32 API doesn't support enumerating sections easily
        while (std::getline(file, line)) {
            auto startPos = line.find("[{");
            auto endPos = line.find("}]");

            if ((startPos != std::string::npos) && (endPos != std::string::npos) && (endPos > startPos))
            {
                std::string tag = line.substr(startPos + 1, (endPos - startPos)); // Extract GUID
                results.push_back(tag);
            }
        }

        return results;
    }

    // Aggregates all persistent video metadata into a list
    std::vector<videoinfo_lib::videoinfo_t> GetVideoInfoListData()
    {
        std::vector<videoinfo_lib::videoinfo_t> list; // Target collection
        auto results = GetAllVideoIDSection(); // Get list of IDs

        for (const auto& id : results)
        {
            list.push_back(videoinfo_lib::GetExistData(id)); // Hydrate each ID into a struct
        }

        return list;
    }

    // Copies a specific video file to a removable drive with progress reporting
    bool ExportVideoToExternalDrive(std::string id, std::string name, std::string driveLetter)
    {
        wchar_t inputPath[MAX_CHARS] = { 0 }; // Source file path
        wchar_t outputPath[MAX_CHARS] = { 0 }; // Target file path on USB
        
        // Construct source path from internal library storage
        swprintf_s(inputPath, L"%s\\video_data\\%s\\video.mp4", common_lib::GetSystemPath().c_str(), common_lib::ConvertStringToWString(id).c_str());
        
        // Construct output filename using the provided name on the root of the target drive
        swprintf_s(outputPath, L"%s%s.mp4", common_lib::ConvertStringToWString(driveLetter).c_str(), common_lib::ConvertStringToWString(name).c_str());

        return storage_lib::CopyFileWithProgress(inputPath, outputPath);
    }

    // Transmits a video package to a network peer using the Dukto protocol via an embedded Python tool
    bool SendVideoToNetwork(std::string id, std::string name)
    {
        char inputPath[MAX_CHARS] = { 0 }; // Base directory of the video
        char command1[MAX_CHARS * 2] = { 0 }; // Command to create identification file
        char command2[MAX_CHARS * 2] = { 0 }; // Command to execute network transfer

        // Prepare paths and commands
        sprintf_s(inputPath, "%s\\video_data\\%s", common_lib::ConvertWStringToString(common_lib::GetSystemPath()).c_str(), id.c_str());
        
        // Step 1: Create a dummy file with the video name so the receiver knows the title
        sprintf_s(command1, "copy nul \"%s\\%s\"", inputPath, name.c_str());
        
        // Step 2: Use embedded python to run 'ndrop' (Dukto implementation) to send the folder content
        sprintf_s(command2, "%s\\python.exe -m ndrop --mode dukto --send %s \"%s\"", common_lib::ConvertWStringToString(common_lib::GetFullPath(L"python-3.7.9-embed-amd64")).c_str(), temp_data::DuktoIP.GetValue().c_str(), inputPath);

        ::system(command1); // Execution via shell
        ::system(command2); // Execution via shell

        return true;
    }
}
