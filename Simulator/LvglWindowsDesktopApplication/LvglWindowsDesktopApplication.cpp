#pragma warning(disable : 4244)

#include <Windows.h>
#include <thread>
#include <sstream>
#include <iomanip>
#include <LvglWindowsIconResource.h>

#include "ui/ui.h"
#include "CommonData.h"
#include "CommonService.h"
#include "CommonLibrary.h"
#include "ScreenMapping.h"

WNDPROC OriginalWndProc = NULL;
HWND ClientHandle = NULL;

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
    case WM_QUIT:
    case WM_DESTROY:
    case WM_NCDESTROY:
        videorecord_lib::KillAllProcess();
        ::SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)OriginalWndProc);
        break;

    default:
        break;
    }

    return CallWindowProc(OriginalWndProc, hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nShowCmd);

    ::lv_init();

    int32_t zoom_level = 100;
    bool allow_dpi_override = false;
    bool simulator_mode = false;
    lv_display_t* display = ::lv_windows_create_display(APP_NAME, HORIZON_MAX_RESOLUTION, VERTICAL_MAX_RESOLUTION, zoom_level, allow_dpi_override, simulator_mode);
    if (!display)
    {
        return -1;
    }

    HWND window_handle = ::lv_windows_get_display_window_handle(display);
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

    lv_indev_t* pointer_indev = ::lv_windows_acquire_pointer_indev(display);
    if (!pointer_indev)
    {
        return -1;
    }

    lv_indev_t* keypad_indev = ::lv_windows_acquire_keypad_indev(display);
    if (!keypad_indev)
    {
        return -1;
    }

    lv_indev_t* encoder_indev = ::lv_windows_acquire_encoder_indev(display);
    if (!encoder_indev)
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
    LONG_PTR style = ::GetWindowLongPtr(window_handle, GWL_STYLE);
    style &= ~WS_CAPTION;
    style &= ~WS_THICKFRAME;
    ::SetWindowLongPtr(window_handle, GWL_STYLE, style);

    // Redraw window
    ::SetWindowPos(window_handle, HWND_TOPMOST, 0, 0, HORIZON_MAX_RESOLUTION, VERTICAL_MAX_RESOLUTION, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
#endif

#ifdef _DEBUG
    AttachConsoleWindow();
#endif

    // Create UI
    ui_init();

    // Init service
    InitService();

    // Change to main screen
    ScreenMapping::GetInstance().ChangeScreen(SCREEN_NAME::SCREEN_MAIN);

#ifdef _DEBUG
    DebugConsoleRead();
#endif

    while (true)
    {
        ::lv_timer_handler();
        ServiceProcess();
        ScreenMapping::GetInstance().HandleScreen();
        CommonDataUpdateAll();

        ::Sleep(10);
    }

    lv_deinit();

    ::FreeConsole();

    return 0;
}
