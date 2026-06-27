#pragma warning(disable : 4244)

#include <Windows.h>
#include <thread>
#include <sstream>
#include <iomanip>
#include <LvglWindowsIconResource.h>
#include <dbghelp.h>

#include "ui/ui.h"
#include "CommonData.h"
#include "CommonService.h"
#include "CommonLibrary.h"
#include "ScreenMapping.h"

WNDPROC OriginalWndProc = NULL;

#ifdef _DEBUG
static void DebugConsoleProcess();

static void AttachConsoleWindow()
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);
}

static void DebugConsoleRead()
{
    std::thread([] {
        while (true)
        {
            std::string inputConsole;

            try
            {
                std::getline(std::cin, inputConsole);

                if (!inputConsole.empty())
                {
                    std::istringstream iss(inputConsole);
                    std::string param;
                    std::vector<std::string> inputParams;

                    while (iss >> param) {
                        inputParams.push_back(param);
                    }

                    debug_data::InputParamList.SetValue(inputParams);

                    DebugConsoleProcess();
                }
            }
            catch (...)
            {
                debug_println("Process debug data fail! Try again!");
            }
        }
        }).detach();
}

static void DebugConsoleProcess()
{
    // Debug console
    if (debug_data::InputParamList.GetState())
    {
        try {
            auto inputParams = debug_data::InputParamList.GetValue();

            // Test show message box
            if (inputParams.at(0) == "test")
            {
                ::MessageBox(NULL, L"Test show message box", L"Test", MB_OK);
            }
            else if (inputParams.at(0) == "input_test")
            {
                temp_data::VideoEvent.SetValue("Test event");
                temp_data::VideoName.SetValue("Test name");
                temp_data::VideoCategory.SetValue("Test category");
                temp_data::VideoDesc.SetValue("Test desc");
                temp_data::VideoAuthor.SetValue("Test author");
            }
            else if (inputParams.at(0) == "appkey")
            {
                int key = 0;

                ::Sleep(TIMECYCLE_10MS);
                ::SetForegroundWindow(system_data::WindowHandle.GetValue());
                ::SetFocus(system_data::WindowHandle.GetValue());

                if (inputParams.at(1) == "home")
                {
                    key = VK_BROWSER_HOME;
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

                KBDLLHOOKSTRUCT lParam = {
                    key,
                    ::MapVirtualKey(key, MAPVK_VK_TO_VSC),
                    0, 0, 0
                };

                keyboard_lib::LowLevelKeyboardProc(HC_ACTION, WM_KEYDOWN, (LPARAM)&lParam);
                keyboard_lib::LowLevelKeyboardProc(HC_ACTION, WM_KEYUP, (LPARAM)&lParam);
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

static LRESULT CALLBACK MyNewWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    keyboard_lib::HardwareKeyboardProcess(hwnd, uMsg, wParam, (lv_event_code_t)lParam);

    switch (uMsg)
    {
    case WM_APPCOMMAND:
    {
        int cmd = GET_APPCOMMAND_LPARAM(lParam);
        int key = 0;

        switch (cmd)
        {
        case APPCOMMAND_BROWSER_HOME:
            key = VK_BROWSER_HOME;
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

        KBDLLHOOKSTRUCT lParam = {
            key,
            ::MapVirtualKey(key, MAPVK_VK_TO_VSC),
            0, 0, 0
        };

        keyboard_lib::LowLevelKeyboardProc(HC_ACTION, WM_KEYDOWN, (LPARAM)&lParam);
        keyboard_lib::LowLevelKeyboardProc(HC_ACTION, WM_KEYUP, (LPARAM)&lParam);
    }
    break;
    case WM_DEVICECHANGE:
        storage_lib::CollectExternalDrivesList();
        system_data::DeviceChange.SetValue(true);
        break;
    case WM_DEVICECHANGE_DONE:
        system_data::DeviceChange.SetValue(false);
        break;
    case WM_HOOK_NUMPAD_START:
        keyboard_lib::SetupHookInternalNumpad();
        break;
    case WM_HOOK_NUMPAD_STOP:
        keyboard_lib::StopHookInternalNumpad();
        break;
    case WM_QUIT:
    case WM_DESTROY:
    case WM_NCDESTROY:
        keyboard_lib::StopHookInternalNumpad();
        videorecord_lib::KillAllProcess();
        ::SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)OriginalWndProc);
        break;

    default:
        break;
    }

    return CallWindowProc(OriginalWndProc, hwnd, uMsg, wParam, lParam);
}

static LONG WINAPI OurCrashHandler(EXCEPTION_POINTERS* pExceptionInfo)
{
    std::wstring dumpFileName = L"crash_report.dmp";

    HANDLE hFile = CreateFileW(dumpFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile != INVALID_HANDLE_VALUE)
    {
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ExceptionPointers = pExceptionInfo;
        dumpInfo.ClientPointers = TRUE;

        BOOL success = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpWithDataSegs, &dumpInfo, NULL, NULL);

        CloseHandle(hFile);
    }

    return EXCEPTION_EXECUTE_HANDLER;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nShowCmd);

    ::SetUnhandledExceptionFilter(OurCrashHandler);

    lv_init();

    int32_t zoom_level = 100;
    bool allow_dpi_override = false;
    bool simulator_mode = false;
    lv_display_t* display = lv_windows_create_display(APP_NAME, HORIZON_MAX_RESOLUTION, VERTICAL_MAX_RESOLUTION, zoom_level, allow_dpi_override, simulator_mode);
    if (!display)
    {
        return -1;
    }

    HWND window_handle = lv_windows_get_display_window_handle(display);
    if (!window_handle)
    {
        return -1;
    }
    else
    {
        system_data::WindowHandle.SetValue(window_handle);
    }

    HICON icon_handle = ::LoadIconW(hInstance, MAKEINTRESOURCE(IDI_LVGL_WINDOWS));
    if (icon_handle)
    {
        ::SendMessageW(window_handle, WM_SETICON, TRUE, (LPARAM)icon_handle);
        ::SendMessageW(window_handle, WM_SETICON, FALSE, (LPARAM)icon_handle);
    }

    lv_indev_t* pointer_indev = lv_windows_acquire_pointer_indev(display);
    if (!pointer_indev)
    {
        return -1;
    }

    // Set custom winproc
    OriginalWndProc = (WNDPROC)::SetWindowLongPtr(window_handle, GWLP_WNDPROC, (LONG_PTR)MyNewWinProc);
    if (!OriginalWndProc)
    {
        return -1;
    }

#ifdef NDEBUG
    // Hide caption, block resizable
    DWORD style = ::GetWindowLongPtr(window_handle, GWL_STYLE);
    style &= ~WS_CAPTION;
    style &= ~WS_THICKFRAME;
    ::SetWindowLongPtr(window_handle, GWL_STYLE, style);

    // Redraw window
    ::SetWindowPos(window_handle, HWND_TOPMOST, 0, 0, HORIZON_MAX_RESOLUTION, VERTICAL_MAX_RESOLUTION, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
#endif

#ifdef _DEBUG
    AttachConsoleWindow();
#endif

    // Init external device
    ::SendMessage(window_handle, WM_DEVICECHANGE, 0, 0);

    // Create UI
    ui_init();

    // Init setting
    systemsetting_lib::SetupSetting();
    systemsetting_lib::SetupSystemData();

    // Setup dropdownlist
    dropdownlist_lib::SetupDropdownList();

    // Init service
    InitService();

    soundvolume_lib::CreateSoundVolumeWindow();

    ::SendMessage(system_data::WindowHandle.GetValue(), WM_HOOK_NUMPAD_START, 0, 0);

    // Change to main screen
    ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::SCREEN_MAIN);

#ifdef _DEBUG
    DebugConsoleRead();
#endif

    while (true)
    {
        lv_timer_handler();
        ServiceProcess();
        ScreenMapping::GetInstance().HandleScreen();
        CommonDataUpdateAll();

        ::Sleep(TIMECYCLE_10MS);
    }

    lv_deinit();

    ::FreeConsole();

    return 0;
}
