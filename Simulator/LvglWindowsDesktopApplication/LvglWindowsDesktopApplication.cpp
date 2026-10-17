#pragma warning(disable : 4244)

#include <Windows.h>
#include <thread>
#include <sstream>
#include <iomanip>
#include <LvglWindowsIconResource.h>
#include <dbghelp.h>

#include "ui/ui.h"
#include "CommonData.h"
#include "CommonLibrary.h"
#include "ScreenMapping.h"

WNDPROC OriginalWndProc = NULL; // Pointer to the original window procedure for subclassing

#ifdef _DEBUG
static void DebugConsoleProcess(); // Forward declaration for processing console commands

// Allocates and attaches a console window for debugging
static void AttachConsoleWindow()
{
    AllocConsole(); // Allocate a new console for the calling process
    FILE* fp; // File pointer for stream redirection
    freopen_s(&fp, "CONOUT$", "w", stdout); // Redirect stdout to console
    freopen_s(&fp, "CONOUT$", "w", stderr); // Redirect stderr to console
    freopen_s(&fp, "CONIN$", "r", stdin); // Redirect stdin from console
}

// Spawns a thread to read and process console input
static void DebugConsoleRead()
{
    std::thread([] {
        while (true)
        {
            std::string inputConsole; // Buffer for console input

            try
            {
                std::getline(std::cin, inputConsole); // Read line from console

                if (!inputConsole.empty())
                {
                    std::istringstream iss(inputConsole); // String stream for parsing
                    std::string param; // Buffer for individual parameters
                    std::vector<std::string> inputParams; // List of parsed parameters

                    while (iss >> param) {
                        inputParams.push_back(param);
                    }

                    debug_data::InputParamList.SetValue(inputParams); // Update debug data shared state

                    DebugConsoleProcess(); // Execute logic based on input
                }
            }
            catch (...)
            {
                debug_println("Process debug data fail! Try again!");
            }
        }
        }).detach();
}

// Processes commands received via the debug console
static void DebugConsoleProcess()
{
    // Debug console
    if (debug_data::InputParamList.GetState())
    {
        try {
            auto inputParams = debug_data::InputParamList.GetValue(); // Retrieve parsed parameters

            // Test case: Show a standard Windows message box
            if (inputParams.at(0) == "test")
            {
                ::MessageBox(NULL, L"Test show message box", L"Test", MB_OK);
            }
            // Test case: Populate temporary video data
            else if (inputParams.at(0) == "input_test")
            {
                temp_data::VideoEvent.SetValue("Test event");
                temp_data::VideoName.SetValue("Test name");
                temp_data::VideoCategory.SetValue("Test category");
                temp_data::VideoDesc.SetValue("Test desc");
                temp_data::VideoAuthor.SetValue("Test author");
            }
            // Test case: Simulate hardware key presses
            else if (inputParams.at(0) == "appkey")
            {
                int key = 0; // Virtual key code

                ::Sleep(TIMECYCLE_10MS);
                ::SetForegroundWindow(system_data::WindowHandle.GetValue());
                ::SetFocus(system_data::WindowHandle.GetValue());

                if (inputParams.at(1) == "home")
                {
                    key = VK_BROWSER_HOME;
                }
                else if (inputParams.at(1) == "back")
                {
                    key = VK_BROWSER_BACK;
                }
                else if (inputParams.at(1) == "tab")
                {
                    key = VK_TAB;
                }
                else if (inputParams.at(1) == "mail")
                {
                    key = VK_LAUNCH_MAIL;
                }
                else if (inputParams.at(1) == "calc")
                {
                    key = VK_LAUNCH_APP2;
                }

                if (key)
                {
                    KBDLLHOOKSTRUCT lParam = {
                        key,
                        ::MapVirtualKey(key, MAPVK_VK_TO_VSC),
                        0, 0, 0
                    };

                    // Manually trigger the low-level keyboard hook procedure
                    keyboard_lib::LowLevelKeyboardProc(HC_ACTION, WM_KEYDOWN, (LPARAM)&lParam);
                    keyboard_lib::LowLevelKeyboardProc(HC_ACTION, WM_KEYUP, (LPARAM)&lParam);
                }
            }

            debug_println("Process debug data done!");
        }
        catch (...)
        {
            debug_println("Process debug data fail! Try again!");
        }

        debug_data::InputParamList.SetValue({ });
        debug_data::InputParamList.ResetState();
    }
}
#endif

