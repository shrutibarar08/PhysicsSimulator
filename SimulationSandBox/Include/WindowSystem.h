#pragma once
#include <windows.h>
#include <memory>
#include <string>
#include <optional>

class WindowSystem
{
public:
    static void Init(HINSTANCE hInstance, int width, int height, const std::wstring& title);
    static WindowSystem* Get();
    static std::optional<int> ProcessMessage();

    static int GetHeight();
    static int GetWidth();

	HWND GetHandleWindow() const;
    HINSTANCE GetInstance() const;
    std::string GetWindowName() const;

    ~WindowSystem();

private:
    WindowSystem(HINSTANCE hInstance, int width, int height, const std::wstring& title);

    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT HandleMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

private:
    HWND mHandleWindow;
    HINSTANCE mhInstance;
    std::wstring mWindowName;

    std::wstring mClassName{L"WindowManagerClass"};
    inline static std::unique_ptr<WindowSystem> m_instance = nullptr;
};
