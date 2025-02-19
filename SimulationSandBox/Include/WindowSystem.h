#pragma once
#include <windows.h>
#include <memory>
#include <string>
#include <optional>

class WindowSystem {
public:
    static void Init(HINSTANCE hInstance, int width, int height, const char* title);
    static WindowSystem* Get();
    HWND GetHandleWindow() const;
    HINSTANCE GetInstance() const;
    static std::optional<int> ProcessMessage();
    std::string GetWindowName() const;

    ~WindowSystem();

private:
    WindowSystem(HINSTANCE hInstance, int width, int height, const char* title);
    

    void InitClassWindow();

    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT HandleMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

    static std::unique_ptr<WindowSystem> instance; 

    HWND mHandleWindow = nullptr;
    HINSTANCE mhInstance = nullptr;
    std::string windowName;

    static const wchar_t CLASS_NAME[];
};
