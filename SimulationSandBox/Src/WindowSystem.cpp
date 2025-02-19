#include "WindowSystem.h"
#include <optional>
#include <stdexcept>
#include <string>

void WindowSystem::Init(HINSTANCE hInstance, int width, int height, const std::wstring& title)
{
    if (!m_instance)
    {
        m_instance = std::unique_ptr<WindowSystem>(new WindowSystem(hInstance, width, height, title));
    }
}

WindowSystem* WindowSystem::Get()
{
    return m_instance.get();
}

WindowSystem::WindowSystem(HINSTANCE hInstance, int width, int height, const std::wstring& title)
    : mhInstance(hInstance), mWindowName(title)
{
    WNDCLASSEX wc{};
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wc.lpszMenuName = nullptr;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = HandleMsgSetup;
    wc.hInstance = mhInstance;
    wc.lpszClassName = mClassName.c_str();

    if (!RegisterClassEx(&wc))
    {
        DWORD error = GetLastError();
        throw std::runtime_error("Failed to register window class. Error code: " + std::to_string(error));
    }

    std::wstring windowTitle(mWindowName.begin(), mWindowName.end());
    mHandleWindow = CreateWindowEx(
        0, mClassName.c_str(), windowTitle.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, mhInstance, nullptr
    );

    if (!mHandleWindow)
    {
        throw std::runtime_error("Failed to create window");
    }

    ShowWindow(mHandleWindow, SW_SHOW);
}

WindowSystem::~WindowSystem()
{
    if (mHandleWindow && IsWindow(mHandleWindow))
    {
        DestroyWindow(mHandleWindow);
    }
    UnregisterClass(mClassName.c_str(), mhInstance);
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

int WindowSystem::GetHeight()
{
    RECT rt;
    GetClientRect(m_instance->GetHandleWindow(), &rt);
    return rt.bottom - rt.top;
}

int WindowSystem::GetWidth()
{
    RECT rt;
    GetClientRect(m_instance->GetHandleWindow(), &rt);
    return rt.right - rt.left;
}

HWND WindowSystem::GetHandleWindow() const
{
    return mHandleWindow;
}

HINSTANCE WindowSystem::GetInstance() const
{
    return mhInstance;
}

std::string WindowSystem::GetWindowName() const
{
    return std::string(mWindowName.begin(), mWindowName.end());
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
