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
            {
                DROPDOWNLIST_NAME::DD_DATE_FORMAT,
                {
                    ui_dropDateFormat,
                    {
                        "DD MM YYYY",
                        "MM DD YYYY",
                        "YYYY MM DD",
                    },
                    &temp_data::DateFormat,
                },
            },
            {
                DROPDOWNLIST_NAME::DD_DATE_SEPARATOR,
                {
                    ui_dropDateSeparator,
                    {
                        ".",
                        "/",
                        "-",
                        "Space",
                    },
                    &temp_data::DateSeparator,
                },
            },
            {
                DROPDOWNLIST_NAME::DD_KEYBOARD_TYPE,
                {
                    ui_dropKeyboardType,
                    {
                        "Internal numpad",
                        "Standard numpad",
                        "Standard keyboard",
                    },
                    &temp_data::KeyboardType,
                },
            },
            {
                DROPDOWNLIST_NAME::DD_T9_AUTO_CONFIRM_SPEED,
                {
                    ui_dropT9AutoConfirm,
                    {
                        "Slow",
                        "Normal",
                        "Fast",
                    },
                    &temp_data::T9AutoConfirmSpeed,
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
                if (tempInfo.data->GetValue() == i)
                {
                    lv_dropdown_set_selected(tempInfo.dropdownObj, i);
                    break;
                }
            }
        }
    }

    void UpdateDropdownList(std::vector<DROPDOWNLIST_NAME> listName)
    {
        for (auto& info : mapDropdownInfo)
        {
            if (std::find(listName.begin(), listName.end(), info.first) != listName.end())
            {
                auto& tempInfo = info.second;

                lv_dropdown_set_options(tempInfo.dropdownObj, common_lib::JoinString("\n", tempInfo.options).c_str());

                for (int i = 0; i < tempInfo.options.size(); i++)
                {
                    if (tempInfo.data->GetValue() == i)
                    {
                        lv_dropdown_set_selected(tempInfo.dropdownObj, i);
                        break;
                    }
                }
            }
        }
    }

    const dropdown_info_t& GetDropdownInfo(DROPDOWNLIST_NAME name)
    {
        return mapDropdownInfo[name];
    }

    short GetDropdownSelectedIndex(DROPDOWNLIST_NAME name)
    {
        return lv_dropdown_get_selected(mapDropdownInfo[name].dropdownObj);
    }

    void UpdateDropdownData(lv_obj_t* obj)
    {
        for (auto& info : mapDropdownInfo)
        {
            if (info.second.dropdownObj == obj)
            {
                info.second.data->SetValue(GetDropdownSelectedIndex(info.first));
                break;
            }
        }
    }
}
