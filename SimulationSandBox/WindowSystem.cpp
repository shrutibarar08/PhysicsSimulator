#include "WindowSystem.h"
#include <stdexcept>
#include <iostream>

const wchar_t WindowSystem::CLASS_NAME[] = L"WindowSystemClass";

// Initialize the static singleton instance
std::unique_ptr<WindowSystem> WindowSystem::instance = nullptr;

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

    // Create the window
    mHandleWindow = CreateWindowEx(
        0,
        CLASS_NAME,
        std::wstring(title, title + strlen(title)).c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr, nullptr,
        mhInstance, nullptr
    );

    if (!mHandleWindow) {
        throw std::runtime_error("Failed to create window");
    }

    ShowWindow(mHandleWindow, SW_SHOW);

    // Initialize DirectX after creating the window
    InitDirectX();
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
    wc.style = CS_HREDRAW | CS_VREDRAW;
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

std::string WindowSystem::GetWindowName() const {
    return windowName;
}

HWND WindowSystem::GetHandleWindow() const {
    return mHandleWindow;
}

HINSTANCE WindowSystem::GetInstance() const {
    return mhInstance;
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

void WindowSystem::InitDirectX() {
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = 0; // Match window size
    swapChainDesc.BufferDesc.Height = 0; // Match window size
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = mHandleWindow;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, // Default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,
        nullptr, 0, // Default feature levels
        D3D11_SDK_VERSION,
        &swapChainDesc,
        swapChain.GetAddressOf(),
        device.GetAddressOf(),
        nullptr, // Feature level
        deviceContext.GetAddressOf()
    );

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create DirectX device and swap chain");
    }

    InitRenderTargetView();
}

void WindowSystem::InitRenderTargetView() {
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

    HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to get swap chain back buffer");
    }

    hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create render target view");
    }

    // Bind the render target view to the pipeline
    deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);
}
