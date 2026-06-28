#include <thread>
#include <shlwapi.h>
#include "CommonData.h"
#include "CommonLibrary.h"
#include "VideoInfoData.h"

namespace videorecord_lib
{
    static HWND extHwnd = NULL;
    static POINT point;
    static SIZE size;
    bool isMouseRestrict = false;

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

        // Redraw window
#ifdef _DEBUG
        ::SetWindowPos(extHwnd, HWND_TOP, point.x, point.y, size.cx, size.cy, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
#else
        ::SetWindowPos(extHwnd, HWND_TOPMOST, point.x, point.y, size.cx, size.cy, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
        ::SetWindowLongPtr(extHwnd, GWLP_HWNDPARENT, (LONG_PTR)system_data::WindowHandle.GetValue());
#endif
    }

    static void RestrictCursorFromArea(POINT pt, SIZE sz)
    {
        POINT mousePos;

        if (!::GetCursorPos(&mousePos))
        {
            return;
        }

        // Calculate the boundaries of the forbidden zone
        int left = pt.x;
        int top = pt.y;
        int right = pt.x + sz.cx;
        int bottom = pt.y + sz.cy;

        // A 10-pixel buffer zone to intercept high-speed cursor skipping
        int buffer = 10;

        // Check if the cursor is within or approaching the forbidden area boundaries
        if (common_lib::CheckInRangeNumberEqual(mousePos.x, left - buffer, right + buffer)
            && common_lib::CheckInRangeNumberEqual(mousePos.y, top - buffer, bottom + buffer)
            )
        {
            // Get the current screen dimensions
            int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
            int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

            RECT clipRect;

            // Calculate distances to find which side the cursor is approaching from
            int distLeft = mousePos.x - left;
            int distRight = right - mousePos.x;
            int distTop = mousePos.y - top;
            int distBottom = bottom - mousePos.y;

            int minDist = distLeft;

            // Default: Lock the cursor within the left half of the screen
            clipRect = { 0, 0, left - 1, screenHeight };

            // Determine the closest boundary and dynamically split the allowed screen area
            if (distRight < minDist)
            {
                minDist = distRight;
                // Lock within the right half of the screen
                clipRect = { right + 1, 0, screenWidth, screenHeight };
            }
            if (distTop < minDist)
            {
                minDist = distTop;
                // Lock within the upper half of the screen
                clipRect = { 0, 0, screenWidth, top - 1 };
            }
            if (distBottom < minDist)
            {
                minDist = distBottom;
                // Lock within the lower half of the screen
                clipRect = { 0, bottom + 1, screenWidth, screenHeight };
            }

            // Force Windows to confine the cursor using low-level OS boundaries
            ::ClipCursor(&clipRect);

            // Micro-adjust cursor position to satisfy the OS input subsystem
            ::SetCursorPos(mousePos.x, mousePos.y);
        }
        else
        {
            // Free the cursor to move across the entire screen when away from the forbidden zone
            ::ClipCursor(NULL);
        }
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
        point = { lv_obj_get_x(wnd), lv_obj_get_y(wnd) };
        size = { lv_obj_get_width(wnd), lv_obj_get_height(wnd) };

        // Convert to screen coordinates
        ::ClientToScreen(system_data::WindowHandle.GetValue(), &point);

        ChangeWindowStyle(screenName.c_str(), point, size);
        SetWindowFocus(system_data::WindowHandle.GetValue());

        isMouseRestrict = true;

#ifdef NDEBUG
        std::thread([]() {
            while (isMouseRestrict)
            {
                RestrictCursorFromArea(point, size);
                ::Sleep(TIMECYCLE_10MS);
            }
            }).detach();
#endif
    }

    void StopExternalWindow()
    {
        isMouseRestrict = false;

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

                    ::keybd_event(vkey, scanCode, 0, 0);
                    ::keybd_event(vkey, scanCode, KEYEVENTF_KEYUP, 0);

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
            ::keybd_event(VK_LEFT, 0, KEYEVENTF_EXTENDEDKEY, 0);
            ::keybd_event(VK_LEFT, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        }
        else if (forward)
        {
            ::keybd_event(VK_RIGHT, 0, KEYEVENTF_EXTENDEDKEY, 0);
            ::keybd_event(VK_RIGHT, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
        }

        SetWindowFocus(system_data::WindowHandle.GetValue());
    }

    void ExecutePause()
    {
        SetWindowFocus(extHwnd);

        BYTE vkey = LOBYTE(VkKeyScan('P'));
        BYTE scanCode = (BYTE)MapVirtualKey(vkey, MAPVK_VK_TO_VSC);

        ::keybd_event(vkey, scanCode, 0, 0);
        ::keybd_event(vkey, scanCode, KEYEVENTF_KEYUP, 0);

        SetWindowFocus(system_data::WindowHandle.GetValue());
    }

    void KillAllProcess()
    {
        system("taskkill /f /im ffplay.exe");
        system("taskkill /f /im ffmpeg.exe");
        system("taskkill /f /im ffprobe.exe");
    }
}
