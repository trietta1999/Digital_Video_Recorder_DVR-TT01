#include "CommonData.h"
#include "CommonLibrary.h"
#include "SystemConfig.h"

namespace systemsetting_lib
{
    // Mapping of application settings to their configuration keys and default values
    static std::vector<setting_info_t> listSettingInfo = {
        { &temp_data::DateFormat               , &setting_data::DateFormat               , L"DateFormat"               , 0 },
        { &temp_data::DateSeparator            , &setting_data::DateSeparator            , L"DateSeparator"            , 0 },
        { &temp_data::InitAudState             , &setting_data::InitAudState             , L"InitAudState"             , 0 },
        { &temp_data::PreviewVideoState        , &setting_data::PreviewVideoState        , L"PreviewVideoState"        , 0 },
        { &temp_data::PlayVideoState           , &setting_data::PlayVideoState           , L"PlayVideoState"           , 0 },
        { &temp_data::KeyboardType             , &setting_data::KeyboardType             , L"KeyboardType"             , 0 },
        { &temp_data::T9AutoConfirmSpeed       , &setting_data::T9AutoConfirmSpeed       , L"T9AutoConfirmSpeed"       , 0 },
        { &temp_data::KeyboardExitShortcutState, &setting_data::KeyboardExitShortcutState, L"KeyboardExitShortcutState", 0 },
        { &temp_data::InsSpaceAfterPuncState   , &setting_data::InsSpaceAfterPuncState   , L"InsSpaceAfterPuncState"   , 0 },
        { &temp_data::AutoCloseBracketState    , &setting_data::AutoCloseBracketState    , L"AutoCloseBracketState"    , 0 },
        { &temp_data::NetworkType              , &setting_data::NetworkType              , L"NetworkType"              , 0 },
        { &temp_data::DuktoIPPart12            , &setting_data::DuktoIPPart12            , L"DuktoIPPart12"            , 0 },
        { &temp_data::DuktoIPPart34            , &setting_data::DuktoIPPart34            , L"DuktoIPPart34"            , 0 },
    };

    void SetupSystemData()
    {
        // Reserved for future system-level data initialization
    }

    void SetupSetting()
    {
        // Loads all settings from the persistent configuration file into memory
        for (auto& info : listSettingInfo)
        {
            auto value = config_lib::GetUShortConfig(SYSTEM_SECTION, info.configName, common_lib::GetFullPath(SYSTEM_CONFIG));
            info.settingData->SetValue(value); // Set persistent storage
            info.tempData->SetValue(value); // Set temporary staging buffer
            info.configValue = value; // Update default reference
        }

        // Initialize the formatted Dukto IP string from the individual octet pairs
        temp_data::DuktoIP.SetValue(network_lib::CalculateIPStrFromCombinedNum(
            setting_data::DuktoIPPart12.GetValue(),
            setting_data::DuktoIPPart34.GetValue()
        ));
    }

    void SetupTempSetting()
    {
        // Synchronizes temporary buffers with persistent data, typically called before entering a settings screen
        for (auto& info : listSettingInfo)
        {
            info.tempData->SetValue(0); // Reset to ensure state change logic triggers if needed
            info.tempData->SetValue(info.settingData->GetValue());
        }
    }

    void SaveSetting()
    {
        // Commits temporary staged changes to persistent memory and writes to the filesystem
        for (auto& info : listSettingInfo)
        {
            auto value = info.tempData->GetValue();

            info.settingData->SetValue(0); // Reset to ensure state change triggers
            info.settingData->SetValue(value);
            info.configValue = value;

            // Update the INI configuration file on disk
            config_lib::WriteWStringConfig(SYSTEM_SECTION, info.configName, common_lib::GetFullPath(SYSTEM_CONFIG), std::to_wstring(value));
        }
    }
}
