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
}
