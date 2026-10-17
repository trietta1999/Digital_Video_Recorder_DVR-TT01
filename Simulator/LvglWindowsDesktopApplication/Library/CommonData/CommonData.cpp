#include "CommonData.h"

namespace system_data
{
    CData<HWND> WindowHandle; // Native handle of the main application window
    CData<SCREEN_NAME> CurrentKbScreen; // Current virtual keyboard screen context
    CData<SCREEN_NAME> CurrentScreen; // Current active application screen
    CData<std::string> CurrentDate; // Formatted current date string
    CData<std::string> CurrentTime; // Formatted current time string
    CData<std::string> FreeStorage; // Available internal storage space description
    CData<std::string> FreeUSBStorage; // Available external USB storage space description
    CData<STATE_TYPE> CurrentState; // Current system operation state (Record, Play, etc.)
    CData<bool> IsTempVideoInfo; // Flag indicating if current video info is non-persistent
    CData<double> TransferPercent; // Progress percentage for file transfer operations
    CData<bool> DeviceChange; // Flag indicating a hardware configuration change
}

namespace current_videoinfo_data
{
    CData<std::string> VideoID; // ID of the currently active or selected video
    CData<std::string> VideoEvent; // Event title of current video
    CData<std::string> VideoName; // Filename/Title of current video
    CData<std::string> VideoCategory; // Category of current video
    CData<std::string> VideoDesc; // Detailed description of current video
    CData<std::string> VideoAuthor; // Author/Operator of current video
    CData<unsigned short> VideoDay; // Recording day
    CData<unsigned short> VideoMonth; // Recording month
    CData<unsigned short> VideoYear; // Recording year
    CData<unsigned short> VideoHour; // Recording hour
    CData<unsigned short> VideoMinute; // Recording minute
    CData<unsigned short> VideoSecond; // Recording second
}

namespace input_data
{
    CData<std::string> VideoEvent; // UI input buffer for video event
    CData<std::string> VideoName; // UI input buffer for video name
    CData<std::string> VideoCategory; // UI input buffer for video category
    CData<std::string> VideoDesc; // UI input buffer for video description
    CData<std::string> VideoAuthor; // UI input buffer for video author
    CData<std::string> VideoSearch; // UI input buffer for search query
    CData<short> VideoSearchType; // Selected filter type for video search
}

namespace setting_data
{
    CData<short> DateFormat; // Selected date display format (DDMMYYYY, etc.)
    CData<short> DateSeparator; // Selected character to separate date parts
    CData<short> InitAudState; // Initial audio state on startup
    CData<short> PreviewVideoState; // Configuration for video preview behavior
    CData<short> PlayVideoState; // Configuration for video playback behavior
    CData<short> KeyboardType; // Selected keyboard layout (Numpad, QWERTY, etc.)
    CData<short> T9AutoConfirmSpeed; // Speed of T9 input auto-confirmation
    CData<short> KeyboardExitShortcutState; // Toggle for keyboard exit shortcut
    CData<short> InsSpaceAfterPuncState; // Auto-insert space after punctuation
    CData<short> AutoCloseBracketState; // Auto-close brackets during typing
    CData<short> NetworkType; // Current network connection type (LAN/WiFi)
    CData<short> SSID; // Index of the selected WiFi SSID
    CData<short> DuktoIPPart12; // First two octets of Dukto server IP
    CData<short> DuktoIPPart34; // Last two octets of Dukto server IP
}

namespace temp_data
{
    CData<std::string> VideoEvent; // Staging buffer for video event editing
    CData<std::string> VideoName; // Staging buffer for video name editing
    CData<std::string> VideoCategory; // Staging buffer for video category editing
    CData<std::string> VideoDesc; // Staging buffer for video description editing
    CData<std::string> VideoAuthor; // Staging buffer for video author editing
    CData<std::string> VideoSearch; // Staging buffer for search text

