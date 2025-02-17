#pragma once

#include <windows.h>
#include <memory>
#include <string>
#include <d3d11.h>
#include <wrl/client.h>

class WindowSystem {
public:
    static void Init(HINSTANCE hInstance, int width, int height, const char* title);

    static WindowSystem* Get();

    HWND GetHandleWindow() const;
    HINSTANCE GetInstance() const;
    std::string GetWindowName() const;

    //Windows messages
    bool ProcessMessage();

    //DirectX accessors
    ID3D11Device* GetDevice() const { return device.Get(); }
    ID3D11DeviceContext* GetDeviceContext() const { return deviceContext.Get(); }
    IDXGISwapChain* GetSwapChain() const { return swapChain.Get(); }
    ID3D11RenderTargetView* GetRenderTargetView() const { return renderTargetView.Get(); }

    //device, context, and swap chain
    void InitDirectX();

private:
    WindowSystem(HINSTANCE hInstance, int width, int height, const char* title);
    ~WindowSystem();

    void InitClassWindow();
    void InitRenderTargetView();

    //callback
    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    //Singleton instance of WindowSystem
    static std::unique_ptr<WindowSystem> instance;

    //Window and application-specific variables
    HWND mHandleWindow = nullptr;
    HINSTANCE mhInstance = nullptr;
    std::string windowName;

    //DirectX 11 objects managed by WRL ComPtr
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

    static const wchar_t CLASS_NAME[];
};
