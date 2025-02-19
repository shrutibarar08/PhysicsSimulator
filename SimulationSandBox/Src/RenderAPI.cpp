#include "RenderAPI.h"

RenderAPI::RenderAPI() {}

RenderAPI::~RenderAPI() {
    if (d3d11DeviceContext) {
        d3d11DeviceContext->ClearState(); 
    }
}

void RenderAPI::InitDeviceAndSwapChain(HWND hwnd, int width, int height) {
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;

    UINT createDeviceFlags = 0;
#if defined(_DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG; 
#endif

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,                     
        D3D_DRIVER_TYPE_HARDWARE,    
        nullptr,                     
        createDeviceFlags,          
        featureLevels,               
        ARRAYSIZE(featureLevels),   
        D3D11_SDK_VERSION,           
        &swapChainDesc,              
        dxgiSwapChain.GetAddressOf(),
        d3d11Device.GetAddressOf(),  
        &featureLevel,               
        d3d11DeviceContext.GetAddressOf() 
    );

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create device and swap chain");
    }
}

void RenderAPI::InitRenderTarget() {
    //back buffer from the swap chain
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    HRESULT hr = dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to get back buffer");
    }

    //render target view from the back buffer
    hr = d3d11Device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create render target view");
    }

    d3d11DeviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);
}

void RenderAPI::InitViewport(int width, int height) {
    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<FLOAT>(width);
    viewport.Height = static_cast<FLOAT>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    d3d11DeviceContext->RSSetViewports(1, &viewport);
}

void RenderAPI::PreDraw(float clearColor[4]) {
    // Clear the render target view with the specified color
    d3d11DeviceContext->ClearRenderTargetView(renderTargetView.Get(), clearColor);
}

void RenderAPI::Draw() {
    
}

void RenderAPI::PostDraw() {
    //Present the back buffer to the screen
    HRESULT hr = dxgiSwapChain->Present(1, 0); 
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to present the swap chain buffer");
    }
}
