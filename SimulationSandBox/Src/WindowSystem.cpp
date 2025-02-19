#include "WindowSystem.h"
#include <optional>
#include <stdexcept>
#include <string>

std::unique_ptr<WindowSystem> WindowSystem::instance = nullptr;
const wchar_t WindowSystem::CLASS_NAME[] = L"DXWindowClass_UniqueID";

void WindowSystem::Init(HINSTANCE hInstance, int width, int height, const char* title) {
    if (!instance) {
        instance = std::unique_ptr<WindowSystem>(new WindowSystem(hInstance, width, height, title));
    }
}
WindowSystem* WindowSystem::Get()
{
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

WindowSystem::~WindowSystem(){
    if (mHandleWindow && IsWindow(mHandleWindow))
    {
        DestroyWindow(mHandleWindow);
    }
    UnregisterClass(CLASS_NAME, mhInstance);
}

void WindowSystem::InitClassWindow()
{
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = HandleMsgSetup;
    wc.hInstance = mhInstance;
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClassEx(&wc)) {
        throw std::runtime_error("Failed to register window class");
    }
}

std::optional<int> WindowSystem::ProcessMessage()
{
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return static_cast<int>(msg.message);
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return {};
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

LRESULT WindowSystem::HandleMsgSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
    if (message == WM_NCCREATE)
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        WindowSystem* const pWnd = static_cast<WindowSystem*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowSystem::HandleMsgThunk));
        return pWnd->HandleMsg(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT WindowSystem::HandleMsgThunk(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
    // Retrieve pointer and forward message to window class
    WindowSystem* const pWnd = reinterpret_cast<WindowSystem*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWnd->HandleMsg(hWnd, message, wParam, lParam);
}

LRESULT WindowSystem::HandleMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}