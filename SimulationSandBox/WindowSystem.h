#pragma once
#include <windows.h>
#include <memory>
#include <string>

class WindowSystem {
public:
    static void Init(HINSTANCE hInstance, int width, int height, const char* title);
    static WindowSystem* Get();
    HWND GetHandleWindow() const;
    HINSTANCE GetInstance() const;
    bool ProcessMessage();
    std::string GetWindowName() const;

private:
    WindowSystem(HINSTANCE hInstance, int width, int height, const char* title);
    ~WindowSystem();

    void InitClassWindow();
    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    static std::unique_ptr<WindowSystem> instance; 

    HWND mHandleWindow = nullptr;
    HINSTANCE mhInstance = nullptr;
    std::string windowName;

    static const wchar_t CLASS_NAME[];
};