// Custom window procedure to handle specific Windows messages
static LRESULT CALLBACK MyNewWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Process hardware keyboard events
    keyboard_lib::HardwareKeyboardProcess(hwnd, uMsg, wParam, (lv_event_code_t)lParam);

    switch (uMsg)
    {
    case WM_APPCOMMAND: // Handle multimedia and browser keys
    {
        int cmd = GET_APPCOMMAND_LPARAM(lParam); // Extract command
        int key = 0; // Corresponding virtual key

        switch (cmd)
        {
        case APPCOMMAND_BROWSER_HOME:
            key = VK_BROWSER_HOME;
            break;
        case APPCOMMAND_BROWSER_BACKWARD:
            key = VK_BROWSER_BACK;
            break;
        case APPCOMMAND_LAUNCH_MAIL:
            key = VK_LAUNCH_MAIL;
            break;
        case APPCOMMAND_LAUNCH_APP2:
            key = VK_LAUNCH_APP2;
            break;
        default:
            break;
        }

        if (key)
        {
            KBDLLHOOKSTRUCT lParam = {
                key,
                ::MapVirtualKey(key, MAPVK_VK_TO_VSC),
                0, 0, 0
            };

            // Trigger internal keyboard processing
            keyboard_lib::LowLevelKeyboardProc(HC_ACTION, WM_KEYDOWN, (LPARAM)&lParam);
            keyboard_lib::LowLevelKeyboardProc(HC_ACTION, WM_KEYUP, (LPARAM)&lParam);
        }
    }
    break;
    case WM_DEVICECHANGE: // Notify when hardware devices (like USB) are plugged/unplugged
        storage_lib::CollectExternalDrivesList();
        system_data::DeviceChange.SetValue(true);
        break;
    case WM_DEVICECHANGE_DONE: // Internal message to signal completion of device processing
        system_data::DeviceChange.SetValue(false);
        break;
    case WM_HOOK_NUMPAD_START: // Custom message to start numpad hook
        keyboard_lib::SetupHookInternalNumpad();
        break;
    case WM_HOOK_NUMPAD_STOP: // Custom message to stop numpad hook
        keyboard_lib::StopHookInternalNumpad();
        break;
    case WM_QUIT:
    case WM_DESTROY:
    case WM_NCDESTROY: // Handle application shutdown
        keyboard_lib::StopHookInternalNumpad();
        videorecord_lib::KillAllProcess();
        ::SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)OriginalWndProc); // Restore original winproc
        break;

    default:
        break;
    }

    return CallWindowProc(OriginalWndProc, hwnd, uMsg, wParam, lParam);
}

// Exception handler to create a minidump when the application crashes
static LONG WINAPI OurCrashHandler(EXCEPTION_POINTERS* pExceptionInfo)
{
    std::wstring dumpFileName = L"crash_report.dmp"; // File name for the crash dump

    HANDLE hFile = CreateFileW(dumpFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile != INVALID_HANDLE_VALUE)
    {
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo; // Minidump exception metadata
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ExceptionPointers = pExceptionInfo;
        dumpInfo.ClientPointers = TRUE;

        // Specify dump level (full memory and additional metadata)
        MINIDUMP_TYPE dumpType = (MINIDUMP_TYPE)(MiniDumpWithFullMemory | MiniDumpWithHandleData | MiniDumpWithThreadInfo | MiniDumpWithUnloadedModules);

        // Write the crash dump to file
        BOOL success = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, dumpType, &dumpInfo, NULL, NULL);

        CloseHandle(hFile);
    }

    return EXCEPTION_EXECUTE_HANDLER;
}

