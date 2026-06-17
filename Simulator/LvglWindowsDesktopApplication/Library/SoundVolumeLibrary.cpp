#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <thread>
#include "CommonData.h"
#include "CommonLibrary.h"

namespace soundvolume_lib
{
    bool isMute = false;

    static HWND hWnd = NULL;
    static HWND hStatic = NULL;
    static int currentVolume = 0;

    static int myWidth = 250;
    static int myHeight = 100;
    static int textSize = 100;

    static void CALLBACK AutoHideWindow(HWND hwnd, UINT uMsg, UINT_PTR timerId, DWORD dwTime)
    {
        ::KillTimer(hwnd, TID_VOLUME_WND);
        ::ShowWindow(hwnd, SW_HIDE);
    }

    static LRESULT CALLBACK MyWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_SETTEXT:
            ::KillTimer(hwnd, TID_VOLUME_WND);

            ::SetWindowText(hStatic, (LPCWSTR)lParam);
            ::ShowWindow(hwnd, SW_SHOW);

            ::SetTimer(hwnd, TID_VOLUME_WND, TIMECYCLE_3SEC, AutoHideWindow);

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
        std::thread([]() {
            HWND hParent = system_data::WindowHandle.GetValue();

            RECT parentRect;
            ::GetClientRect(hParent, &parentRect);
            int parentWidth = parentRect.right - parentRect.left;
            int parentHeight = parentRect.bottom - parentRect.top;

            POINT parentTopLeft = { parentRect.left, parentRect.top };
            ::ClientToScreen(hParent, &parentTopLeft);

            int myX = parentTopLeft.x + (parentWidth - myWidth) / 2;
            int myY = parentTopLeft.y + (parentHeight - myHeight) / 2;

            WNDCLASS wc = {};
            wc.lpfnWndProc = MyWinProc;
            wc.hInstance = ::GetModuleHandle(NULL);
            wc.lpszClassName = L"SOUND_VOLUME_WND";
            wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
            ::RegisterClass(&wc);

            hWnd = ::CreateWindowEx(
                WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
                L"SOUND_VOLUME_WND", NULL,
                WS_POPUP | WS_CHILD | WS_BORDER,
                myX, myY, myWidth, myHeight, hParent, NULL, ::GetModuleHandle(NULL), NULL);

            hStatic = ::CreateWindowEx(
                0, L"STATIC", NULL,
                WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
                0, 0, myWidth, myHeight, hWnd, NULL, wc.hInstance, NULL);

            HFONT hFont = ::CreateFont(
                textSize,                 // Height (in logical units)
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

            MSG msg = {};

            while (::GetMessage(&msg, hWnd, 0, 0))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
            }).detach();

        currentVolume = GetSystemVolume();

        SetSystemVolume(currentVolume);
        SetMute(!setting_data::InitAudState.GetValue());
    }

    static IAudioEndpointVolume* GetVolumeController()
    {
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

        ::SetWindowText(hWnd, (std::to_wstring(percent) + L"%").c_str());
    }

    int GetSystemVolume()
    {
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
            return (int)(fVolume * 100.0f + 0.5f);
        }

        return -1;
    }

    void ChangeVolume(bool up, bool down)
    {
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
            }
        }

        SetSystemVolume(currentVolume);
    }

    void SetMute(bool value)
    {
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
        isMute = !isMute;

        SetMute(isMute);
    }
}
