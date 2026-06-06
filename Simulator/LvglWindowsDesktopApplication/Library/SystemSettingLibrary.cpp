#include "CommonData.h"
#include "CommonLibrary.h"
#include "SystemConfig.h"

namespace systemsetting_lib
{
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
    };

    void SetupSystemData()
    {
        COPY_CDATA_1TO2(setting_data::InitAudState, system_data::CurrentSoundState);
    }

    void SetupSetting()
    {
        for (auto& info : listSettingInfo)
        {
            auto value = config_lib::GetUShortConfig(SYSTEM_SECTION, info.configName, common_lib::GetFullPath(SYSTEM_CONFIG));
            info.settingData->SetValue(value);
            info.tempData->SetValue(value);
            info.configValue = value;
        }
    }

    void SetupTempSetting()
    {
        // Copy setting to temp data
        for (auto& info : listSettingInfo)
        {
            info.tempData->SetValue(0); // Clear data for changing state
            info.tempData->SetValue(info.settingData->GetValue());
        }
    }

    void SaveSetting()
    {
        // Copy temp to setting data
        for (auto& info : listSettingInfo)
        {
            auto value = info.tempData->GetValue();

            info.settingData->SetValue(0); // Clear data for changing state
            info.settingData->SetValue(value);
            info.configValue = value;

            // Write to config file
            config_lib::WriteWStringConfig(SYSTEM_SECTION, info.configName, common_lib::GetFullPath(SYSTEM_CONFIG), std::to_wstring(value));
        }
    }
}
