#include <windows.h>
#include "RenderAPI.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int) {
    const wchar_t CLASS_NAME[] = L"SampleWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Direct3D 11 - Clear Screen",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    ShowWindow(hwnd, SW_SHOW);

    RenderAPI renderAPI;
    renderAPI.InitDeviceAndSwapChain(hwnd, 800, 600);
    renderAPI.InitRenderTarget();
    renderAPI.InitViewport(800, 600);

    MSG msg = {};
    while (true) {
        if (WindowsSy)
        {
	        
        }
        
        else {
            float clearColor[4] = { 1.0, 0.7529, 0.7961 }; // Dark blue
            renderAPI.PreDraw(clearColor);
            renderAPI.Draw();
            renderAPI.PostDraw();
        }
    }

    return 0;
}
