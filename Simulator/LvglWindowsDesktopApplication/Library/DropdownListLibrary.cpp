#include "ui.h"
#include "CommonData.h"
#include "CommonLibrary.h"

namespace dropdownlist_lib
{
    static std::unordered_map<DROPDOWNLIST_NAME, dropdown_info_t> mapDropdownInfo; // Internal registry of all managed dropdown widgets

    void SetupDropdownList()
    {
        // Initializes the registry with UI objects, option strings, and associated data pointers
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
            {
                DROPDOWNLIST_NAME::DD_NETWORK_TYPE,
                {
                    ui_dropNetworkType,
                    {},
                    &temp_data::NetworkType,
                },
            },
            {
                DROPDOWNLIST_NAME::DD_SSID,
                {
                    ui_dropSSID,
                    {},
                    &temp_data::SSID,
                },
            },
        };

        // Populate options and set initial selection for each registered dropdown
        for (auto& info : mapDropdownInfo)
        {
            auto& tempInfo = info.second;

            // Apply option list separated by newlines
            lv_dropdown_set_options(tempInfo.dropdownObj, common_lib::JoinString("\n", tempInfo.options).c_str());

            // Set the UI selection index based on current CData value
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
        // Selectively updates only the dropdowns specified in listName
        for (auto& info : mapDropdownInfo)
        {
            if (std::find(listName.begin(), listName.end(), info.first) != listName.end())
            {
                auto& tempInfo = info.second;

                // Refresh options list
                lv_dropdown_set_options(tempInfo.dropdownObj, common_lib::JoinString("\n", tempInfo.options).c_str());

                // Re-synchronize selected index from data
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

    dropdown_info_t& GetDropdownInfo(DROPDOWNLIST_NAME name)
    {
        // Returns the metadata structure for a specific dropdown
        return mapDropdownInfo[name];
    }

    short GetDropdownSelectedIndex(DROPDOWNLIST_NAME name)
    {
        // Helper to retrieve the current selection index from the UI object
        return lv_dropdown_get_selected(mapDropdownInfo[name].dropdownObj);
    }

    void UpdateDropdownData(lv_obj_t* obj)
    {
        // Synchronizes the CData value with the UI's current selection
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
