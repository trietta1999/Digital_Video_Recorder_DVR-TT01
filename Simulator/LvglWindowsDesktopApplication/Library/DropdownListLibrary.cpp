#include "ui.h"
#include "CommonData.h"
#include "CommonLibrary.h"

namespace dropdownlist_lib
{
    static std::unordered_map<DROPDOWNLIST_NAME, dropdown_info_t> mapDropdownInfo;

    void SetupDropdownList()
    {
        // Register dropdownlist info
        mapDropdownInfo = {
            {
                DROPDOWNLIST_NAME::DD_VIDEO_SEARCH,
                {
                    ui_dropVideoFilter,
                    {
                        "Event",
                        "Video name",
                        "Category",
                        "Description",
                        "Author",
                    },
                    &input_data::VideoSearchType,
                },
            },
        };

        // Init dropdown options
        for (auto& info : mapDropdownInfo)
        {
            auto& tempInfo = info.second;

            lv_dropdown_set_options(tempInfo.dropdownObj, common_lib::JoinString("\n", tempInfo.options).c_str());

            for (int i = 0; i < tempInfo.options.size(); i++)
            {
                if (tempInfo.options[i] == tempInfo.data->GetValue())
                {
                    lv_dropdown_set_selected(tempInfo.dropdownObj, i);
                    break;
                }
            }
        }
    }

    const dropdown_info_t& GetDropdownInfo(DROPDOWNLIST_NAME name)
    {
        return mapDropdownInfo[name];
    }

    std::string GetDropdownSelected(DROPDOWNLIST_NAME name)
    {
        char option[MAX_PATH] = { 0 };
        lv_dropdown_get_selected_str(mapDropdownInfo[name].dropdownObj, option, sizeof(option));

        return option;
    }

    void UpdateDropdownData(lv_obj_t* obj)
    {
        for (auto& info : mapDropdownInfo)
        {
            if (info.second.dropdownObj == obj)
            {
                info.second.data->SetValue(GetDropdownSelected(info.first));
                break;
            }
        }
    }
}
