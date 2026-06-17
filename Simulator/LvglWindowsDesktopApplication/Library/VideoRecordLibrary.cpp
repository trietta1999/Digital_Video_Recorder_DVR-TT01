#include <shlwapi.h>
#include "CommonData.h"
#include "CommonLibrary.h"
#include "VideoInfoData.h"

namespace videorecord_lib
{
    static HWND extHwnd = NULL;
    static std::wstring listScreenName[] = { REVIEW_SCREENNAME, RECORD_SCREENNAME, RECORD_PROCESSNAME, PLAY_SCREENNAME, DUKTO_SCREENNAME };

    static HWND Wait(std::wstring screenName)
    {
        HWND hwnd = 0;

        while (!hwnd)
        {
            hwnd = ::FindWindow(NULL, screenName.c_str());
            ::Sleep(TIMECYCLE_10MS);
        }

        return hwnd;
    }

    static void SetWindowFocus(HWND hwnd)
    {
        ::Sleep(TIMECYCLE_10MS);
        ::SetForegroundWindow(hwnd);
        ::SetFocus(hwnd);
    }

    static void ChangeWindowStyle(std::wstring screenName, POINT point, SIZE size)
    {
        // Hide caption, block resizable
        LONG_PTR style = ::GetWindowLongPtr(extHwnd, GWL_STYLE);
        style &= ~WS_CAPTION;
        style &= ~WS_THICKFRAME;
        style |= WS_BORDER;
        ::SetWindowLongPtr(extHwnd, GWL_STYLE, style);

        // Block mouse clicking
        LONG_PTR exStyle = ::GetWindowLongPtr(extHwnd, GWL_EXSTYLE);
        ::SetWindowLongPtr(extHwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED | WS_EX_TRANSPARENT);
        ::SetLayeredWindowAttributes(extHwnd, 0, 255, LWA_ALPHA);

        // Redraw window
#ifdef _DEBUG
        ::SetWindowPos(extHwnd, HWND_TOP, point.x, point.y, size.cx, size.cy, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
#else
        ::SetWindowPos(extHwnd, HWND_TOPMOST, point.x, point.y, size.cx, size.cy, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
        ::SetWindowLongPtr(extHwnd, GWLP_HWNDPARENT, (LONG_PTR)system_data::WindowHandle.GetValue());
#endif
    }

    // @todo: refactor later
    void StartExternalWindow(lv_obj_t* wnd, std::wstring screenName, std::string videoID)
    {
        StopExternalWindow();
        //KillAllProcess();

        //std::thread([screenName]() {
        //    WNDCLASS wc = {};
        //    wc.lpfnWndProc = ::DefWindowProc;
        //    wc.hInstance = ::GetModuleHandle(NULL);
        //    wc.lpszClassName = screenName.c_str();
        //    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        //    wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
        //    ::RegisterClass(&wc);
        //    HWND hwnd = ::CreateWindowEx(0, screenName.c_str(), screenName.c_str(),
        //        WS_VISIBLE,
        //        CW_USEDEFAULT, CW_USEDEFAULT, 1, 1, system_data::WindowHandle.GetValue(), NULL, GetModuleHandle(NULL), NULL);
        //    CreateWindowEx(
        //        0, L"STATIC", screenName.c_str(), WS_CHILD | WS_VISIBLE | SS_LEFT, 0, 0, 100, 20, hwnd, NULL, wc.hInstance, NULL);
        //    MSG msg = {};
        //    while (::GetMessage(&msg, hwnd, 0, 0))
        //    {
        //        ::TranslateMessage(&msg);
        //        ::DispatchMessage(&msg);
        //    }
        //    }).detach();
        //::Sleep(100);

        wchar_t param[MAX_CHARS] = { 0 };
        wchar_t scriptPath[MAX_CHARS] = { 0 };
        wchar_t outputPath[MAX_CHARS] = { 0 };

        if (screenName == RECORD_SCREENNAME)
        {
            swprintf_s(outputPath, L"%s\\video_data\\%s", common_lib::GetSystemPath().c_str(), common_lib::ConvertStringToWString(videoID).c_str());
            swprintf_s(param, L"$host.UI.RawUI.WindowTitle = \\\"%s\\\";cd \\\"%s\\\";.\\record.bat \\\"%s\\\" \\\"%s\\\\video.mp4\\\"", RECORD_PROCESSNAME, common_lib::GetFullPath(SCRIPT_DIRECTORY).c_str(), RECORD_SCREENNAME, outputPath);
            ::ShellExecute(NULL, L"open", L"powershell", param, common_lib::GetFullPath(SCRIPT_DIRECTORY).c_str(), SW_HIDE);
        }
        else if (screenName == PLAY_SCREENNAME)
        {
            swprintf_s(outputPath, L"%s\\video_data\\%s\\video.mp4", common_lib::GetSystemPath().c_str(), common_lib::ConvertStringToWString(videoID).c_str());
            swprintf_s(param, L"\"%s\" \"%s\"", PLAY_SCREENNAME, outputPath);
            swprintf_s(scriptPath, L"%s\\play.bat", SCRIPT_DIRECTORY);
            ::ShellExecute(NULL, L"open", common_lib::GetFullPath(scriptPath).c_str(), param, common_lib::GetFullPath(SCRIPT_DIRECTORY).c_str(), SW_HIDE);
        }
        else if (screenName == REVIEW_SCREENNAME)
        {
            swprintf_s(param, L"\"%s\"", REVIEW_SCREENNAME);
            swprintf_s(scriptPath, L"%s\\review.bat", SCRIPT_DIRECTORY);
            ::ShellExecute(NULL, L"open", common_lib::GetFullPath(scriptPath).c_str(), param, common_lib::GetFullPath(SCRIPT_DIRECTORY).c_str(), SW_HIDE);
        }

        // Wait for process starting
        extHwnd = Wait(screenName);

        // Get LVGL window review
        POINT point = { lv_obj_get_x(wnd), lv_obj_get_y(wnd) };
        SIZE size = { lv_obj_get_width(wnd), lv_obj_get_height(wnd) };

        // Convert to screen coordinates
        ::ClientToScreen(system_data::WindowHandle.GetValue(), &point);

        ChangeWindowStyle(screenName.c_str(), point, size);
        SetWindowFocus(system_data::WindowHandle.GetValue());
    }

    void StopExternalWindow()
    {
        for (const auto& screen : listScreenName)
        {
            auto hwnd = ::FindWindow(NULL, screen.c_str());

            if (hwnd)
            {
                if (screen != RECORD_PROCESSNAME)
                {
                    ::SendMessage(hwnd, WM_CLOSE, 0, 0);
                }
                else
                {
                    SetWindowFocus(hwnd);

                    BYTE vkey = LOBYTE(VkKeyScan('Q'));
                    BYTE scanCode = (BYTE)MapVirtualKey(vkey, MAPVK_VK_TO_VSC);

                    keybd_event(vkey, scanCode, 0, 0);
                    keybd_event(vkey, scanCode, KEYEVENTF_KEYUP, 0);

                    SetWindowFocus(system_data::WindowHandle.GetValue());
                }
            }
        }
    }

    void ExecuteSeek(bool rewind, bool forward)
    {
        SetWindowFocus(extHwnd);

        if (rewind)
        {
            keybd_event(VK_LEFT, 0, KEYEVENTF_EXTENDEDKEY, 0);
            keybd_event(VK_LEFT, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        }
        else if (forward)
        {
            keybd_event(VK_RIGHT, 0, KEYEVENTF_EXTENDEDKEY, 0);
            keybd_event(VK_RIGHT, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        }

        SetWindowFocus(system_data::WindowHandle.GetValue());
    }

    void ExecutePause()
    {
        SetWindowFocus(extHwnd);

        BYTE vkey = LOBYTE(VkKeyScan('P'));
        BYTE scanCode = (BYTE)MapVirtualKey(vkey, MAPVK_VK_TO_VSC);

        keybd_event(vkey, scanCode, 0, 0);
        keybd_event(vkey, scanCode, KEYEVENTF_KEYUP, 0);

        SetWindowFocus(system_data::WindowHandle.GetValue());
    }

    void KillAllProcess()
    {
        system("taskkill /f /im ffplay.exe");
        system("taskkill /f /im ffmpeg.exe");
        system("taskkill /f /im ffprobe.exe");
    }
}
