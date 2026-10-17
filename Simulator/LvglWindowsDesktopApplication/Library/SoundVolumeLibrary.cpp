#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <thread>
#include "CommonData.h"
#include "CommonLibrary.h"

#define WND_SV_WIDTH 250
#define WND_SV_HEIGHT 100
#define LBL_SV_TEXTSIZE 100

namespace soundvolume_lib
{
    bool isMute = false; // Flag to track if the system sound is currently muted

    static HWND hWnd = NULL; // Handle for the transparent volume overlay window
    static HWND hStatic = NULL; // Handle for the static text control inside the overlay
    static int currentVolume = 0; // Cached copy of the current system volume (0-100)

    static void CALLBACK AutoHideWindow(HWND hwnd, UINT uMsg, UINT_PTR timerId, DWORD dwTime)
    {
        // Callback function triggered by the TID_VOLUME_WND timer
        ::KillTimer(hwnd, TID_VOLUME_WND);
        ::ShowWindow(hwnd, SW_HIDE); // Hide the overlay after a timeout
    }

    static LRESULT CALLBACK MyWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        // Custom window procedure for the volume overlay
        switch (uMsg)
        {
        case WM_SETTEXT:
            // Custom handler to show/update the overlay text and restart the auto-hide timer
            ::KillTimer(hwnd, TID_VOLUME_WND);

            ::SetWindowText(hStatic, (LPCWSTR)lParam);
            ::ShowWindow(hwnd, SW_SHOW);

            ::SetTimer(hwnd, TID_VOLUME_WND, TIMECYCLE_3SEC, AutoHideWindow);

            // Ensure the main app window retains focus even when overlay is shown
            ::SetForegroundWindow(::GetParent(hwnd));
            ::SetFocus(::GetParent(hwnd));

            break;
        default:
            break;
        }

        return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    void CreateSoundVolumeWindow()
    {
        // Initializes the volume overlay in a background thread to avoid blocking LVGL
        std::thread([]() {
            HWND hParent = system_data::WindowHandle.GetValue();

            // Calculate center position relative to the main application window
            RECT parentRect;
            ::GetClientRect(hParent, &parentRect);
            int parentWidth = parentRect.right - parentRect.left;
            int parentHeight = parentRect.bottom - parentRect.top;

            POINT parentTopLeft = { parentRect.left, parentRect.top };
            ::ClientToScreen(hParent, &parentTopLeft);

            int myX = parentTopLeft.x + (parentWidth - WND_SV_WIDTH) / 2;
            int myY = parentTopLeft.y + (parentHeight - WND_SV_HEIGHT) / 2;

            // Register and create the overlay window class
            WNDCLASS wc = {};
            wc.lpfnWndProc = MyWinProc;
            wc.hInstance = ::GetModuleHandle(NULL);
            wc.lpszClassName = L"SOUND_VOLUME_WND";
            wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
            ::RegisterClass(&wc);

            // Transparent, non-interactive overlay
            hWnd = ::CreateWindowEx(
                WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
                L"SOUND_VOLUME_WND", NULL,
                WS_POPUP | WS_CHILD | WS_BORDER,
                myX, myY, WND_SV_WIDTH, WND_SV_HEIGHT, hParent, NULL, ::GetModuleHandle(NULL), NULL);

            // Static label to display volume percentage
            hStatic = ::CreateWindowEx(
                0, L"STATIC", NULL,
                WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
                0, 0, WND_SV_WIDTH, WND_SV_HEIGHT, hWnd, NULL, wc.hInstance, NULL);

            // Configure large font for the static label
            HFONT hFont = ::CreateFont(
                LBL_SV_TEXTSIZE,          // Height (in logical units)
                0, 0, 0,                  // Width, Escapement, Orientation
                FW_NORMAL,                // Weight
                FALSE, FALSE, FALSE,      // Italic, Underline, Strikeout
                DEFAULT_CHARSET,          // Character Set
                OUT_DEFAULT_PRECIS,       // Output Precision
                CLIP_DEFAULT_PRECIS,      // Clipping Precision
                DEFAULT_QUALITY,          // Quality
                DEFAULT_PITCH | FF_SWISS, // Pitch and Family
                L"Arial"                  // Font face name
            );

            ::SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);
            ::SendMessage(hStatic, WM_SETFONT, (WPARAM)hFont, TRUE);

            // Standard Windows message loop
            MSG msg = {};
            while (::GetMessage(&msg, hWnd, 0, 0))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
            }).detach();

