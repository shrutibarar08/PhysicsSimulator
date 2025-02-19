#include "WindowSystem.h"
#include <stdexcept>
#include <string>

std::unique_ptr<WindowSystem> WindowSystem::instance = nullptr;
const wchar_t WindowSystem::CLASS_NAME[] = L"DXWindowClass_UniqueID";

void WindowSystem::Init(HINSTANCE hInstance, int width, int height, const char* title) {
    if (!instance) {
        instance = std::unique_ptr<WindowSystem>(new WindowSystem(hInstance, width, height, title));
    }
}

WindowSystem* WindowSystem::Get() {
    return instance.get();
}

WindowSystem::WindowSystem(HINSTANCE hInstance, int width, int height, const char* title)
    : mhInstance(hInstance), windowName(title) {
    InitClassWindow();

    std::wstring windowTitle(windowName.begin(), windowName.end());
    mHandleWindow = CreateWindowEx(
        0, CLASS_NAME, windowTitle.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, mhInstance, nullptr
    );

    if (!mHandleWindow) {
        throw std::runtime_error("Failed to create window");
    }

    ShowWindow(mHandleWindow, SW_SHOW);
}

WindowSystem::~WindowSystem() {
    if (mHandleWindow && IsWindow(mHandleWindow)) {
        DestroyWindow(mHandleWindow);
    }
    UnregisterClass(CLASS_NAME, mhInstance);
}

void WindowSystem::InitClassWindow() {
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = mhInstance;
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClassEx(&wc)) {
        throw std::runtime_error("Failed to register window class");
    }
}

bool WindowSystem::ProcessMessage() {
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

HWND WindowSystem::GetHandleWindow() const {
    return mHandleWindow;
}

HINSTANCE WindowSystem::GetInstance() const {
    return mhInstance;
}

std::string WindowSystem::GetWindowName() const {
    return windowName;
}

LRESULT CALLBACK WindowSystem::WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}