    CData<short> DateFormat; // Unsaved preference for date format
    CData<short> DateSeparator; // Unsaved preference for date separator
    CData<short> InitAudState; // Unsaved preference for audio state
    CData<short> PreviewVideoState; // Unsaved preference for preview behavior
    CData<short> PlayVideoState; // Unsaved preference for playback behavior
    CData<short> KeyboardType; // Unsaved preference for keyboard type
    CData<short> T9AutoConfirmSpeed; // Unsaved preference for T9 speed
    CData<short> KeyboardExitShortcutState; // Unsaved preference for keyboard exit shortcut
    CData<short> InsSpaceAfterPuncState; // Unsaved preference for punctuation space
    CData<short> AutoCloseBracketState; // Unsaved preference for bracket closing
    CData<short> NetworkType; // Unsaved preference for network type
    CData<short> SSID; // Unsaved preference for WiFi SSID
    CData<std::string> WiFiPassword; // Buffer for WiFi password entry
    CData<std::string> DuktoIP; // Buffer for Dukto IP string
    CData<short> DuktoIPPart12; // Unsaved preference for Dukto IP part 1&2
    CData<short> DuktoIPPart34; // Unsaved preference for Dukto IP part 3&4
}

namespace debug_data
{
    CData<std::vector<std::string>> InputParamList; // List of command line arguments
}

void CommonDataUpdateAll()
{
    system_data::WindowHandle.ResetState();
    system_data::CurrentKbScreen.ResetState();
    system_data::CurrentScreen.ResetState();
    system_data::CurrentDate.ResetState();
    system_data::CurrentTime.ResetState();
    system_data::FreeStorage.ResetState();
    system_data::FreeUSBStorage.ResetState();
    system_data::CurrentState.ResetState();
    system_data::IsTempVideoInfo.ResetState();
    system_data::TransferPercent.ResetState();
    system_data::DeviceChange.ResetState();

    current_videoinfo_data::VideoID.ResetState();
    current_videoinfo_data::VideoEvent.ResetState();
    current_videoinfo_data::VideoName.ResetState();
    current_videoinfo_data::VideoCategory.ResetState();
    current_videoinfo_data::VideoDesc.ResetState();
    current_videoinfo_data::VideoAuthor.ResetState();
    current_videoinfo_data::VideoDay.ResetState();
    current_videoinfo_data::VideoMonth.ResetState();
    current_videoinfo_data::VideoYear.ResetState();
    current_videoinfo_data::VideoHour.ResetState();
    current_videoinfo_data::VideoMinute.ResetState();
    current_videoinfo_data::VideoSecond.ResetState();

    input_data::VideoEvent.ResetState();
    input_data::VideoName.ResetState();
    input_data::VideoCategory.ResetState();
    input_data::VideoDesc.ResetState();
    input_data::VideoAuthor.ResetState();
    input_data::VideoSearch.ResetState();
    input_data::VideoSearchType.ResetState();

    setting_data::DateFormat.ResetState();
    setting_data::DateSeparator.ResetState();
    setting_data::InitAudState.ResetState();
    setting_data::PreviewVideoState.ResetState();
    setting_data::PlayVideoState.ResetState();
    setting_data::KeyboardType.ResetState();
    setting_data::T9AutoConfirmSpeed.ResetState();
    setting_data::KeyboardExitShortcutState.ResetState();
    setting_data::InsSpaceAfterPuncState.ResetState();
    setting_data::AutoCloseBracketState.ResetState();
    setting_data::NetworkType.ResetState();
    setting_data::SSID.ResetState();
    setting_data::DuktoIPPart12.ResetState();
    setting_data::DuktoIPPart34.ResetState();

    temp_data::VideoEvent.ResetState();
    temp_data::VideoName.ResetState();
    temp_data::VideoCategory.ResetState();
    temp_data::VideoDesc.ResetState();
    temp_data::VideoAuthor.ResetState();

    temp_data::DateFormat.ResetState();
    temp_data::DateSeparator.ResetState();
    temp_data::InitAudState.ResetState();
    temp_data::PreviewVideoState.ResetState();
    temp_data::PlayVideoState.ResetState();
    temp_data::KeyboardType.ResetState();
    temp_data::T9AutoConfirmSpeed.ResetState();
    temp_data::KeyboardExitShortcutState.ResetState();
    temp_data::InsSpaceAfterPuncState.ResetState();
    temp_data::AutoCloseBracketState.ResetState();
    temp_data::NetworkType.ResetState();
    temp_data::SSID.ResetState();
    temp_data::WiFiPassword.ResetState();
    temp_data::DuktoIP.ResetState();
    temp_data::DuktoIPPart12.ResetState();
    temp_data::DuktoIPPart34.ResetState();
}