        // Load initial system volume and apply mute state from settings
        currentVolume = GetSystemVolume();

        SetSystemVolume(currentVolume);
        SetMute(!setting_data::InitAudState.GetValue());
    }

    static IAudioEndpointVolume* GetVolumeController()
    {
        // Helper to obtain COM interface for system audio control
        IMMDeviceEnumerator* deviceEnumerator = NULL;
        IMMDevice* defaultDevice = NULL;
        IAudioEndpointVolume* endpointVolume = NULL;

        HRESULT hr = ::CoInitialize(NULL);

        if (FAILED(hr))
        {
            return NULL;
        }

        hr = ::CoCreateInstance(
            __uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
            __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator
        );

        if (FAILED(hr))
        {
            return NULL;
        }

        hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
        deviceEnumerator->Release();

        if (FAILED(hr))
        {
            return NULL;
        }

        hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&endpointVolume);
        defaultDevice->Release();

        if (FAILED(hr))
        {
            return NULL;
        }

        return endpointVolume;
    }

    void SetSystemVolume(int percent)
    {
        // Applies a specific volume level and updates the UI overlay
        IAudioEndpointVolume* volumeController = GetVolumeController();

        if (volumeController == NULL)
        {
            return;
        }

        if (percent > 0)
        {
            volumeController->SetMute(FALSE, NULL);
            volumeController->SetMasterVolumeLevelScalar((float)percent / 100.0f, NULL);
        }
        else
        {
            volumeController->SetMute(TRUE, NULL);
        }

        volumeController->Release();
        ::CoUninitialize();

        // Trigger the WM_SETTEXT handler in MyWinProc to show the percentage overlay
        ::SetWindowText(hWnd, (std::to_wstring(percent) + L"%").c_str());
    }

    int GetSystemVolume()
    {
        // Reads the current scalar volume from the default audio endpoint
        IAudioEndpointVolume* volumeController = GetVolumeController();

        if (volumeController == NULL)
        {
            return -1;
        }

        float fVolume = 0.0f;
        HRESULT hr = volumeController->GetMasterVolumeLevelScalar(&fVolume);

        volumeController->Release();
        ::CoUninitialize();

        if (SUCCEEDED(hr))
        {
            return (int)(fVolume * 100.0f + 0.5f); // Convert to integer percentage with rounding
        }

        return -1;
    }

    void ChangeVolume(bool up, bool down)
    {
        // Increments or decrements the cached volume level by a fixed step
        isMute = false;

        if (up)
        {
            if (currentVolume < STANDARD_PERCENT)
            {
                currentVolume += VOLUME_STEP;
            }

            if (currentVolume > STANDARD_PERCENT)
            {
                currentVolume = STANDARD_PERCENT;
            }
        }
        else if (down)
        {
            if (currentVolume > 0)
            {
                currentVolume -= VOLUME_STEP;
            }

            if (currentVolume < 0)
            {
                currentVolume = 0;
                isMute = true;
            }
        }

        SetSystemVolume(currentVolume);
    }

    void SetMute(bool value)
    {
        // Explicitly sets the system-wide mute state
        IAudioEndpointVolume* volumeController = GetVolumeController();

        if (volumeController == NULL)
        {
            return;
        }

        volumeController->SetMute(value, NULL);
        volumeController->Release();

        ::CoUninitialize();

        isMute = value;
    }

    void ToggleMute()
    {
        // Toggles the current mute state
        isMute = !isMute;

        SetMute(isMute);
    }
}