// WinMain entry point
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nShowCmd);

    ::SetUnhandledExceptionFilter(OurCrashHandler); // Register custom crash handler

    lv_init(); // Initialize LVGL graphics library

    int32_t zoom_level = 100; // Display zoom factor
    bool allow_dpi_override = false; // DPI awareness flag
    bool simulator_mode = false; // LVGL simulator flag
    lv_display_t* display = lv_windows_create_display(APP_NAME, HORIZON_MAX_RESOLUTION, VERTICAL_MAX_RESOLUTION, zoom_level, allow_dpi_override, simulator_mode);
    if (!display)
    {
        return -1;
    }

    HWND window_handle = lv_windows_get_display_window_handle(display); // Get native window handle
    if (!window_handle)
    {
        return -1;
    }
    else
    {
        system_data::WindowHandle.SetValue(window_handle); // Store window handle globally
    }

    // Set application icon
    HICON icon_handle = ::LoadIconW(hInstance, MAKEINTRESOURCE(IDI_LVGL_WINDOWS));
    if (icon_handle)
    {
        ::SendMessageW(window_handle, WM_SETICON, TRUE, (LPARAM)icon_handle);
        ::SendMessageW(window_handle, WM_SETICON, FALSE, (LPARAM)icon_handle);
    }

    // Acquire pointer input device
    lv_indev_t* pointer_indev = lv_windows_acquire_pointer_indev(display);
    if (!pointer_indev)
    {
        return -1;
    }

    // Subclass the main window with custom procedure
    OriginalWndProc = (WNDPROC)::SetWindowLongPtr(window_handle, GWLP_WNDPROC, (LONG_PTR)MyNewWinProc);
    if (!OriginalWndProc)
    {
        return -1;
    }

#ifdef NDEBUG
    // Production window styling: Hide title bar and disable resizing
    DWORD style = ::GetWindowLongPtr(window_handle, GWL_STYLE);
    style &= ~WS_CAPTION;
    style &= ~WS_THICKFRAME;
    ::SetWindowLongPtr(window_handle, GWL_STYLE, style);

    // Position window as topmost and full resolution
    ::SetWindowPos(window_handle, HWND_TOPMOST, 0, 0, HORIZON_MAX_RESOLUTION, VERTICAL_MAX_RESOLUTION, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
#endif

#ifdef _DEBUG
    AttachConsoleWindow(); // Attach debug console in debug builds
#endif

    // Initial external device scan
    ::SendMessage(window_handle, WM_DEVICECHANGE, 0, 0);

    // Initialize UI components
    ui_init();

    // Initialize system settings and data
    systemsetting_lib::SetupSetting();
    systemsetting_lib::SetupSystemData();

    // Configure dropdown lists
    dropdownlist_lib::SetupDropdownList();

    // Initialize volume management
    soundvolume_lib::CreateSoundVolumeWindow();

    // Start keyboard hook
    ::SendMessage(system_data::WindowHandle.GetValue(), WM_HOOK_NUMPAD_START, 0, 0);

    // Navigate to the main screen
    ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::SCREEN_MAIN);

#ifdef _DEBUG
    DebugConsoleRead(); // Start console input thread
#endif

    // Application main loop
    while (true)
    {
        lv_timer_handler(); // Process LVGL timers and UI events
        ScreenMapping::GetInstance().HandleScreen(); // Process screen transitions and logic
        CommonDataUpdateAll(); // Synchronize shared data

        ::Sleep(TIMECYCLE_10MS); // Throttle loop execution
    }

    lv_deinit(); // Clean up LVGL

    ::FreeConsole(); // Detach debug console

    return 0;
}
